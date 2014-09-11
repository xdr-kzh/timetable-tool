#ifndef GROUPTIMETABLEWIDGET_H
#define GROUPTIMETABLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <list>

#include "daytimetablewidget.h"

class GroupTimeTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupTimeTableWidget(TimeTable* timetable,QWidget *parent = 0);

private:
    QGridLayout* mainLayout_;
    QLineEdit* groupNameLineEdit_;

    static const int DAY_COUNT;
    std::list<DayTimeTableWidget*> timetableList_;
};

#endif // GROUPTIMETABLEWIDGET_H
