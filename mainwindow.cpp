#include "mainwindow.h"

#include <QMenuBar>
#include <QKeySequence>
#include <QScrollArea>
#include "timetable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Infovuz Timetable Tool V. 0.1");

    timetable_ = new TimeTable( "ContactBookV2.db",this);
    groupWidget_ = new GroupTimeTableWidget( timetable_, this);

    menu_ = new QMenu( QString::fromUtf8("Файл"), this);

    QKeySequence keySequence = Qt::CTRL + Qt::Key_S;
    saveAction_ = new QAction( QString::fromUtf8("Сохранить занятие"), this);
    saveAction_->setShortcut( keySequence);
    menu_->addAction( saveAction_);

    menuBar()->addMenu( menu_);

    setCentralWidget( groupWidget_);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget( groupWidget_);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumSize( 1100, 400);

//    connect( saveAction_, SIGNAL(triggered()), gr, SLOT(saveTask()));
}

MainWindow::~MainWindow()
{
}
