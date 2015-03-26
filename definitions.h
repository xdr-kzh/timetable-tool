#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>

static const QString EVENT_TIME_KEY = "EVENT_TIME_KEY";
static const QString WEEK_KEY = "WEEK_KEY";
static const QString EVENT_NAME_KEY = "EVENT_NAME_KEY";
static const QString EVENT_TYPE_KEY = "EVENT_TYPE_KEY";
static const QString EVENT_LOCATION_KEY = "EVENT_LOCATION_KEY";
static const QString NAME_KEY = "NAME_KEY";
static const QString LAST_NAME_KEY = "LAST_NAME_KEY";
static const QString SURE_NAME_KEY = "SURE_NAME_KEY";

struct Pair {
    bool isWarDay = false;
    std::wstring group;
    std::wstring day;
    std::wstring time;
    std::wstring pair;
    std::wstring week;
    std::wstring teacher;
    std::wstring type;
    std::wstring location;

};

typedef std::vector< Pair> PairList;
typedef std::vector<std::wstring> StringList;


#endif // DEFINITIONS_H
