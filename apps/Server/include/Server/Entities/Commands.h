#pragma once
#include <Server/Entities/IMovingObject.h>
#include <Server/Entities/MovingObjectAdapter.h>
#include <Server/Entities/ObjectManager.h>

#include <Server/Entities/UObject.h>
#include <cstring>
#include <memory>
#include <vector>

// Вспомогательные функции для сериализации примитивных типов
template<typename T>
void serializePrimitive(const T &value, std::vector<uint8_t> &buffer)
{
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
}

template<typename T>
T deserializePrimitive(const uint8_t *&buffer)
{
    T value;
    std::memcpy(&value, buffer, sizeof(T));
    buffer += sizeof(T);
    return value;
}

// Функция для сериализации Move
std::vector<uint8_t> serializeMove(/*int x, int y*/)
{
    std::vector<uint8_t> buffer;
    serializePrimitive(static_cast<uint8_t>(0), buffer); // Тип команды: Move
    // serializePrimitive(x, buffer);                       // Координата X
    // serializePrimitive(y, buffer);                       // Координата Y
    return buffer;
}

// Функция для сериализации Rotate
std::vector<uint8_t> serializeRotate(int angle)
{
    std::vector<uint8_t> buffer;
    serializePrimitive(static_cast<uint8_t>(1), buffer); // Тип команды: Rotate
    serializePrimitive(angle, buffer);                   // Угол поворота
    return buffer;
}

// Интерфейс для команды
class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;

    // Статический метод для десериализации команд
};

// Реализация команды Move
class Move : public ICommand
{
public:
    Move(std::shared_ptr<UObject> object)
        : m_object(std::move(object))
    {}

    void Execute() override
    {
        if (m_object) {
            //m_object->setPosition(m_object->getPosition() + m_object->getVelocity());

            MovingObjectAdapter adapter(m_object);

            Vector<double, 2> newPosition = adapter.getPosition() + adapter.getVelocity();
            adapter.setPosition(newPosition);
        }
    }

    // Статический метод для десериализации Move
    static std::shared_ptr<Move> Deserialize(const uint8_t *&buffer, std::shared_ptr<UObject> object)
    {
        // int x = deserializePrimitive<int>(buffer); // Десериализуем координату X
        // int y = deserializePrimitive<int>(buffer); // Десериализуем координату Y
        return std::make_shared<Move>(object);
    }

private:
    std::shared_ptr<UObject> m_object;
};

// Реализация команды Rotate
class Rotate : public ICommand
{
public:
    Rotate(std::shared_ptr<UObject> object, double angle)
        : m_object(std::move(object))
        , angle(angle)
    {}

    void Execute() override
    {
        if (m_object) {
            // Получаем текущий угол и скорость вращения
            Angle currentAngle = m_object->getAngle();
            // Вычисляем новый угол

            m_object->setAngle(currentAngle + angle);
            // Обновляем направление скорости

            RotatingObjectAdapter adapter(m_object);
            m_object->setVelocity(adapter.getVelocity());
        }
    }

    // Статический метод для десериализации Rotate
    static std::shared_ptr<Rotate> Deserialize(const uint8_t *&buffer,
                                               std::shared_ptr<UObject> object)
    {
        int angle = deserializePrimitive<int>(buffer); // Десериализуем угол поворота
        return std::make_shared<Rotate>(object, angle);
    }

private:
    std::shared_ptr<UObject> m_object;
    Angle angle;
};

namespace Command {
std::shared_ptr<ICommand> Deserialize(const uint8_t *buffer, std::shared_ptr<UObject> object)
{
    uint8_t type = deserializePrimitive<uint8_t>(buffer); // Читаем тип команды
    switch (type) {
    case 0: // Move
        return Move::Deserialize(buffer, object);
    case 1: // Rotate
        return Rotate::Deserialize(buffer, object);
    default:
        throw std::runtime_error("Unknown command type");
    }
}
} // namespace Command

// Класс для управления командами
class CommandManager
{
public:
    void AddCommand(std::shared_ptr<ICommand> command) { m_commands.push_back(command); }

    void ExecuteAll()
    {
        for (const auto &command : m_commands) {
            command->Execute();
        }
        m_commands.clear(); // Очищаем список после выполнения
    }

private:
    std::vector<std::shared_ptr<ICommand>> m_commands;
};
