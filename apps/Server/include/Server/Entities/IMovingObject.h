#pragma once

#include <Server/Math/Angle.h>
#include <Server/Math/Vector.h>

class IMovingObject
{
public:
    virtual Vector<double, 2> getPosition() const = 0;
    virtual Vector<double, 2> getVelocity() const = 0;
    virtual void setPosition(const Vector<double, 2> &newV) = 0;
    virtual ~IMovingObject() = default;
};
