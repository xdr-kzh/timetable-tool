#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>

#include "grouptimetablewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    GroupTimeTableWidget* groupWidget_;
    QMenu* menu_;
    QAction* saveAction_;

    TimeTable* timetable_;
};

#endif // MAINWINDOW_H
