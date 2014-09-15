#ifndef GROUPTIMETABLEWIDGET_H
#define GROUPTIMETABLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <list>

#include "daytimetablewidget.h"
#include "timetable.h"

class GroupTimeTableWidget : public QWidget
{
typedef std::list<DayTimeTableWidget*> TimetableList;
    Q_OBJECT
public:
    explicit GroupTimeTableWidget(TimeTable* timetable,QWidget *parent = 0);

public slots:
    void saveTask();
    void clearAll();

private:
    QGridLayout* mainLayout_;
    QLineEdit* groupNameLineEdit_;
    QPushButton* clearButton_;
    QPushButton* saveButton_;

    QHBoxLayout* manageLayout_;
    QGroupBox* titleGroupBox_;

    TimeTable* timetable_;

    static const int DAY_COUNT;
    TimetableList timetableList_;
};

#endif // GROUPTIMETABLEWIDGET_H
