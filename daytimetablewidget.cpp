#include "daytimetablewidget.h"
#include <QLabel>
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QObject>
#include "definitions.h"

const int DayTimeTableWidget::EVENT_COUNT = 8;
const int DayTimeTableWidget::MILITARY_EVENT_NUMBER = 8;
const int DayTimeTableWidget::MINIMUM_HEIGHT = 260;

DayTimeTableWidget::DayTimeTableWidget( int dayNumber,QWidget *parent) :
    dayNumber_( dayNumber),
    QGroupBox(parent)
{
    tableWidget_ = new QTableWidget( this);
    tableWidget_->setColumnCount(8);
    QStringList headerLabels;
    headerLabels << "Время"<< "Неделя" << "Название"<<"Тип занятия"<<"Место занятия"<<"Фамилия"<<"Имя"<<"Отчество";
    tableWidget_->setHorizontalHeaderLabels( headerLabels);

    nameLabel_ = new QLabel( QDate::longDayName( dayNumber+1), this);

    addTaskButton_ = new QPushButton( QString::fromUtf8("Добавить пару"), this);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget( nameLabel_);
    mainLayout_->addWidget( addTaskButton_);
    mainLayout_->addWidget( tableWidget_);
    connect( addTaskButton_, SIGNAL(clicked()), this, SLOT(addTask()));
    setAutoFillBackground(true);

    setMinimumHeight( MINIMUM_HEIGHT);
}

void DayTimeTableWidget::addTask()
{
    qDebug() << "ADD TASK";

    int currentRow = tableWidget_->rowCount();
    tableWidget_->insertRow( currentRow);

    QComboBox* eventTimeComboBox;
    QComboBox* weekComboBox;
    QLineEdit* eventNameLineEdit;
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
    {
        if( i == MILITARY_EVENT_NUMBER)
            timeList << QString::fromUtf8("Военная подготовка");
        else
            timeList << QString::number( i);
    }
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
    tableWidget_->resizeColumnsToContents();
}

DayTimeTableWidget::ValueList* DayTimeTableWidget::saveTask()
{
    qDebug() << "Saving data in database";
    for( int i = 0; i < tableWidget_->rowCount(); i++)
    {
        ValueMapPtr valueMap = ValueMapPtr( new ValueMap);

        for( int j = 0; j < tableWidget_->columnCount(); j++)
        {
            QWidget* widget = tableWidget_->cellWidget( i, j);
            QComboBox* combo;
            QLineEdit* lineEdit;
            switch( j)
            {
            case 0:
                combo = qobject_cast<QComboBox*>(widget);
                qDebug() << QString::number(combo->currentIndex()+1);
                valueMap->insert( EVENT_TIME_KEY, QString::number(combo->currentIndex()+1));
                break;
            case 1:
                combo = qobject_cast<QComboBox*>(widget);
                qDebug() << QString::number(combo->currentIndex()+1);
                valueMap->insert( WEEK_KEY, QString::number(combo->currentIndex()+1));
                break;
            case 2:
                lineEdit = qobject_cast<QLineEdit*>(widget);
                qDebug() << lineEdit->text();
                valueMap->insert( EVENT_NAME_KEY, lineEdit->text());
                break;
            case 3:
                combo = qobject_cast<QComboBox*>(widget);
                qDebug() << QString::number(combo->currentIndex()+1);
                valueMap->insert( EVENT_TYPE_KEY, QString::number(combo->currentIndex()+1));
                break;
            case 4:
                lineEdit = qobject_cast<QLineEdit*>(widget);
                qDebug() << lineEdit->text();
                valueMap->insert( EVENT_LOCATION_KEY, lineEdit->text());
                break;
            case 5:
                lineEdit = qobject_cast<QLineEdit*>(widget);
                qDebug() << lineEdit->text();
                valueMap->insert( NAME_KEY, lineEdit->text());
                break;
            case 6:
                lineEdit = qobject_cast<QLineEdit*>(widget);
                qDebug() << lineEdit->text();
                valueMap->insert( LAST_NAME_KEY, lineEdit->text());
                break;
            case 7:
                lineEdit = qobject_cast<QLineEdit*>(widget);
                qDebug() << lineEdit->text();
                valueMap->insert( SURE_NAME_KEY, lineEdit->text());
                break;
            }
        }
        valueList_.push_back( valueMap);
    }
    return &valueList_;
}

void DayTimeTableWidget::clear()
{
    if( tableWidget_->rowCount())
    {
        while( tableWidget_->rowCount())
            tableWidget_->removeRow( 0);
    }
}
