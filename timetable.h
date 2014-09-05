#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QObject>
#include <QSqlDatabase>

class TimeTable : public QObject
{
    Q_OBJECT
public:
    explicit TimeTable( QString dbFileName, QObject *parent = 0);

signals:

public slots:

private:
    addLecture( );

private:
    QSqlDatabase* dbase_;

    static QString BATTLE_GROUP_QUERY;
    static QString EVENT_QUERY;
    static QString COMMA;
    static QString ENDING;
    static QString EVENT_ID_QUERY;
    static QString GET_TEACHER_FIRST_NAME_QUERY;
    static QString GET_TEACHER_AND_NAME_QUERY;
    static QString GET_TEACHER_AND_LAST_NAME_QUERY;
    static QString GET_EVENT_TYPE_QUERY;
    static QString ADD_TEACHER_QUERY;
    static QString ADD_TO_TIMETABLE_QUERY;
};

#endif // TIMETABLE_H
