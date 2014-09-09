#include "daytimetablewidget.h"
#include <QLabel>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

const int DayTimeTableWidget::EVENT_COUNT = 7;

DayTimeTableWidget::AddTaskWidget( int dayNumber, TimeTable* timetable, QWidget *parent) :
    dayNumber_( dayNumber),
    timetable_( timetable),
    QTableWidget(parent)
{
    setColumnCount(8);
    QStringList headerLabels;
    headerLabels << "Время"<< "Неделя" << "Название"<<"Тип занятия"<<"Место занятия"<<"Фамилия"<<"Имя"<<"Отчество";
    setHorizontalHeaderLabels( headerLabels);

    addTask();
    addTask();
    resizeColumnsToContents();
}

void DayTimeTableWidget::addTask()
{
    int currentRow = rowCount();
    insertRow( rowCount());
//    QLineEdit* groupNameLineEdit;
    QComboBox* weekComboBox;
    QLineEdit* eventNameLineEdit;
    QComboBox* eventTimeComboBox;
    QComboBox* eventTypeComboBox;
    QLineEdit* eventLocationLineEdit;
//    QComboBox* dayComboBox;
    QLineEdit* teacherNameLineEdit;
    QLineEdit* teacherLastNameLineEdit;
    QLineEdit* teacherSureNameLineEdit;

    groupNameLineEdit = new QLineEdit( this);

    weekComboBox = new QComboBox( this);
    QStringList weekList;
    weekList << "1" << "2" << "Все";
    weekComboBox->addItems( weekList);

    eventNameLineEdit = new QLineEdit( this);

    eventTimeComboBox = new QComboBox( this);
    QStringList timeList;
    for( int i = 1; i <= EVENT_COUNT; i++)
        timeList << QString::number( i);
    eventTimeComboBox->addItems( timeList);

    eventTypeComboBox = new QComboBox( this);
    QStringList typeList;
    typeList << "Лекция" << "Практика" << "Семинар";
    eventTypeComboBox->addItems( typeList);

    eventLocationLineEdit = new QLineEdit( this);

//    dayComboBox = new QComboBox( this);
//    QStringList dayList;
//    for( int i = 1; i <= DAY_COUNT; i++)
//        dayList << QDate::longDayName( i);
//    dayComboBox->addItems( dayList);

    teacherNameLineEdit = new QLineEdit( this);
    teacherLastNameLineEdit = new QLineEdit( this);
    teacherSureNameLineEdit = new QLineEdit( this);

//    setCellWidget( currentRow,0,groupNameLineEdit);
//    setCellWidget( currentRow,1,dayComboBox);
    setCellWidget( currentRow,0,eventTimeComboBox);
    setCellWidget( currentRow,1,weekComboBox);
    setCellWidget( currentRow,2,eventNameLineEdit);
    setCellWidget( currentRow,3,eventTypeComboBox);
    setCellWidget( currentRow,4,eventLocationLineEdit);
    setCellWidget( currentRow,5,teacherLastNameLineEdit);
    setCellWidget( currentRow,6,teacherNameLineEdit);
    setCellWidget( currentRow,7,teacherSureNameLineEdit);
}

void DayTimeTableWidget::saveTask()
{
    qDebug() << "Saving data in database";
    for( int i = 0; i < rowCount(); i++)
    {
        QString groupName;
        int week;
        int eventTimeNumber;
        QString eventName;
        int eventType;
        QString location;
        int day;
        QString teacherName;
        QString teacherLastName;
        QString teacherSureName;

        for( int j = 0; j < columnCount(); j++)
        {
            itemAt( i, j)->text();
        }
    }

//    if( !groupNameLineEdit->text().isEmpty() && !eventNameLineEdit->text().isEmpty() &&
//            !eventLocationLineEdit->text().isEmpty() && !teacherNameLineEdit->text().isEmpty() && !teacherLastNameLineEdit->text().isEmpty() &&
//            !teacherSureNameLineEdit->text().isEmpty())
//    {
//        // если обе недели выбраны
//        if( weekComboBox->currentIndex() == 2)
//        {
//            timetable_->addEvent( groupNameLineEdit->text(), 1, eventTimeComboBox->currentIndex()+1,
//                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
//                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
//            timetable_->addEvent( groupNameLineEdit->text(), 2, eventTimeComboBox->currentIndex()+1,
//                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
//                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
//        }
//        else
//            timetable_->addEvent( groupNameLineEdit->text(), weekComboBox->currentIndex()+1, eventTimeComboBox->currentIndex()+1,
//                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
//                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
//    }
//    else
//    {
//        QMessageBox::warning( this, QString::fromUtf8("Ошибка записи"), QString::fromUtf8("Заполни все поля, Трекин)"));
//    }

}
