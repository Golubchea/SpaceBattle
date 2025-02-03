#include <Server/Entities/Commands.h>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
using boost::asio::ip::tcp;

class Agent
{
public:
    Agent(boost::asio::io_context &io_context, const std::string &server_ip, int server_port)
        : socket_(io_context)
        , resolver_(io_context)
    {
        connect_to_server(server_ip, server_port);
        start_reading();
    }

    void send_command(const std::string &command)
    {
        std::vector<uint8_t> buffer;

        if (command.find("move(") == 0) {
            // Парсим move(x, y)
            int x, y;
            sscanf(command.c_str(), "move(%d,%d)", &x, &y);
            buffer = serializeMove(/*x, y*/);
        } else if (command.find("rotate(") == 0) {
            // Парсим rotate(angle)
            int angle;
            sscanf(command.c_str(), "rotate(%d)", &angle);
            buffer = serializeRotate(angle);
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            return;
        }

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(buffer),
                                 [command](const boost::system::error_code &error,
                                           std::size_t /*length*/) {
                                     if (error) {
                                         std::cerr << "Error sending command: " << error.message()
                                                   << std::endl;
                                     } else {
                                         std::cout << "Command sent: " << command << std::endl;
                                     }
                                 });
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
                                       } else {
                                           std::cerr << "Connection failed: " << error.message()
                                                     << std::endl;
                                       }
                                   });
    }

    void start_reading()
    {
        auto buffer = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(socket_,
                                      *buffer,
                                      '\n',
                                      [this, buffer](const boost::system::error_code &error,
                                                     std::size_t /*length*/) {
                                          if (!error) {
                                              std::istream is(buffer.get());
                                              std::string message;
                                              std::getline(is, message);
                                              std::cout << "Received from server: " << message
                                                        << std::endl;

                                              // Продолжаем чтение следующих данных
                                              start_reading();
                                          } else {
                                              std::cerr << "Error reading from server: "
                                                        << error.message() << std::endl;
                                              socket_.close();
                                          }
                                      });
    }

    tcp::socket socket_;
    tcp::resolver resolver_;
};

int main(int argc, char *argv[])
{
    try {
        boost::asio::io_context io_context;

        std::string server_ip = "127.0.0.1";
        int server_port = 12345;

        Agent agent(io_context, server_ip, server_port);

        // Запускаем отдельный поток для чтения ввода пользователя
        std::thread input_thread([&agent, &io_context]() {
            std::string command;
            while (true) {
                std::cout << "Enter command: ";
                std::getline(std::cin, command);
                if (command == "exit") {
                    break;
                }
                agent.send_command(command);
            }
        });

        // Запускаем цикл обработки событий
        io_context.run();

        // Ждем завершения потока ввода
        input_thread.join();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
