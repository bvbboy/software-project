#include "window.h"
#include <QDebug>
#include <QString>

Window::Window(QMainWindow *parent) :
    QGLWidget(parent)
{
    setMinimumHeight(400);
    setMinimumWidth(600);

    model = new StlModel(this);
    modelList.append(model);//初始化，并将第一个模型加到链表中
    connect(model,SIGNAL(signal_loaded()),this,SLOT(slot_finishLoadStl()));
    this->setFocusPolicy(Qt::ClickFocus);
}

void Window::RotateX(float angle)
{
    float d=cam.getDist();
    int cnt=100;
    float theta=angle/cnt;
    float slide_d=-2*d*sin(theta*3.14159265/360);
    cam.yaw(theta/2);
    for(;cnt!=0;--cnt)
    {
        cam.slide(slide_d,0,0);
        cam.yaw(theta);
    }
    cam.yaw(-theta/2);
}

void Window::RotateY(float angle)
{
    float d=cam.getDist();
    int cnt=100;
    float theta=angle/cnt;
    float slide_d=2*d*sin(theta*3.14159265/360);
    cam.pitch(theta/2);
    for(;cnt!=0;--cnt)
    {
        cam.slide(0,slide_d,0);
        cam.pitch(theta);
    }
    cam.pitch(-theta/2);
}


void Window::initializeGL()
{
    initializeOpenGLFunctions();

    lighting light;
    light.ambient[0] = 1.0f;
    light.ambient[1] = 1.0f;
    light.ambient[2] = 1.0f;
    light.ambient[3] = 1.0f;

    light.diffuse[0] = 1.0f;
    light.diffuse[1] = 1.0f;
    light.diffuse[2] = 1.0f;
    light.diffuse[3] = 1.0f;

    light.specular[0] = 0.2f;
    light.specular[1] = 0.2f;
    light.specular[2] = 0.2f;
    light.specular[3] = 1.0f;

    GLfloat LightPosition[] = {10.0f, 50.0f, 10.0f, 1.0f};

    material mtrl;
    mtrl.ambient[0] = 0.19225f;
    mtrl.ambient[1] = 0.19225f;
    mtrl.ambient[2] = 0.19225f;
    mtrl.ambient[3] = 1.0f;

    mtrl.diffuse[0] = 0.50754f;
    mtrl.diffuse[1] = 0.50754f;
    mtrl.diffuse[2] = 0.50754f;
    mtrl.diffuse[3] = 1.0f;

    mtrl.specular[0] = 0.508273f;
    mtrl.specular[1] = 0.508273f;
    mtrl.specular[2] = 0.508273f;
    mtrl.specular[3] = 1.0f;

    mtrl.shininess = 0.4f;


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light.ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light.diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light.specular);
    glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mtrl.ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mtrl.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mtrl.specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mtrl.shininess);

    glClearColor(0.2f,0.2f,0.2f,0.0f);//设置背景颜色
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void Window::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat)width/height,0.1,500);//设置透视投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void Window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    cam.setModelViewMatrix();
    //画出modellist中所有模型
    for (int i=0; i<modelList.size(); i++)
    {
        //设置后一个模型的偏移位置
        modelBox box = modelList.at(i)->model_getBox();
        float xoffset = box.x_max - box.x_min;
        if (i>0)
            glTranslatef(1.1*xoffset,0,0);

        QVector3D vn;
        QVector<QVector3D> triangle;
        GLfloat normalVector[3];
        for(int j=0; j<modelList.at(i)->facetList.size(); j++)
        {
            vn = modelList.at(i)->facetList.at(j)->getNormalVector();
            vn.normalize();
            normalVector[0] = vn.x();
            normalVector[1] = vn.y();
            normalVector[2] = vn.z();
            triangle = modelList.at(i)->facetList.at(j)->getTriangle();

            glBegin(GL_TRIANGLES);
            glNormal3fv(normalVector);
            glVertex3f(triangle.at(0).x(),triangle.at(0).y(),triangle.at(0).z());
            glVertex3f(triangle.at(1).x(),triangle.at(1).y(),triangle.at(1).z());
            glVertex3f(triangle.at(2).x(),triangle.at(2).y(),triangle.at(2).z());
            glEnd();

        }
    }

}

