#include "grouptimetablewidget.h"

const int GroupTimeTableWidget::DAY_COUNT = 7;

GroupTimeTableWidget::GroupTimeTableWidget(QWidget *parent) :
    QWidget(parent)
{
    for( int i = 0; i < DAY_COUNT; i++)
    {
        DayTimeTableWidget* dayWidget = new DayTimeTableWidget( i, timetable, this);
//                QDate::longDayName( i);
        timetableList_.push_back( dayWidget);
    }
}
