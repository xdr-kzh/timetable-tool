#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>

#include "timetable.h"

class DayTimeTableWidget : public QGroupBox//QTableWidget
{
    Q_OBJECT
public:
    explicit DayTimeTableWidget( int dayNumber, TimeTable* timetable, QWidget *parent = 0);

public slots:
    void saveTask();

private slots:
    void addTask();

private:
    TimeTable* timetable_;
    QPushButton* addTaskButton_;
    QLabel* nameLabel_;
    QTableWidget* tableWidget_;
    QVBoxLayout* mainLayout_;

    QAction* addTaskAction_;

    static const int EVENT_COUNT;
    int dayNumber_;
};

#endif // ADDTASKWIDGET_H
