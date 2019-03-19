#include "facet.h"

facet::facet() : QObject()
{

}

QVector3D facet::getNormalVector()
{
    return normalVector;
}

QVector<QVector3D> facet::getTriangle()
{
    return triangle;
}

void facet::setNormalVector(QVector3D &value)
{
    normalVector = value;
}

void facet::setTriangle(QVector<QVector3D> &value)
{
    triangle = value;
}
