#include "model.h"

StlModel::StlModel(QObject *parent) : QObject(parent)
{
    model_clear();
    //20171219新增
    stride=0.1;
    isVariantPace=false;
}

void StlModel::model_load(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString header = file.readLine();

        if(header.trimmed().startsWith("solid"))
        {
            qDebug("start to read text..");

            //facet类数据结构
            facet *tempFacet;
            QVector3D vn;
            QVector<QVector3D> triangle;

            QVector3D point1;
            QVector3D point2;
            QVector3D point3;
            QVector3D midp12;
            QVector3D midp23;
            QVector3D midp31;
            float length12;
            float length23;
            float length31;
            QVector3D length;

            int count = 0;
            QString row;
            row = file.readLine();
            //读取STL文件每一行，直到endsolid结束
            while(!row.trimmed().startsWith("endsolid"))
            {
                vn = getCoord(row);//法向量
                row = file.readLine();
                row = file.readLine().trimmed();
                point1 = getCoord(row);//第一个点
                row = file.readLine().trimmed();
                point2 = getCoord(row);//第二个点
                row = file.readLine().trimmed();
                point3 = getCoord(row);//第三个点
                row = file.readLine();
                row = file.readLine();
                row = file.readLine();
                //计算三边中点
                midp12 = (point1 + point2)/2;
                midp23 = (point2 + point3)/2;
                midp31 = (point3 + point1)/2;
                //计算三边长度
                length12 = getLength(point1,point2);
                length23 = getLength(point2,point3);
                length31 = getLength(point3,point1);
                length = QVector3D(length12,length23,length31);
                //数据加到triangle中
                triangle.clear();
                triangle.append(point1);
                triangle.append(point2);
                triangle.append(point3);
                triangle.append(midp12);
                triangle.append(midp23);
                triangle.append(midp31);
                triangle.append(length);
                //更新模型边界
                model_updateBox(point1);
                model_updateBox(point2);
                model_updateBox(point3);

                //vn和triangle加到facet中
                tempFacet = new facet;
                tempFacet->setNormalVector(vn);
                tempFacet->setTriangle(triangle);
                facetList.append(tempFacet);//每个facet组成一个list
                count++;
            }
            file.close();
            qDebug("loading finished");
            qDebug()<<"number of triangle: "<<count;
            emit signal_loaded();
        }
    }
    else
    {
        QMessageBox::information(0,tr("Error"),tr("Failed to open the STL file"));//打开错误文件报错
    }
}

void StlModel::model_clear()
{
    //清除模型中的数据
    facetList.clear();

    box.x_min = 0;
    box.x_max = 0;
    box.y_min = 0;
    box.y_max = 0;
    box.z_min = 0;
    box.z_max = 0;

}

modelBox StlModel::model_getBox()
{
    return box;
}

QVector3D StlModel::model_getCenter(modelBox box)
{
    QVector3D model_center;
    model_center.setX((box.x_max+box.x_min)/2);
    model_center.setY((box.y_max+box.y_min)/2);
    model_center.setZ((box.z_max+box.z_min)/2);
    return model_center;
}

void StlModel::model_updateBox(QVector3D point)
{
    QVector3D newpoint = point;
    if (newpoint.x()<box.x_min)
        box.x_min = newpoint.x();
    else if (newpoint.x()>box.x_max)
        box.x_max = newpoint.x();
    if (newpoint.y()<box.y_min)
        box.y_min = newpoint.y();
    else if (newpoint.y()>box.y_max)
        box.y_max = newpoint.y();
    if (newpoint.z()<box.z_min)
        box.z_min = newpoint.z();
    else if (newpoint.z()>box.z_max)
        box.z_max = newpoint.z();

}

int StlModel::model_facetNum()
{
    return facetList.size();
}

