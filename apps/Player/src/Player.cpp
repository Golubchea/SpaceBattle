#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Player
{
public:
    Player(boost::asio::io_context &io_context, const std::string &server_ip, int server_port)
        : socket_(io_context)
        , resolver_(io_context)
    {
        connect_to_server(server_ip, server_port);
    }

private:
    void connect_to_server(const std::string &server_ip, int server_port)
    {
        auto endpoints = resolver_.resolve(server_ip, std::to_string(server_port));
        boost::asio::async_connect(socket_,
                                   endpoints,
                                   [this](const boost::system::error_code &error,
                                          const tcp::endpoint &) {
                                       if (!error) {
                                           std::cout << "Connected to server" << std::endl;
                                           start_reading();
                                       } else {
                                           std::cerr << "Connection failed: " << error.message()
                                                     << std::endl;
                                       }
                                   });
    }

    void start_reading()
    {
        boost::asio::async_read_until(
            socket_,
            buffer_,
            '\n', // Предполагается, что сообщения заканчиваются символом '\n'
            [this](const boost::system::error_code &error, std::size_t /*length*/) {
                if (!error) {
                    std::istream is(&buffer_);
                    std::string message;
                    std::getline(is, message);
                    std::cout << "Server update: " << message << std::endl;

                    // Продолжаем чтение
                    start_reading();
                } else {
                    std::cerr << "Error reading from server: " << error.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    tcp::resolver resolver_;
    boost::asio::streambuf buffer_;
};

int main(int argc, char* argv[]) {
    // if (argc != 3) {
    //     std::cerr << "Usage: player <server_ip> <server_port>\n";
    //     return 1;
    // }

    try {
        boost::asio::io_context io_context;
        // std::string server_ip = argv[1];
        // int server_port = std::stoi(argv[2]);

        std::string server_ip = "127.0.0.1";
        int server_port = 12346;

        Player player(io_context, server_ip, server_port);
        io_context.run();

    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
