#ifndef GROUPTIMETABLEWIDGET_H
#define GROUPTIMETABLEWIDGET_H

#include <QWidget>
#include <list>

#include "daytimetablewidget.h"

class GroupTimeTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupTimeTableWidget(QWidget *parent = 0);

signals:

public slots:

private:
    static const int DAY_COUNT;
    std::list<DayTimeTableWidget*> timetableList_;
};

#endif // GROUPTIMETABLEWIDGET_H