void StlModel::model_iteration(StlModel* oldmodel)
{
    qDebug("after iteration");
    if(isVariantPace)//20171219新增
    {
        stride = rand()%50/100.0;//步长设置为0.00到0.50随机数
    }
    int count = 0;
    //oldmodel中的数据
    QVector<QVector3D> triangle;
    QVector3D point1;
    QVector3D point2;
    QVector3D point3;
    QVector3D midp12;
    QVector3D midp23;
    QVector3D midp31;
    QVector3D length;
    float length12;
    float length23;
    float length31;
    QVector3D model_center = oldmodel->model_getCenter(oldmodel->box);

    //newmodel数据
    QVector3D newvn;
    QVector<QVector3D> newtriangle;
    facet *tempFacet1;
    facet *tempFacet2;
    facet *tempFacet3;
    facet *tempFacet4;
    QVector3D newp12;
    QVector3D newp23;
    QVector3D newp31;
    QVector3D midnewp1;
    QVector3D midnewp2;
    QVector3D midnewp3;
    QVector3D newLength;
    float len12;
    float len23;
    float len31;

    //取oldmodel每一个面进行迭代操作
    for (int i = 0; i < oldmodel->facetList.size(); i++)
    {
        //读取triangle数据
        triangle = oldmodel->facetList.at(i)->getTriangle();
        point1 = triangle.at(0);
        point2 = triangle.at(1);
        point3 = triangle.at(2);
        midp12 = triangle.at(3);
        midp23 = triangle.at(4);
        midp31 = triangle.at(5);
        length = triangle.at(6);
        length12 = length.x();
        length23 = length.y();
        length31 = length.z();

        //计算新中点,20171219新增
        newp12 = midp12 + stride * length12 * (midp12 - model_center) / getLength(midp12,model_center);
        newp23 = midp23 + stride * length23 * (midp23 - model_center) / getLength(midp23,model_center);
        newp31 = midp31 + stride * length31 * (midp31 - model_center) / getLength(midp31,model_center);
        //更新包围盒
        model_updateBox(newp12);
        model_updateBox(newp23);
        model_updateBox(newp31);
        //第一个三角形
        newtriangle.clear();
        midnewp1 = (newp12 + newp23) / 2;
        midnewp2 = (newp23 + newp31) / 2;
        midnewp3 = (newp31 + newp12) / 2;
        len12 = getLength(newp12,newp23);
        len23 = getLength(newp23,newp31);
        len31 = getLength(newp31,newp12);
        newLength = QVector3D(len12,len23,len31);
        newtriangle.append(newp12);
        newtriangle.append(newp23);
        newtriangle.append(newp31);
        newtriangle.append(midnewp1);
        newtriangle.append(midnewp2);
        newtriangle.append(midnewp3);
        newtriangle.append(newLength);
        newvn = QVector3D::crossProduct(newp23-newp12,newp31-newp23);
        tempFacet1 = new facet;
        tempFacet1->setNormalVector(newvn);
        tempFacet1->setTriangle(newtriangle);
        facetList.append(tempFacet1);
        count++;

        //第二个三角形
        newtriangle.clear();
        midnewp1 = (point1 + newp12) / 2;
        midnewp2 = (newp12 + newp31) / 2;
        midnewp3 = (newp31 + point1) / 2;
        len12 = getLength(point1,newp12);
        len23 = getLength(newp12,newp31);
        len31 = getLength(newp31,point1);
        newLength = QVector3D(len12,len23,len31);
        newtriangle.append(point1);
        newtriangle.append(newp12);
        newtriangle.append(newp31);
        newtriangle.append(midnewp1);
        newtriangle.append(midnewp2);
        newtriangle.append(midnewp3);
        newtriangle.append(newLength);
        newvn = QVector3D::crossProduct(newp12-point1,newp31-newp12);
        tempFacet2 = new facet;
        tempFacet2->setNormalVector(newvn);
        tempFacet2->setTriangle(newtriangle);
        facetList.append(tempFacet2);
        count++;

        //第三个三角形
        newtriangle.clear();
        midnewp1 = (point2 + newp23) / 2;
        midnewp2 = (newp23 + newp12) / 2;
        midnewp3 = (newp12 + point2) / 2;
        len12 = getLength(point2,newp23);
        len23 = getLength(newp23,newp12);
        len31 = getLength(newp12,point2);
        newLength = QVector3D(len12,len23,len31);
        newtriangle.append(point2);
        newtriangle.append(newp23);
        newtriangle.append(newp12);
        newtriangle.append(midnewp1);
        newtriangle.append(midnewp2);
        newtriangle.append(midnewp3);
        newtriangle.append(newLength);
        newvn = QVector3D::crossProduct(newp23-point2,newp12-newp23);
        tempFacet3 = new facet;
        tempFacet3->setNormalVector(newvn);
        tempFacet3->setTriangle(newtriangle);
        facetList.append(tempFacet3);
        count++;

        //第四个三角形
        newtriangle.clear();
        midnewp1 = (point3 + newp31) / 2;
        midnewp2 = (newp31 + newp23) / 2;
        midnewp3 = (newp23 + point3) / 2;
        len12 = getLength(point3,newp31);
        len23 = getLength(newp31,newp23);
        len31 = getLength(newp23,point3);
        newLength = QVector3D(len12,len23,len31);
        newtriangle.append(point3);
        newtriangle.append(newp31);
        newtriangle.append(newp23);
        newtriangle.append(midnewp1);
        newtriangle.append(midnewp2);
        newtriangle.append(midnewp3);
        newtriangle.append(newLength);
        newvn = QVector3D::crossProduct(newp31-point3,newp23-newp31);
        tempFacet4 = new facet;
        tempFacet4->setNormalVector(newvn);
        tempFacet4->setTriangle(newtriangle);
        facetList.append(tempFacet4);
        count++;
    }
    qDebug()<<count;
}

