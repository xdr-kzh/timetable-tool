#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <map>

typedef std::pair<int, int> IntPair;
typedef std::map< int, IntPair> TimeMap;

class TimeTable : public QObject
{
    Q_OBJECT
public:
    explicit TimeTable( QString dbFileName, QObject *parent = 0);

signals:

public slots:

    void addEvent( QString groupName, int week, int eventTimeNumber, QString eventName,
        int eventType, QString location, int day, QString teacherFirstName, QString teacherName, QString teacherLastName);

private:
    QSqlDatabase dbase_;
    TimeMap time;

    static QString BATTLE_GROUP_QUERY;
    static QString ADD_EVENT_QUERY;
    static QString GET_GROUP_QUERY;
    static QString EVENT_ID_QUERY;
    static QString GET_TEACHER_NAME_QUERY;
    static QString GET_EVENT_TYPE_QUERY;
    static QString ADD_TEACHER_QUERY;
    static QString TEACHER_ID_QUERY;
    static QString ADD_TO_TIMETABLE_QUERY;
    static QString GROUP_ID_QUERY;
};

#endif // TIMETABLE_H
