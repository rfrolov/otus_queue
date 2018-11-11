#include "JoinServer.h"
#include "ClientSession.h"

JoinServer::JoinServer(uint16_t port) :
        m_service{}
        , m_acceptor{m_service, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)}
        , m_port{port}
        , m_socket{m_service} {
}

void JoinServer::run() {
    ba::signal_set signals(m_service, SIGINT, SIGTERM);
    signals.async_wait(
            [this, &signals](const boost::system::error_code &/*error*/, int /*signal_number*/) { do_stop(); });
    do_accept();
    m_service.run();
}

void JoinServer::do_stop() {
    m_service.stop();
}

void JoinServer::do_accept() {
    m_acceptor.async_accept(m_socket,
                            [this](const boost::system::error_code &err) {
                                if (!err) {
                                    std::make_shared<ClientSession>(std::move(m_socket))->start();
                                }
                                do_accept();
                            });
}