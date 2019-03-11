#include "shape.h"
#include "utils.h"

Shape::Shape()
    : duringTransaction(false), autoCenter(true)
{

}

void Shape::beginTransaction()
{
    duringTransaction = true;
}

void Shape::commitTransaction()
{
    duringTransaction = false;
}

void Shape::rollbackTransaction()
{
    duringTransaction = false;
}

QPoint Shape::getCenter()
{
    return autoCenter ? getRectHull().center() : center;
}

void Shape::setCenter(const QPoint &newCenter)
{
    center = newCenter;
    autoCenter = Utils::isClose(getRectHull().center(), center, 6);
}
