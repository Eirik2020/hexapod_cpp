#include "TCP_server.h"


// Constructor to initialize the server on a specific port
TCP_server::TCP_server(unsigned short port)
    : port_(port),
    io_context_(),
    acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    socket_(nullptr) {}

// Starts the server and waits for a client to connect
void TCP_server::start() {
    socket_ = std::make_unique<boost::asio::ip::tcp::socket>(io_context_);
    std::cout << "Server is running on port " << port_ << ". Waiting for connections..." << std::endl;

    // Wait for a client connection
    acceptor_.accept(*socket_);
    std::cout << "Client connected." << std::endl;
}

// Receives a single character from the client
char TCP_server::receiveMessage() {
    char receivedChar;
    boost::system::error_code error;

    // Read a single character from the client
    size_t length = socket_->read_some(boost::asio::buffer(&receivedChar, 1), error);

    if (error == boost::asio::error::eof) {
        std::cout << "Connection closed by client." << std::endl;
        socket_.reset();  // Reset the socket to indicate disconnection
        return '\0';  // Return a null character to indicate disconnection
    }
    else if (error) {
        throw boost::system::system_error(error);  // Handle other errors
    }

    return receivedChar;  // Return the received character
}

// Check if the client is still connected
bool TCP_server::isClientConnected() const {
    return socket_ != nullptr && socket_->is_open();
}