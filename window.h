#ifndef WINDOW_H
#define WINDOW_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions>
#include <model.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <camera.h>
#include <QTime>

//光照
class lighting
{
public:
    lighting() {}
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
};
//材料
class material
{
public:
    material() {}
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

class Window : public QGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Window(QMainWindow *parent = 0);
    void RotateX(float angle);
    void RotateY(float angle);
    //20171220新增
    void resetCamera();//每次迭代后重置摄像机位置
    void homeReset();//Home键重置摄像机位置

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Camera cam;
    StlModel* model;
    StlModel* temp;
    QList<StlModel*> modelList;//所有模型加到一个模型链表中
    QVector3D centerInOpenGLWid;
    QPointF lastPos;
    //20171219新增
    int iterationNumber;
    double stride;
    bool isVariantPace;

signals:


public slots:
    void slot_openStl_inopengl(QString path);//在opengl窗口中加载STL模型数据
    void slot_finishLoadStl();//模型加载完成，开始绘制第一个模型
    void slot_iteration();//迭代产生新模型
    //20171219新增
    void slot_receivePara(int iterNum, bool isVariant, double Stride);//接收Dialog中设置的参数
    void slot_startIter();//执行指定次数的迭代
    //20171220新增
    void slot_resetModel();//清除迭代产生的模型

};

#endif // WINDOW_H
