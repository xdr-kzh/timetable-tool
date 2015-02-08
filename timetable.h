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
    ~TimeTable();

public slots:
    void addEvent( QString groupName, int week, int eventTimeNumber, QString eventName,
        int eventType, QString location, int day, QString teacherFirstName, QString teacherLastName, QString teacherSureName);

private:
    QString dbFileName_;
    QSqlDatabase dbase_;
    TimeMap time;

    static QString INSERT_BATTLE_GROUP_QUERY;
    static QString INSERT_EVENT_QUERY;
    static QString SELECT_GROUP_QUERY;
    static QString EVENT_ID_QUERY;
    static QString SELECT_PERSON_QUERY;
    static QString GET_EVENT_TYPE_QUERY;
    static QString INSERT_PERSON_QUERY;
    static QString TEACHER_ID_QUERY;
    static QString INSERT_TIMETABLE_QUERY;
    static QString GROUP_ID_QUERY;
    static QString SELECT_PERSON_WITHOUT_NAME_QUERY;
    static QString SELECT_PERSON_WITHOUT_SURENAME_QUERY;
    static QString SELECT_PERSON_WITHOUT_NAME_AND_SURENAME_QUERY;
};

#endif // TIMETABLE_H
