#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "hexapod_config.h"


class TCP_server {
public:
    // Constructor to initialize the server on a specific port
    TCP_server(unsigned short port);

    // Starts the server and waits for a client to connect
    void start();

    // Receives a single character from the client
    char receiveMessage();

    // Check if the client is still connected
    bool isClientConnected() const;

private:
    unsigned short port_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;  // Use a pointer to manage connection state
};

#endif // TCPSERVER_H
