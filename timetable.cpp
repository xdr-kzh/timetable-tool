#include "timetable.h"

#include <QSqlQuery>
#include <QSqlResult>
#include <QDebug>
#include <QSqlError>

QString TimeTable::SELECT_GROUP_QUERY = "SELECT _id FROM battle_group WHERE name = (?)";
QString TimeTable::INSERT_BATTLE_GROUP_QUERY = QString::fromUtf8("INSERT INTO battle_group(name) VALUES (?)");
QString TimeTable::INSERT_EVENT_QUERY = QString::fromUtf8(
    "INSERT INTO event(week,start_time,end_time,discipline,type_id,location,day) VALUES ( ?, ?, ?, ?, ?, ?, ?);");
QString TimeTable::SELECT_PERSON_QUERY = "SELECT _id FROM Persons WHERE Name = (?) AND LastName = (?) AND SureName = (?)";
QString TimeTable::GET_EVENT_TYPE_QUERY = "";
QString TimeTable::INSERT_PERSON_QUERY = "INSERT INTO `Persons`(`Name`,`LastName`,`SureName`) VALUES (?, ?, ?)";
QString TimeTable::INSERT_TIMETABLE_QUERY = "INSERT INTO `TimeTable`(`EventId`,`OwnerTypeId`,`OwnerId`,`IsEmcee`) VALUES ( ?, ?, ?, ?);";

TimeTable::TimeTable(QString dbFileName, QObject *parent) :
    dbFileName_( dbFileName),
    QObject(parent)
{
    dbase_ = QSqlDatabase::addDatabase("QSQLITE");
    dbase_.setDatabaseName( dbFileName);
    if (!dbase_.open()) {
        qDebug() << "Что-то пошло не так!";
    }

    time.insert( std::pair< int, std::pair<int,int> >( 1, std::pair<int, int>( 480, 580)));
    time.insert( std::pair< int, std::pair<int,int> >( 2, std::pair<int, int>( 590, 685)));
    time.insert( std::pair< int, std::pair<int,int> >( 3, std::pair<int, int>( 700, 795)));
    time.insert( std::pair< int, std::pair<int,int> >( 4, std::pair<int, int>( 825, 920)));
    time.insert( std::pair< int, std::pair<int,int> >( 5, std::pair<int, int>( 935, 1030)));
    time.insert( std::pair< int, std::pair<int,int> >( 6, std::pair<int, int>( 1045, 1140)));
    time.insert( std::pair< int, std::pair<int,int> >( 7, std::pair<int, int>( 1155, 1250)));
    time.insert( std::pair< int, std::pair<int,int> >( 8, std::pair<int, int>( 480, 920)));
}

void TimeTable::addEvent( QString groupName, int week, int eventTimeNumber, QString eventName,
    int eventType, QString location, int day, QString teacherName, QString teacherLastName, QString teacherSureName)
{
    QSqlQuery dbQuery;
    QString query = SELECT_GROUP_QUERY;
    dbQuery.prepare( query);
    dbQuery.bindValue(0,groupName);

    bool b = dbQuery.exec();

    if (!b) {
        qDebug() << "Ошибка выполнения запроса 0";
    }

    int groupId = 0;

    /************---------1st----------************/
    if(!dbQuery.next())
    {
        query = INSERT_BATTLE_GROUP_QUERY;
        dbQuery.prepare( query);
        dbQuery.bindValue( 0, groupName);

        bool b = dbQuery.exec();
        if (!b) {
            qDebug() << "Ошибка выполнения запроса 1";
        }

        groupId = dbQuery.lastInsertId().toInt();
    }
    else
        groupId = dbQuery.value(0).toInt();

    /************---------2nd----------************/
    int startTime = 0;
    int endTime = 0;
    qDebug() << eventTimeNumber;
    TimeMap::iterator it = time.find( eventTimeNumber);
    if( it != time.end())
    {
        startTime = it->second.first;
        endTime = it->second.second;
    }
    qDebug() << "THE TIME" << startTime << " " << endTime;
    query = INSERT_EVENT_QUERY;
    dbQuery.prepare( query);
    dbQuery.bindValue( 0, QString::number(week));
    dbQuery.bindValue( 1, QString::number(startTime));
    dbQuery.bindValue( 2, QString::number(endTime));
    dbQuery.bindValue( 3, eventName);
    dbQuery.bindValue( 4, QString::number(eventType));
    dbQuery.bindValue( 5, location);
    dbQuery.bindValue( 6, QString::number(day));

    b = dbQuery.exec();
    if (!b) {
        qDebug() << "Ошибка выполнения запроса 2";
    }

    /************---------3rd----------************/

    int eventId = 0;
    eventId = dbQuery.lastInsertId().toInt();

    int teacherId = 0;
    query = SELECT_PERSON_QUERY;
    dbQuery.prepare( query);
    dbQuery.bindValue( 0, teacherName);
    dbQuery.bindValue( 1, teacherLastName);
    dbQuery.bindValue( 2, teacherSureName);

    b = dbQuery.exec();
    if (!b) {
        qDebug() << "Ошибка выполнения запроса 3.5";
    }

    if( dbQuery.boundValue(1).toInt() == 0)
    {
        //добавляем препода
        query = INSERT_PERSON_QUERY;
        dbQuery.prepare( query);
        dbQuery.bindValue( 0, teacherName);
        dbQuery.bindValue( 1, teacherLastName);
        dbQuery.bindValue( 2, teacherSureName);
        b = dbQuery.exec();
    }

    teacherId = dbQuery.lastInsertId().toInt();

    /************---------4th----------************/
    qDebug() << "FINAL: " << eventId <<" " << groupId << " " << teacherId;
    //группа
    query = INSERT_TIMETABLE_QUERY;
    dbQuery.prepare( query);
    dbQuery.bindValue( 0, QString::number( eventId));
    dbQuery.bindValue( 1, QString::number( 1));
    dbQuery.bindValue( 2, QString::number( groupId));
    dbQuery.bindValue( 3, QString::number( 0));
    b = dbQuery.exec();
    if (!b) {
        qDebug() << "Ошибка выполнения запроса 4a";
    }
    //препод
    query = INSERT_TIMETABLE_QUERY;
    dbQuery.prepare( query);
    dbQuery.bindValue( 0, QString::number( eventId));
    dbQuery.bindValue( 1, QString::number( 2));
    dbQuery.bindValue( 2, QString::number( teacherId));
    dbQuery.bindValue( 3, QString::number( 1));

    b = dbQuery.exec();
    if (!b) {
        qDebug() << "Ошибка выполнения запроса 4b";
    }
}

TimeTable::~TimeTable()
{
    QSqlDatabase::removeDatabase( dbFileName_);
}
