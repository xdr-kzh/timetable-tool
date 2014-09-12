#include "mainwindow.h"

#include <QMenuBar>
#include <QKeySequence>
#include "timetable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    layout_ = new QVBoxLayout(this);
    setWindowTitle("Infovuz Timetable Tool v0.2");

    timetable_ = new TimeTable( "ContactBookV2.db",this);
    groupWidget_ = new GroupTimeTableWidget( timetable_, this);

    menu_ = new QMenu( QString::fromUtf8("Файл"), this);

    QKeySequence keySequence = Qt::CTRL + Qt::Key_S;
    saveAction_ = new QAction( QString::fromUtf8("Сохранить занятие"), this);
    saveAction_->setShortcut( keySequence);
    menu_->addAction( saveAction_);

    menuBar()->addMenu( menu_);

    setCentralWidget( groupWidget_);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setWidget( groupWidget_);
    scrollArea_->setWidgetResizable(true);

    resize( 1200, 800);

    scrollArea_->setMinimumSize( size());

    connect( saveAction_, SIGNAL(triggered()), groupWidget_, SLOT(saveTask()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    scrollArea_->setMinimumSize( size());
}