void Window::keyPressEvent(QKeyEvent *event)
{
    //Home键调整视角
    if(event->key() == Qt::Key_Home)
    {
        //qDebug("After key_home pressed");
        cam.setCamera(centerInOpenGLWid.x(),centerInOpenGLWid.y()+3*20,centerInOpenGLWid.z()+3*20,
                      centerInOpenGLWid.x(),centerInOpenGLWid.y(),centerInOpenGLWid.z(),
                      0.0,1.0,0.0
                      );
        homeReset();
        update();
    }

}

void Window::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    if (event->button() == Qt::RightButton)
        setCursor(Qt::ClosedHandCursor);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ||
            event->button() == Qt::RightButton)
    {
        unsetCursor();
    }
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)//鼠标左键旋转
    {
        RotateX(dx);
        RotateY(dy);
    }
    else if (event->buttons() & Qt::RightButton)//鼠标右键平移
    {
        cam.slide(-dx/(0.01*width()),dy/(0.01*height()),0);
    }
    lastPos = event->pos();
    update();
}

void Window::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    //滚轮实现缩放
    if(numSteps<0)
    {
        cam.slide(0,0,-5);
    }
    else
    {
        cam.slide(0,0,5);
    }
    update();
}

void Window::slot_openStl_inopengl(QString path)
{
    model->model_load(path);
}

void Window::slot_finishLoadStl()
{
    qDebug("begin to draw..");
    centerInOpenGLWid = model->model_getCenter(model->model_getBox());
    qDebug()<<"Model center is :"<<centerInOpenGLWid.x()<<centerInOpenGLWid.y()<<centerInOpenGLWid.z();

    //20171220新增(调整初始视角)
    modelBox box = model->model_getBox();
    float y_scale = box.y_max - box.y_min;
    float z_scale = box.z_max - box.z_min;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.setCamera(centerInOpenGLWid.x(),centerInOpenGLWid.y()+3*y_scale,centerInOpenGLWid.z()+3*z_scale,
                  centerInOpenGLWid.x(),centerInOpenGLWid.y(),centerInOpenGLWid.z(),
                  0.0,1.0,0.0
                  );
    resizeGL(width(),height());
    update();
}

void Window::slot_iteration()
{
    temp = new StlModel(this);
    temp->stride = this->stride;//将步长传递给模型
    temp->isVariantPace = this->isVariantPace;//将随机步长判断标志传递给模型
    temp->model_iteration(modelList.at(modelList.size()-1));//取modellist中最后一个模型作为参数读入
    //temp->model_print(modelList.size());//调试输出三角形数据到txt
    modelList.append(temp);
    //20171220新增
    resetCamera();//重置摄像机位置
    update();
}

//-----20171219新增--------
void Window::slot_receivePara(int iterNum, bool isVariant, double Stride)
{
    iterationNumber=iterNum;
    isVariantPace=isVariant;
    stride=Stride;
}

void Window::slot_startIter()
{
    for(int i=0; i<iterationNumber; i++)
    {
        slot_iteration();
    }
}

//-----20171220新增--------
void Window::slot_resetModel()
{
    while (modelList.size()>1)
    {
        modelList.removeLast();
    }
    update();
}


void Window::resetCamera()
{
    int n = modelList.size();
    modelBox box;
    float x_off = 0;

    for (int i=0; i<n; i++)
    {
        box = modelList.at(i)->model_getBox();
        x_off = 1.1*(box.x_max - box.x_min);
    }
    x_off = x_off/2;
    cam.slide(x_off,0,0);
    cam.slide(0,0,15);
}

void Window::homeReset()
{
    int n = modelList.size();
    modelBox box;
    float x_off = 0;

    for (int i=0; i<n; i++)
    {
        box = modelList.at(i)->model_getBox();
        x_off = x_off + 1.1*(box.x_max - box.x_min);
    }
    x_off = x_off/2;
    cam.slide(x_off-10,0,0);
    cam.slide(0,0,15*n);
}