void StlModel::model_print(int i)
{
    //新建txt文件
    QString s = QString::number(i);
    QFile data(QString("triangle"+s+".txt"));

    QVector3D Vn;
    QVector<QVector3D> triangle;

    QVector3D Vertex1;
    QVector3D Vertex2;
    QVector3D Vertex3;
    if(data.open(QFile::WriteOnly))
    {
        //写入每一个facet数据
        for(int i=0; i<facetList.size(); i++)
        {
            QTextStream out(&data);
            Vn = facetList.at(i)->getNormalVector();
            triangle = facetList.at(i)->getTriangle();
            Vertex1 = triangle.at(0);
            Vertex2 = triangle.at(1);
            Vertex3 = triangle.at(2);

            out<<"triangle no."<<i<<endl;
            out<<Vn.x()<<","<<Vn.y()<<","<<Vn.z()<<endl;
            out<<Vertex1.x()<<","<<Vertex1.y()<<","<<Vertex1.z()<<endl;
            out<<Vertex2.x()<<","<<Vertex2.y()<<","<<Vertex2.z()<<endl;
            out<<Vertex3.x()<<","<<Vertex3.y()<<","<<Vertex3.z()<<endl;
            out<<endl;
        }
    }
    data.close();
}

QVector3D StlModel::getCoord(QString row)
{
    QStringList strList;
    QVector3D coord;
    QVector<float> temp;

    strList = row.split(" ");
    for(int i=strList.size()-3; i<strList.size(); i++)
    {
        temp.append(strList[i].toFloat());
    }
    coord.setX(temp[0]);
    coord.setY(temp[1]);
    coord.setZ(temp[2]);
    return coord;
}

float StlModel::getLength(QVector3D p1, QVector3D p2)
{
    float lx = p1.x() - p2.x();
    float ly = p1.y() - p2.y();
    float lz = p1.z() - p2.z();
    float length = pow(lx,2) + pow(ly,2) + pow(lz,2);
    return pow(length,0.5);
}





