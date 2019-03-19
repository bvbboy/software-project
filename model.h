#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <facet.h>
#include <QList>
#include <GL/glu.h>
#include <QTextStream>
#include <QDebug>
#include <math.h>

//模型包围盒
class modelBox
{
public:
    modelBox() {}
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
};

class StlModel : public QObject
{
    Q_OBJECT
public:
    explicit StlModel(QObject *parent);

    void model_load(QString path);//模型导入
    void model_clear();//清除模型
    modelBox model_getBox();//获取模型包围盒
    QVector3D model_getCenter(modelBox box);//获取模型中点
    void model_updateBox(QVector3D point);//更新包围盒
    int model_facetNum();//面的数目
    void model_iteration(StlModel* oldmodel);//模型迭代
    void model_print(int i);//测试用，输出三角形数据

    QList<facet*> facetList;//存储每一个面的链表
    double stride=0.1;//迭代时步长
    bool isVariantPace=false;//随机步长判断标志

private:
    modelBox box;
    QVector3D getCoord(QString row);//从STL文件中获取坐标
    float getLength(QVector3D p1, QVector3D p2);//计算两点距离

signals:
    void signal_loaded();//加载完成信号
public slots:

};

#endif // MODEL_H
