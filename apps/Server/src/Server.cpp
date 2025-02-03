#include <cmath>
#include <iostream>
#include <memory>

#include <Server/Entities/Commands.h>
#include <Server/Entities/MovingObjectAdapter.h>
#include <Server/Entities/UObject.h>

#include <Server/Server.h>

int main()
{
    // std::shared_ptr<UObject> object = std::make_shared<UObject>(Vector<double, 2>({2, 0}),
    //                                                             Vector<double, 2>({1, 1}));

    // std::cout << "Initial Position:" << *object;

    // auto moveCommand = std::make_shared<Move>(object);
    // auto rotateCommand = std::make_shared<Rotate>(object, 123);

    // // Создаем менеджер команд
    // CommandManager manager;

    // // Добавляем команды в менеджер
    // manager.AddCommand(moveCommand);
    // manager.AddCommand(rotateCommand);

    // // Выполняем все команды
    // manager.ExecuteAll();

    // Выводим результат
    std::cout << "Server started:" << std::endl;

    try {
        boost::asio::io_context io_context;
        //int server_port = std::stoi(argv[1]);
        int agent_port = 12345;
        int player_port = 12346;
        GameServer server(io_context, agent_port, player_port);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
