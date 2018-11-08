#pragma once

#include "boost/asio.hpp"
#include <functional>
#include <list>

namespace ba = boost::asio;

struct JoinServer {
    explicit JoinServer(uint16_t port);

    JoinServer(const JoinServer &) = delete;
    void operator=(const JoinServer &) = delete;

    void run();

private:
    using socket_t  =  ba::ip::tcp::socket;

    void do_accept();
    void do_stop();

    ba::io_service        m_service;
    ba::ip::tcp::acceptor m_acceptor;
    uint16_t              m_port;
    socket_t              m_socket;
};

