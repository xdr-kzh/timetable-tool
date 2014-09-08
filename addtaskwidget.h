#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>

#include "timetable.h"

class AddTaskWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit AddTaskWidget( TimeTable* timetable, QWidget *parent = 0);

signals:


public slots:
    void saveTask();

private:
    TimeTable* timetable_;

    static const int EVENT_COUNT;
    static const int DAY_COUNT;

    QLineEdit* groupNameLineEdit;
    QComboBox* weekComboBox;
    QLineEdit* eventNameLineEdit;
    QComboBox* eventTimeComboBox;
    QComboBox* eventTypeComboBox;
    QLineEdit* eventLocationLineEdit;
    QComboBox* dayComboBox;
    QLineEdit* teacherNameLineEdit;
    QLineEdit* teacherLastNameLineEdit;
    QLineEdit* teacherSureNameLineEdit;
};

#endif // ADDTASKWIDGET_H
