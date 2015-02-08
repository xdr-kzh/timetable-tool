#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QScrollArea>

#include "grouptimetablewidget.h"

class TimeTable;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void chooseImportFile();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    GroupTimeTableWidget* groupWidget_;
    QMenu* menu_;
    QAction* saveAction_;
    QAction* clearAction_;
    QAction* importAction_;
    QScrollArea *scrollArea_;

    TimeTable* timetable_;
};

#endif // MAINWINDOW_H
