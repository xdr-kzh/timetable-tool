#include "mainwindow.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize( 800, 450);

    tabWidget_ = new TabWidget( this);

    menu_ = new QMenu( QString::fromUtf8("Файл"), this);

    newGroupAction_ = new QAction( QString::fromUtf8("Добавить группу"), this);
    menu_->addAction( newGroupAction_);

    menuBar()->addMenu( menu_);

    setCentralWidget( tabWidget_);
}

MainWindow::~MainWindow()
{}
