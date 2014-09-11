#include "grouptimetablewidget.h"

const int GroupTimeTableWidget::DAY_COUNT = 7;

GroupTimeTableWidget::GroupTimeTableWidget(TimeTable* timetable,QWidget *parent) :
    QWidget(parent)
{
    groupNameLineEdit_ = new QLineEdit( this);
    mainLayout_ = new QGridLayout(this);
    mainLayout_->addWidget( groupNameLineEdit_, 0,0);
    for( int i = 1; i <= DAY_COUNT; i++)
    {
        DayTimeTableWidget* dayWidget = new DayTimeTableWidget( i, timetable, this);
        timetableList_.push_back( dayWidget);
        mainLayout_->addWidget( dayWidget, i, 0);
//        break;
    }
    setLayout( mainLayout_);

    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
}
