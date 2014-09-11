#include "daytimetablewidget.h"
#include <QLabel>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

const int DayTimeTableWidget::EVENT_COUNT = 7;

DayTimeTableWidget::DayTimeTableWidget( int dayNumber, TimeTable* timetable, QWidget *parent) :
    dayNumber_( dayNumber),
    timetable_( timetable),
    QGroupBox(parent)
{
    tableWidget_ = new QTableWidget( this);
    tableWidget_->setColumnCount(8);
    QStringList headerLabels;
    headerLabels << "Время"<< "Неделя" << "Название"<<"Тип занятия"<<"Место занятия"<<"Фамилия"<<"Имя"<<"Отчество";
    tableWidget_->setHorizontalHeaderLabels( headerLabels);

    nameLabel_ = new QLabel( QDate::longDayName( dayNumber), this);

    addTaskAction_ = new QAction( this);
    addTaskButton_ = new QPushButton( QString::fromUtf8("Добавить пару"), this);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget( nameLabel_);
    mainLayout_->addWidget( addTaskButton_);
    mainLayout_->addWidget( tableWidget_);
setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect( addTaskButton_, SIGNAL(clicked()), this, SLOT(addTask()));
}

void DayTimeTableWidget::addTask()
{
    qDebug() << "ADD TASK";

    int currentRow = tableWidget_->rowCount();
    tableWidget_->insertRow( currentRow);

    QComboBox* weekComboBox;
    QLineEdit* eventNameLineEdit;
    QComboBox* eventTimeComboBox;
    QComboBox* eventTypeComboBox;
    QLineEdit* eventLocationLineEdit;
    QLineEdit* teacherNameLineEdit;
    QLineEdit* teacherLastNameLineEdit;
    QLineEdit* teacherSureNameLineEdit;

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

    teacherNameLineEdit = new QLineEdit( this);
    teacherLastNameLineEdit = new QLineEdit( this);
    teacherSureNameLineEdit = new QLineEdit( this);

    tableWidget_->setCellWidget( currentRow,0,eventTimeComboBox);
    tableWidget_->setCellWidget( currentRow,1,weekComboBox);
    tableWidget_->setCellWidget( currentRow,2,eventNameLineEdit);
    tableWidget_->setCellWidget( currentRow,3,eventTypeComboBox);
    tableWidget_->setCellWidget( currentRow,4,eventLocationLineEdit);
    tableWidget_->setCellWidget( currentRow,5,teacherLastNameLineEdit);
    tableWidget_->setCellWidget( currentRow,6,teacherNameLineEdit);
    tableWidget_->setCellWidget( currentRow,7,teacherSureNameLineEdit);
    tableWidget_->resizeRowsToContents();
//    this->resize( tableWidget_->width(), tableWidget_->height());
    tableWidget_->resizeColumnsToContents();
}

void DayTimeTableWidget::saveTask()
{
    qDebug() << "Saving data in database";
    for( int i = 0; i < tableWidget_->rowCount(); i++)
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

        for( int j = 0; j < tableWidget_->columnCount(); j++)
        {
            tableWidget_->itemAt( i, j)->text();
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
