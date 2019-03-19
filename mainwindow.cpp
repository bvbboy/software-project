#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("中点迭代器");
    setWindowIcon(QIcon(":/new/prefix1/good.png"));
    opengl = new Window(this);
    setCentralWidget(opengl);

    menu_F = new QMenu(tr("File"));
    ui->menuBar->addMenu(menu_F);
    openAction = new QAction(tr("&Open"),this);
    menu_F->addAction(openAction);
    quitAction = new QAction(tr("&Quit"),this);
    menu_F->addAction(quitAction);

    menu_E = new QMenu(tr("Edit"));
    ui->menuBar->addMenu(menu_E);

    settingAction = new QAction(tr("&set parameters"),this);
    menu_E->addAction(settingAction);
    settingAction->setEnabled(false);
    iterAction = new QAction(tr("&Iterator(one step)"),this);
    menu_E->addAction(iterAction);
    iterAction->setEnabled(false);
    startIterAction = new QAction(tr("&Iterator"),this);
    menu_E->addAction(startIterAction);
    startIterAction->setEnabled(false);
    //20171120新增(重置模型)
    resetAction = new QAction(tr("&Reset"),this);
    menu_E->addAction(resetAction);
    resetAction->setEnabled(false);

    connect(quitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(slot_openStl()));
    connect(this,SIGNAL(signal_openStl_inopengl(QString)),opengl,SLOT(slot_openStl_inopengl(QString)));
    connect(iterAction,SIGNAL(triggered(bool)),opengl,SLOT(slot_iteration()));
    //20171219新增
    connect(startIterAction,SIGNAL(triggered(bool)),opengl,SLOT(slot_startIter()));
    connect(settingAction,SIGNAL(triggered(bool)),this,SLOT(slot_openDialog()));
    connect(&setting,SIGNAL(settingDoneW(int,bool, double)),opengl,SLOT(slot_receivePara(int,bool,double)));
    //20171120新增
    connect(resetAction,SIGNAL(triggered(bool)),opengl,SLOT(slot_resetModel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_openStl()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,"open file dialog","/","STL files (*.stl)");

    if(!path.isEmpty())
    {
        emit signal_openStl_inopengl(path);
        settingAction->setEnabled(true);
        resetAction->setEnabled(true);
    }
}
//20171219新增
void MainWindow::slot_openDialog()
{
    setting.show();
    iterAction->setEnabled(true);
    startIterAction->setEnabled(true);
}
