#include "grouptimetablewidget.h"
#include "definitions.h"

#include <QDebug>

const int GroupTimeTableWidget::DAY_COUNT = 7;

GroupTimeTableWidget::GroupTimeTableWidget(TimeTable* timetable,QWidget *parent) :    
    timetable_( timetable),
    QWidget(parent)
{
    titleGroupBox_ = new QGroupBox(this);

    groupNameLineEdit_ = new QLineEdit( titleGroupBox_);
    saveButton_ = new QPushButton( QString::fromUtf8("Сохранить расписание группы"), titleGroupBox_);
    clearButton_ = new QPushButton( QString::fromUtf8("Очистить"), titleGroupBox_);

    manageLayout_ = new QHBoxLayout(titleGroupBox_);
    manageLayout_->addWidget( groupNameLineEdit_);
    manageLayout_->addWidget( saveButton_);
    manageLayout_->addWidget( clearButton_);

    mainLayout_ = new QGridLayout(this);
    mainLayout_->addWidget( titleGroupBox_, 0,0);

    for( int i = 1; i <= DAY_COUNT; i++)
    {
        DayTimeTableWidget* dayWidget = new DayTimeTableWidget( i-1, this);
        timetableList_.push_back( dayWidget);
        mainLayout_->addWidget( dayWidget, i, 0);
    }

    setLayout( mainLayout_);

    setAutoFillBackground(true);
    connect( saveButton_, SIGNAL(clicked()), this, SLOT(saveTask()));
    connect( clearButton_, SIGNAL(clicked()), this, SLOT(clearAll()));
}

void GroupTimeTableWidget::saveTask()
{
    for( TimetableList::iterator it = timetableList_.begin(); it != timetableList_.end(); ++it)
    {
        DayTimeTableWidget::ValueList* list = (*it)->saveTask();
        for( DayTimeTableWidget::ValueList::iterator iter = list->begin(); iter != list->end(); ++iter)
        {
            int weekCount = (*(*iter))[WEEK_KEY].toInt();
            int eventTimeNumber = (*(*iter))[EVENT_TIME_KEY].toInt();
            QString eventName = (*(*iter))[EVENT_NAME_KEY];
            int eventType = (*(*iter))[EVENT_TYPE_KEY].toInt();
            QString location = (*(*iter))[EVENT_LOCATION_KEY];
            QString name = (*(*iter))[NAME_KEY];
            QString lastName = (*(*iter))[LAST_NAME_KEY];
            QString sureName = (*(*iter))[SURE_NAME_KEY];
            qDebug() << "NAME: " << name << lastName << sureName;
//            if( !groupNameLineEdit->text().isEmpty() && !eventNameLineEdit->text().isEmpty() &&
//                    !eventLocationLineEdit->text().isEmpty() && !teacherNameLineEdit->text().isEmpty() && !teacherLastNameLineEdit->text().isEmpty() &&
//                    !teacherSureNameLineEdit->text().isEmpty())
//            {
                // если обе недели выбраны
                if( weekCount == 3)
                {
                    timetable_->addEvent( groupNameLineEdit_->text(), 1, eventTimeNumber, eventName, eventType, location, (*it)->getDayNumber(), name, lastName, sureName);
                    timetable_->addEvent( groupNameLineEdit_->text(), 2, eventTimeNumber, eventName, eventType, location, (*it)->getDayNumber(), name, lastName, sureName);
                }
                else
                    timetable_->addEvent( groupNameLineEdit_->text(), weekCount, eventTimeNumber, eventName, eventType, location, (*it)->getDayNumber(), name, lastName, sureName);
//            }
//            else
//            {
//                QMessageBox::warning( this, QString::fromUtf8("Ошибка записи"), QString::fromUtf8("Заполни все поля, Трекин)"));
//            }
        }
    }
    clearAll();
}

void GroupTimeTableWidget::clearAll()
{
    groupNameLineEdit_->clear();
    for( TimetableList::iterator it = timetableList_.begin(); it != timetableList_.end(); ++it)
    {
        (*it)->clear();
    }
}
