#include "timetable.h"

#include <QSqlQuery>
#include <QSqlResult>

QString TimeTable::COMMA = ",";
QString TimeTable::ENDING = ");";
QString TimeTable::BATTLE_GROUP_QUERY = QString::fromUtf8("INSERT INTO `battle_group`(`name`) VALUES (");
QString TimeTable::EVENT_QUERY = QString::fromUtf8(
    "INSERT INTO `event`(`week`,`start_time`,`end_time`,`discipline`,`type_id`,`location`,`day`) VALUES (");

QString TimeTable::EVENT_ID_QUERY = "SELECT last_insert_rowid() FROM `event`";
QString TimeTable::GET_TEACHER_FIRST_NAME_QUERY = "SELECT * FROM Persons WHERE Persons.FirstName = ";
QString TimeTable::GET_TEACHER_AND_NAME_QUERY = " AND WHERE Persons.Name = ";
QString TimeTable::GET_TEACHER_AND_LAST_NAME_QUERY = " AND WHERE Persons.LastName = ";
QString TimeTable::GET_EVENT_TYPE_QUERY = "";
QString TimeTable::ADD_TEACHER_QUERY = "INSERT INTO `Persons`(`Name`,`LastName`,`SureName`,`Department`,`Post`,`Biography`,`Photo`,`PhoneNumber`,`Email`) VALUES (";

QString TimeTable::ADD_TO_TIMETABLE_QUERY = "INSERT INTO `TimeTable`(`EventId`,`OwnerTypeId`,`OwnerId`,`IsEmcee`) VALUES (";

TimeTable::TimeTable(QString dbFileName, QObject *parent) :
    QObject(parent)
{
    dbase_ = new QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName( dbFileName);
    if (!dbase.open()) {
        qDebug() << "Что-то пошло не так!";
    }

}

void TimeTable::addEvent( QString groupName, int week, int eventTimeNumber, QString eventName,
    int eventType, QString location, int day, QString teacherFirstName, QString teacherName, QString teacherLastName)
{
    QSqlQuery a_query;
    QString query = BATTLE_GROUP_QUERY;
    query += groupName + ENDING;
    bool b = a_query.exec( query);
    if (!b) {
        qDebug() << "Ошибка выполнения запроса";
    }

    int startTime = 0;
    int endTime = 0;

    query = EVENT_QUERY + QString::number(week) + COMMA + QString::number(startTime) + COMMA
        + QString::number(endTime) + COMMA + eventName + COMMA + QString::number(eventType) + COMMA
            + location + COMMA + QString::number( day) + ENDING;

    b = a_query.exec( query);
    if (!b) {
        qDebug() << "Ошибка выполнения запроса";
    }

    int eventId = 0;
    if( a_query.last())
    {
        eventId = a_query.value(0).toInt();
    }

    query = GET_TEACHER_FIRST_NAME_QUERY + teacherFirstName +
        GET_TEACHER_AND_NAME_QUERY + teacherName +
            GET_TEACHER_AND_LAST_NAME_QUERY + teacherLastName + ENDING;


    b = a_query.exec( query);
    if (!b) {
        qDebug() << "Ошибка выполнения запроса";
    }

    //хз че тут
    if( a_query.result()->handle().value(0) == 0)
    {
        //добавляем препода
        query = ADD_TEACHER_QUERY + teacherFirstName + COMMA +
            teacherName + COMMA + teacherName + COMMA + "NULL,NULL,NULL,NULL,NULL);";
        b = a_query.exec( query);
    };

//    int eventType = 0;
//    b = a_query.exec( query);
//    if (!b) {
//        qDebug() << "Ошибка выполнения запроса";
//    }

//    int eventId = 0;
//    if( a_query.last())
//    {
//        eventType = a_query.value(0).toInt();
//    }

    //final insult
    query = ADD_TO_TIMETABLE_QUERY + QString::number( eventId) + COMMA +
            QString::number( 0) + COMMA +
            QString::number( 0) + COMMA +
            QString::number( 0) + COMMA + ENDING;

    b = a_query.exec( query);
    if (!b) {
        qDebug() << "Ошибка выполнения запроса";
    }


}
