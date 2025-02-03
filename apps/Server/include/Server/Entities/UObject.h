#pragma once

#include <Server/Math/Angle.h>
#include <Server/Math/Vector.h>
#include <sstream>
#include <vector>
// class ObjectImpl : public UObject
// {
//     std::map<std::string, UObject> values;
//     UObject getValue(std::string key) { return values[key]; }

//     void setValue(std::string key, UObject obj) { values[key] = obj; }
// }

// Класс UObject
class UObject
{
    Vector<double, 2> position;
    Vector<double, 2> velocity;
    Angle angle;

public:
    // Конструктор
    UObject(const Vector<double, 2> &pos, const Vector<double, 2> &vel)
        : position(pos)
        , velocity(vel)
        , angle(0)
    {}

    // Методы для получения и установки позиции
    Vector<double, 2> getPosition() const
    {
        // Проверяем каждую компоненту вектора
        if (std::isnan(position.x()) || std::isinf(position.x()) || std::isnan(position.y())
            || std::isinf(position.y())) {
            throw std::runtime_error("Invalid position: contains NaN or Inf");
        }
        return position;
    }
    void setPosition(const Vector<double, 2> &newV) { position = newV; }

    // Методы для получения и установки скорости
    Vector<double, 2> getVelocity() const
    {
        if (std::isnan(velocity.x()) || std::isinf(velocity.x()) || std::isnan(velocity.y())
            || std::isinf(velocity.y())) {
            throw std::runtime_error("Invalid velocity: contains NaN or Inf");
        }

        return velocity;
    }
    void setVelocity(const Vector<double, 2> &newV) { velocity = newV; }

    // Методы для получения и установки угла
    Angle getAngle() const { return angle; }
    void setAngle(const Angle &newAngle) { angle = newAngle; }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "Position: " << position << "\n"
            << "Velocity: " << velocity << "\n"
            << "Angle: " << angle << "\n";
        return oss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const UObject &obj)
    {
        os << "(" << " Position: " << obj.getPosition() << " \n";
        os << " " << " Velocity: " << obj.getVelocity() << " \n";
        os << " " << " Angle: " << obj.getAngle() << " ) \n";
        return os;
    }
};

std::ostream &operator<<(std::ostream &os, const std::vector<unsigned char> &obj)
{
    for (auto &&it : obj) {
        os << it;
    }
    return os;
}
