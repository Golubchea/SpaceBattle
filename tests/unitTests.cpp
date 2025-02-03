#include "Server/Entities/Commands.h"
#include "Server/Entities/UObject.h"
#include "Server/Math/Angle.h"
#include "Server/Math/Vector.h"
#include <cmath>
#include <gtest/gtest.h>
// Тест: Движение объекта
TEST(MoveCommandTest, MovesObjectCorrectly)
{
    // Инициализация объекта
    auto object = std::make_shared<UObject>(Vector<double, 2>({12, 5}), Vector<double, 2>({-7, 3}));

    // Создание команды Move
    auto moveCommand = std::make_shared<Move>(object);

    // Выполнение команды
    moveCommand->Execute();

    // Проверка нового положения объекта
    EXPECT_NEAR(object->getPosition().x(), 5, 1e-6);
    EXPECT_NEAR(object->getPosition().y(), 8, 1e-6);
}

// Тест: Ошибка при чтении позиции
TEST(MoveCommandTest, FailsWhenPositionIsUnreadable) {
    // Создаем объект, который выбрасывает исключение при попытке чтения позиции

    auto faultyObject = std::make_shared<UObject>(Vector<double, 2>({std::nanf(""), std::nanf("")}),
                                                  Vector<double, 2>({1, 1}));
    auto moveCommand = std::make_shared<Move>(faultyObject);

    // Проверяем, что команда выбрасывает исключение
    EXPECT_THROW(moveCommand->Execute(), std::runtime_error);
};

// Тест: Ошибка при чтении скорости
TEST(MoveCommandTest, FailsWhenVelocityIsUnreadable) {
    // Создаем объект, который выбрасывает исключение при попытке чтения скорости

    auto faultyObject = std::make_shared<UObject>(Vector<double, 2>({1, 1}),
                                                  Vector<double, 2>({std::nanf(""), std::nanf("")}));
    auto moveCommand = std::make_shared<Move>(faultyObject);

    // Проверяем, что команда выбрасывает исключение
    EXPECT_THROW(moveCommand->Execute(), std::runtime_error);
}

// Тест: Вращение объекта
TEST(RotateCommandTest, RotatesObjectCorrectly) {
    // Инициализация объекта
    auto object = std::make_shared<UObject>(Vector<double, 2>({0, 0}), Vector<double, 2>({1, 0}));
    object->setAngle(Angle(0)); // Начальный угол 0 градусов

    // Создание команды Rotate
    auto rotateCommand = std::make_shared<Rotate>(object, 90); // Поворот на 90 градусов

    // Выполнение команды
    rotateCommand->Execute();

    // Проверка нового угла и скорости
    const double rotate = M_PI / 2.0;
    EXPECT_NEAR(object->getAngle().getRadians(), rotate, 1e-6); // 90 градусов в радианах
    EXPECT_NEAR(object->getVelocity().x(), 0, 1e-6);
    EXPECT_NEAR(object->getVelocity().y(), 1, 1e-6);
}

// Тест: Ошибка при изменении позиции
// TEST(MoveCommandTest, FailsWhenPositionIsUnwritable) {
//     // Создаем объект, который выбрасывает исключение при попытке изменения позиции
//     class FaultyUObject : public UObject {
//     public:
//         FaultyUObject() : UObject(Vector<double, 2>({0, 0}), Vector<double, 2>({0, 0})) {}

//         void setPosition(const Vector<double, 2> &) override {
//             throw std::runtime_error("Cannot set position");
//         }
//     };

//     auto faultyObject = std::make_shared<FaultyUObject>();
//     auto moveCommand = std::make_shared<Move>(faultyObject);

//     // Проверяем, что команда выбрасывает исключение
//     EXPECT_THROW(moveCommand->Execute(), std::runtime_error);
// }
