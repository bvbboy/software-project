#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <window.h>
#include <QPushButton>
#include <QFileDialog>
#include <QMenu>
#include <paradialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Window *opengl;
    QMenu *menu_F;
    QMenu *menu_E;
    QAction *settingAction;
    QAction *openAction;
    QAction *quitAction;
    QAction *iterAction;
    QAction *startIterAction;
    paraDialog setting;

    //20171220新增
    QAction *resetAction;

signals:
    void signal_openStl_inopengl(QString path);//发送STL文件路径信号
public slots:
    void slot_openStl();//获取STL文件路径
    void slot_openDialog();//打开参数设置Dialog
};

#endif // MAINWINDOW_H
