#ifndef TIMETABLEFILEIMPORTER_H
#define TIMETABLEFILEIMPORTER_H

#include "timetable.h"

#include <QObject>

class TimetableFileImporter : public QObject
{
    Q_OBJECT
public:
    const static QString TIME_TABLE;
    const static QString PAIR_FIELD;
    const static QString LASTNAME_FIELD;
    const static QString FIRSTNAME_FIELD ;
    const static QString PATRONYMIC_FIELD ;
    const static QString TYPE_FIELD;
    const static QString TEACHER_FIELD;
    const static QString LOCATION_FIELD;
    const static QString START_TIME_FIELD;
    const static QString END_TIME_FIELD;
    const static QString WEEK_FIELD;
    const static QString TIME_FIELD;
    const static QString WARDAY_FIELD;

    const static QString DAY_FIELD;

    const static QString GROUP_FIELD;
    const static QString WEEK_NAME;

    const static QString GROUP_ID_TAG;
    const static QString GROUP_NAME_TAG;
    const static QString FACULTY_NAME_TAG;
    const static int MILITARY_TIME_NUMBER;

public:
    TimetableFileImporter(TimeTable* timetable, QObject *parent = 0);

public slots:
    void import( QString filename);

private:
    TimeTable* timetable_;
    std::map<QString, int> days_;
    std::map<QString, int> time_;
    std::map<QString, int> type_;
};

#endif // TIMETABLEFILEIMPORTER_H
