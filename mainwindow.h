#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QScrollArea>

#include "grouptimetablewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    GroupTimeTableWidget* groupWidget_;
    QVBoxLayout* layout_;
    QMenu* menu_;
    QAction* saveAction_;
    QAction* clearAction_;
    QScrollArea *scrollArea_;

    TimeTable* timetable_;
};

#endif // MAINWINDOW_H
