#include "mainwindow.h"

#include <QMenuBar>
#include "timetable.h"

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

    TimeTable timeTable( "D:\\workspace\\timetable\\ContactBookV2.db",this);
    timeTable.addEvent( "'10ВП2'", 1, 1, "'ФК'", 1, "9-503", 0, "Николь", "Николь", "Николь");
}

MainWindow::~MainWindow()
{}
