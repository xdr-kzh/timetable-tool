#include "addtaskwidget.h"
#include <QLabel>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

const int AddTaskWidget::EVENT_COUNT = 7;
const int AddTaskWidget::DAY_COUNT = 7;

AddTaskWidget::AddTaskWidget(TimeTable* timetable,QWidget *parent) :
    timetable_( timetable),
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(10);

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

    dayComboBox = new QComboBox( this);
    QStringList dayList;
    for( int i = 1; i <= DAY_COUNT; i++)
        dayList << QDate::longDayName( i);
    dayComboBox->addItems( dayList);

    teacherNameLineEdit = new QLineEdit( this);
    teacherLastNameLineEdit = new QLineEdit( this);
    teacherSureNameLineEdit = new QLineEdit( this);

    setCellWidget( 0,0,groupNameLineEdit);
    setCellWidget( 0,1,weekComboBox);
    setCellWidget( 0,2,eventNameLineEdit);
    setCellWidget( 0,3,eventTimeComboBox);
    setCellWidget( 0,4,eventTypeComboBox);
    setCellWidget( 0,5,eventLocationLineEdit);
    setCellWidget( 0,6,dayComboBox);
    setCellWidget( 0,7,teacherLastNameLineEdit);
    setCellWidget( 0,8,teacherNameLineEdit);
    setCellWidget( 0,9,teacherSureNameLineEdit);

    QStringList headerLabels;
    headerLabels << "Группа" << "Неделя" << "Название"<<"Время"<<"Тип занятия"<<"Место занятия"<<"День"<<"Фамилия"<<"Имя"<<"Отчество";
    setHorizontalHeaderLabels( headerLabels);
}

void AddTaskWidget::saveTask()
{
    qDebug() << "Saving data in database";
    if( !groupNameLineEdit->text().isEmpty() && !eventNameLineEdit->text().isEmpty() &&
            !eventLocationLineEdit->text().isEmpty() && !teacherNameLineEdit->text().isEmpty() && !teacherLastNameLineEdit->text().isEmpty() &&
            !teacherSureNameLineEdit->text().isEmpty())
    {
        // если обе недели выбраны
        if( weekComboBox->currentIndex() == 2)
        {
            timetable_->addEvent( groupNameLineEdit->text(), 1, eventTimeComboBox->currentIndex()+1,
                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
            timetable_->addEvent( groupNameLineEdit->text(), 2, eventTimeComboBox->currentIndex()+1,
                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
        }
        else
            timetable_->addEvent( groupNameLineEdit->text(), weekComboBox->currentIndex()+1, eventTimeComboBox->currentIndex()+1,
                eventNameLineEdit->text(), eventTypeComboBox->currentIndex()+1, eventLocationLineEdit->text(), dayComboBox->currentIndex(),
                    teacherNameLineEdit->text(), teacherLastNameLineEdit->text(), teacherSureNameLineEdit->text());
    }
    else
    {
        QMessageBox::warning( this, QString::fromUtf8("Ошибка записи"), QString::fromUtf8("Заполни все поля, Трекин)"));
    }

}
