#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>

#include "tabwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    TabWidget* tabWidget_;
    QMenu* menu_;
    QAction* newGroupAction_;
};

#endif // MAINWINDOW_H
