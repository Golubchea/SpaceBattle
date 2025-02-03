#pragma once

#include <Server/Entities/IMovingObject.h>
#include <Server/Entities/UObject.h>
#include <Server/Math/Angle.h>
#include <Server/Math/Vector.h>
#include <cmath> // Для std::cos и std::sin
#include <memory>
// Предварительное объявление класса UObject

// Адаптер для IMovingObject
class MovingObjectAdapter : public IMovingObject
{
    std::shared_ptr<UObject> obj; // Используем ссылку на UObject

public:
    // Конструктор принимает ссылку на UObject
    MovingObjectAdapter(std::shared_ptr<UObject> object)
        : obj(object)
    {}

    // Реализация методов IMovingObject
    Vector<double, 2> getPosition() const override { return obj->getPosition(); }

    Vector<double, 2> getVelocity() const override
    {
        Vector<double, 2> velocity = obj->getVelocity();
        double angleRadians = obj->getAngle().getRadians(); // Угол в радианах
        // Применяем угол к скорости
        return Vector<double, 2>(
            {velocity.x() * std::cos(angleRadians) - velocity.y() * std::sin(angleRadians),
             velocity.x() * std::sin(angleRadians) + velocity.y() * std::cos(angleRadians)});
    }

    void setPosition(const Vector<double, 2> &newV) override
    {
        obj->setPosition(newV); // Устанавливаем новую позицию
    }
};

// Адаптер для IMovingObject
class RotatingObjectAdapter : public IMovingObject
{
    std::shared_ptr<UObject> obj; // Используем ссылку на UObject

public:
    // Конструктор принимает ссылку на UObject
    RotatingObjectAdapter(std::shared_ptr<UObject> object)
        : obj(object)
    {}

    // Реализация методов IMovingObject
    Vector<double, 2> getPosition() const override { return obj->getPosition(); }

    Vector<double, 2> getVelocity() const override
    {
        double angleRadians = obj->getAngle().getRadians(); // Угол в радианах
        double speed = std::sqrt(obj->getVelocity().x() * obj->getVelocity().x()
                                 + obj->getVelocity().y()
                                       * obj->getVelocity().y()); // Модуль скорости
        // Новые компоненты скорости
        double newVx = speed * std::cos(angleRadians);
        double newVy = speed * std::sin(angleRadians);
        // Устанавливаем новую скорость

        return Vector<double, 2>({newVx, newVy});
    }

    void setPosition(const Vector<double, 2> &newV) override
    {
        obj->setPosition(newV); // Устанавливаем новую позицию
    }
};
