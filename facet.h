#ifndef FACET_H
#define FACET_H

#include <QObject>
#include <QVector>
#include <QVector3D>

class facet : public QObject
{
    Q_OBJECT
public:
    explicit facet();

    QVector3D getNormalVector();//获取面的法向量
    QVector<QVector3D> getTriangle();//获取面的三角形数据
    void setNormalVector(QVector3D &value);//设置面的法向量
    void setTriangle(QVector<QVector3D> &value);//设置面的三角形数据

private:
    QVector3D normalVector;
    QVector<QVector3D> triangle;


signals:

public slots:
};

#endif // FACET_H
