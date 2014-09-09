#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>

#include "timetable.h"

class DayTimeTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit DayTimeTableWidget( int dayNumber, TimeTable* timetable, QWidget *parent = 0);

signals:


public slots:
    void saveTask();

private:
    void addTask();

private:
    TimeTable* timetable_;

    static const int EVENT_COUNT;
    int dayNumber_;
//    QLineEdit* groupNameLineEdit;
//    QComboBox* weekComboBox;
//    QLineEdit* eventNameLineEdit;
//    QComboBox* eventTimeComboBox;
//    QComboBox* eventTypeComboBox;
//    QLineEdit* eventLocationLineEdit;
//    QComboBox* dayComboBox;
//    QLineEdit* teacherNameLineEdit;
//    QLineEdit* teacherLastNameLineEdit;
//    QLineEdit* teacherSureNameLineEdit;
};

#endif // ADDTASKWIDGET_H
