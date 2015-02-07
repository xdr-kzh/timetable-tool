#include "mainwindow.h"

#include <QMenuBar>
#include <QKeySequence>

#include "timetable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("InfoVUZ Timetable Tool v0.2.2");

    timetable_ = new TimeTable( "ContactBookV2.db",this);
    groupWidget_ = new GroupTimeTableWidget( timetable_, this);

    menu_ = new QMenu( QString::fromUtf8("Файл"), this);

    QKeySequence saveKeySequence = Qt::CTRL + Qt::Key_S;
    QKeySequence clearKeySequence = Qt::CTRL + Qt::Key_Q;
    saveAction_ = new QAction( QString::fromUtf8("Сохранить занятие"), this);
    clearAction_ = new QAction( QString::fromUtf8("Очистить"), this);
    saveAction_->setShortcut( saveKeySequence);
    clearAction_->setShortcut( clearKeySequence);
    menu_->addAction( saveAction_);
    menu_->addAction( clearAction_);

    menuBar()->addMenu( menu_);


    scrollArea_ = new QScrollArea();
    scrollArea_->setWidget( groupWidget_);
    scrollArea_->setWidgetResizable(true);

    setCentralWidget( scrollArea_);
    resize( 1200, 800);

    scrollArea_->setMinimumSize( size());

    connect( saveAction_, SIGNAL(triggered()), groupWidget_, SLOT(saveTask()));
    connect( clearAction_, SIGNAL(triggered()), groupWidget_, SLOT(clearAll()));
}

MainWindow::~MainWindow()
{}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
}
