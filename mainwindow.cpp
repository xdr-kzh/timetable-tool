#include "mainwindow.h"

#include <QMenuBar>
#include <QKeySequence>
#include "timetable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize( 1024, 400);

    timetable_ = new TimeTable( "ContactBookV2.db",this);
    addTaskWidget_ = new AddTaskWidget( timetable_, this);

    menu_ = new QMenu( QString::fromUtf8("Файл"), this);

    QKeySequence keySequence = Qt::CTRL + Qt::Key_S;
    saveAction_ = new QAction( QString::fromUtf8("Сохранить занятие"), this);
    saveAction_->setShortcut( keySequence);
    menu_->addAction( saveAction_);

    menuBar()->addMenu( menu_);

    setCentralWidget( addTaskWidget_);

    connect( saveAction_, SIGNAL(triggered()), addTaskWidget_, SLOT(saveTask()));
}

MainWindow::~MainWindow()
{
}
