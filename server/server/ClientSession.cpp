#include <iostream>
#include "ClientSession.h"
#include "../query/QueryParser.h"

ClientSession::ClientSession(socket_t socket) :
        m_started{false}
        , m_read_buffer{}
        , m_socket{std::move(socket)} {
}

ClientSession::~ClientSession() {
    stop();
}

void ClientSession::start() {
    m_self    = shared_from_this();
    m_started = true;
    do_read();
}

void ClientSession::stop() {
    if (!m_started) return;
    m_started = false;
    m_socket.close();
    m_self.reset();
}

void ClientSession::do_read() {
    ba::async_read_until(m_socket, m_read_buffer, '\n', [this](const boost::system::error_code &err, size_t bytes) {
        on_read(err, bytes);
    });
}

void ClientSession::on_read(const boost::system::error_code &err, size_t /*data_size*/) {
    if (!m_started) {
        return;
    }

    if (err) {
        stop();
        return;
    }

    std::istream is(&m_read_buffer);
    std::string  data;
    std::getline(is, data);

    QueryParser               parser;
    std::unique_ptr<ICommand> cmd;

    auto result = parser(data, cmd);
    if (!result.empty()) {
        do_write(result.dump() + "\n");
        do_read();
    } else {
        m_future_result = cmd->execute();
        do_check_result();
    }
}

void ClientSession::do_check_result() {
    auto &service = m_socket.get_io_service();
    service.post([this] { on_check_result(); });
}

void ClientSession::on_check_result() {
    if (m_future_result.wait_for(std::chrono::microseconds(0)) == std::future_status::ready) {
        do_write(m_future_result.get().dump() + "\n");
        do_read();
    } else {
        do_check_result();
    }
}

void ClientSession::do_write(std::string result) {
    std::vector<boost::asio::const_buffer> buffers{boost::asio::buffer(result)};
    m_socket.async_write_some(buffers, [](boost::system::error_code /*error*/, std::size_t /*bytes*/) {});
}
