#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <fstream>
#include <locale>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfoList>
#include <QFileInfo>
#include <QString>

#include "parser.h"


StringList Parser::parseGroups( std::wstring line) {
    boost::replace_all(line, L"	дни	часы	", L",");
    line.erase(0, 9);
    line.erase(line.length() - 1, 1);
    boost::replace_all(line, L"	", L"");
    StringList result;
    boost::split(result, line, boost::is_any_of(L","));
    return result;
}

PairList Parser::parseFvtPairsLine( const StringList& groups, std::wstring& curDay, std::wstring line) {
    static const StringList warDetectionWords = {L"Военной", /*L"День", L"подготовки",*/ L"ВОЕННОЙ", /*L"ДЕНЬ", L"ПОДГОТОВКИ"*/ };
    PairList pairs;
    line.erase(line.length() - 1, 1);
    std::vector<std::wstring> parts;
    boost::split(parts, line, boost::is_any_of(L"\t"));
    if( parts.at(0) != L"\t" && !parts.at(0).empty()) {
        curDay = parts.at(0);
    }
    bool skipExtraTab = false;
    skipExtraTab = parts.at(0).empty();
    parts.erase(parts.begin(), parts.begin() + 1);
    parts = boost::remove_erase_if( parts, [&curDay](const std::wstring& val) { return val == curDay; });
    auto it = parts.begin(), end = parts.end();
    int groupIndex = 0;
    while( groupIndex != groups.size()) {
        Pair newPair;
        newPair.group = groups.at( groupIndex);
        boost::erase_all(newPair.group, " ");
        newPair.day = curDay;
        newPair.time = *it;
        newPair.pair = *(++it);
        if( std::find(warDetectionWords.begin(), warDetectionWords.end(), newPair.pair) != warDetectionWords.end()) {
            newPair.isWarDay = true;
        }
        if(!newPair.isWarDay || !newPair.pair.empty()) {
            static const StringList weekWords = { L"1н.", L"1н ", L"2н.", L"2н "};
            if( std::find(weekWords.begin(), weekWords.end(), newPair.pair.substr(0, 3)) != weekWords.end()) {
                newPair.week = newPair.pair.at(0);
                newPair.pair.erase(0, 3);
                boost::trim(newPair.pair);
            }

            {
                boost::wsmatch res;
                boost::wregex regx(L"ин\\.яз\\.? *(\\w+) *(\\d.?-\\d\\d\\d),? ?(\\w+) *(\\d.?-\\d\\d\\d)",boost::wregex::icase);
                if( boost::regex_search(newPair.pair, res, regx)) {
                    newPair.location = res[2] + L", " + res[4];
                    newPair.pair = L"Ин.яз.";
                    newPair.teacher = res[1] + L", " + res[3];
                } else {
                    {
                        boost::wsmatch res;
                        boost::wregex regx(L"((\\d.?-\\d\\d\\d.?,?)+)",boost::wregex::icase);
                        if( boost::regex_search(newPair.pair, res, regx)) {
                            newPair.location = res[1];
                            newPair.pair = boost::regex_replace(newPair.pair, regx, "");
                            boost::trim(newPair.pair);
                            boost::trim(newPair.location);
                        }
                    }

                    {
                        boost::wsmatch res;
                        boost::wregex regx(L"( *\\.* *лб *\\.* *)", boost::wregex::icase);
                        if( boost::regex_search(newPair.pair, res, regx)) {
                            newPair.type = std::wstring( L"лб");
                            newPair.pair = boost::regex_replace(newPair.pair, regx, "");
                            boost::trim(newPair.pair);
                        } else {
                            boost::wsmatch res;
                            boost::wregex regx(L" ?\\.? ?(лек)\\.? ?(.*)", boost::wregex::icase);
                            if( boost::regex_search(newPair.pair, res, regx)) {
                                newPair.type = std::wstring( L"лек");
                                newPair.teacher = res[2];
                                newPair.pair = boost::regex_replace(newPair.pair, regx, "");
                                boost::trim(newPair.pair);
                                boost::trim(newPair.teacher);
                            }
                        }
                    }
                }
            }
        }

        if( newPair.isWarDay || (!newPair.pair.empty() && !newPair.time.empty())) {
            pairs.push_back(newPair);
        }
        ++groupIndex;
        ++it;
        if(skipExtraTab) {
            ++it;
        }
    }
    return pairs;
}

bool Parser::feuLineHasLocation( const std::wstring& line) {
    boost::wsmatch res;
    boost::wregex regx(L"(((\\d.?-)?\\d\\d\\d.?,?.?)+)",boost::wregex::icase);
    if( boost::regex_search(line, res, regx)) {
        return true;
    } else {
        boost::wregex regx(L"ОКБ ?\\d?",boost::wregex::icase);
        if( boost::regex_search(line, res, regx)) {
            return true;
        } else {
            boost::wregex regx(L"Физическая культура", boost::wregex::icase);
            if( boost::regex_search(line, res, regx)) {
                return true;
            } else {
                boost::wregex regx(L"10к\\. ?Акт\\.?\\/зал", boost::wregex::icase);
                if( boost::regex_search(line, res, regx)) {
                    return true;
                } else {
                    boost::wregex regx(L"акт\\. зал\\.Роспотреб\\.", boost::wregex::icase);
                    if( boost::regex_search(line, res, regx)) {
                        return true;
                    } else {
                        boost::wregex regx(L"10к?\\.? ?а/зал", boost::wregex::icase);
                        if( boost::regex_search(line, res, regx)) {
                            return true;
                        } else {
                            boost::wregex regx(L"\"ОКБ каф\\. \"\"ТО и ВЭМ\"\"\"", boost::wregex::icase);
                            if( boost::regex_search(line, res, regx)) {
                                return true;
                            } else {
                                boost::wregex regx(L"Ж/д\\.? ?бол\\.?", boost::wregex::icase);
                                if( boost::regex_search(line, res, regx)) {
                                    return true;
                                } else {
                                    boost::wregex regx(L"каф\\. ?(Стоматология)?", boost::wregex::icase);
                                    if( boost::regex_search(line, res, regx)) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

Pair Parser::parseFeuEntry(std::wstring s, Pair newPair, bool isMi) {
    if(!newPair.isWarDay || !newPair.pair.empty()) {
        boost::wregex regx(L"Физическая культура", boost::wregex::icase);
        boost::wsmatch res;
        if( boost::regex_search(s, res, regx)) {
            newPair.pair = L"Физическая культура";
            return newPair;
        }
        {
            if(isMi) {
                {
                    boost::wsmatch res;
                    boost::wregex regx(L"(ОКБ ?\\d?)", boost::wregex::icase);
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = res[0];
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"10к\\. ?Акт\\.?\\/зал", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"10к\\. Акт\\. зал\\.";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"акт\\. зал\\.Роспотреб\\.", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"акт\\. зал\\.Роспотреб\\.";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"10к?\\.? ?а/зал", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"10к. а/зал";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"\"ОКБ каф\\. \"\"ТО и ВЭМ\"\"\"", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"ОКБ каф. ТО и ВЭМ";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"Ж/д\\.? ?бол\\.?", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"Ж/д. бол.";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
                {
                    boost::wregex regx(L"каф\\. ?(Стоматология)?", boost::wregex::icase);
                    boost::wsmatch res;
                    if( boost::regex_search(s, res, regx)) {
                        newPair.location = L"каф. Стоматология";
                        s = boost::regex_replace(s, regx, "");
                        boost::trim(s);
                    }
                }
            }
            boost::wsmatch res;
            boost::wregex regx(L"((\\d.?-)?\\d\\d\\d\\w?(,\\w)?),? *",boost::wregex::icase);
            while( boost::regex_search(s, res, regx)) {
                if(!newPair.location.empty()) {
                    newPair.location += L",";
                }
                newPair.location += res[1];
                s = boost::regex_replace(s, regx, "", boost::format_first_only);
                boost::trim(s);
            }
            if(!newPair.location.empty() && (newPair.location.at( newPair.location.size()-1) == L',')) {
                newPair.location.erase( newPair.location.length() - 1, 1);
            }
        }

        {
            boost::wsmatch res;
            boost::wregex regx(L"[ \\.]?((\\w+) (.?)\\.(.?)\\.)? ?\\.? ?(ле?к(ция)?)\\.? ?(а\\.)? ?((.+) ?(.?)\\.(.?)\\.)?", boost::wregex::icase);
            if( boost::regex_search(s, res, regx)) {
                if(res[9].str().empty() && res[10].str().empty()) {
                    newPair.teacher = res[1];
                } else {
                    newPair.teacher = res[8];
                }
                newPair.type = std::wstring( L"лек");
                s = boost::regex_replace(s, regx, "");
                boost::trim(newPair.teacher);
                boost::trim(s);
            } else {
                boost::wsmatch res;
                boost::wregex regx(L"( *\\.* *пр((\\.а ?)|(актика)|(\\. ?)) *\\.? *а?\\.? ?(.*))", boost::wregex::icase);
                if( boost::regex_search(s, res, regx)) {
                    newPair.type = std::wstring( L"пр");
                    s = boost::regex_replace(s, regx, "");
                    newPair.teacher = res[6];
                    boost::trim(s);
                } else {
                    boost::wsmatch res;
                    boost::wregex regx(L"( *\\.* *лб *\\.* *а?\\.?)(.*)?", boost::wregex::icase);
                    if( boost::regex_search(s, res, regx)) {
                        newPair.type = std::wstring( L"лб");
                        s = boost::regex_replace(s, regx, "");
                        if(!res[2].str().empty()) {
                            newPair.teacher = res[2];
                        }
                        boost::trim(s);
                    }
                }
            }
        }

        newPair.pair = s;
        if(newPair.teacher == L".а") {
            newPair.teacher = L"";
        }

        if( newPair.type.empty()) {
            newPair.type = L"пр";
        }
    }
    return newPair;
}

PairList Parser::parseFeuEntry( std::wstring s1, std::wstring s2, Pair tmpPair, bool isMi) {
    PairList pps;
    if( s1.empty() && !s2.empty()) {
        tmpPair.week=L"2";
        pps.push_back( parseFeuEntry( s2, tmpPair, isMi));
        // 1 p - 2 w
    } else if( !s1.empty() && s2.empty()) {
        tmpPair.week=L"1";
        pps.push_back( parseFeuEntry( s1, tmpPair, isMi));
        // 1 p - 1 w
    } else if( !s1.empty() && !s2.empty()) {
        bool s1Has = feuLineHasLocation(s1);
        bool s2Has = feuLineHasLocation(s2);
        if( (!s1Has && s2Has) || (s1Has && !s2Has)) {
            tmpPair.week=L"";
            pps.push_back(parseFeuEntry( s1 + L" " + s2, tmpPair, isMi));
            // 1 pair - all week
        } else if(s1Has && s2Has) {
            Pair p1(tmpPair);
            p1.week=L"1";
            Pair p2(tmpPair);
            p2.week=L"2";
            pps.push_back(parseFeuEntry( s1, p1, isMi));
            pps.push_back(parseFeuEntry( s2, p2, isMi));
            // 2 pair 2 week
        } else if(!s1Has && !s2Has) {
            tmpPair.week=L"";
            pps.push_back(parseFeuEntry( s1 + L" " + s2, tmpPair, isMi));
        }
    }
    return pps;
}

PairList Parser::parseFeu1PairsLine( const StringList& groups, std::wstring& curDay, std::wstring line, std::wstring line2, bool isMi) {
    static const std::wstring warDetectionWord = L"ВОЕННОЙ";
    static const StringList ignoreWords = {L"День", L"подготовки", L"ДЕНЬ", L"ПОДГОТОВКИ", L"самостоятельной", L"день"};
    PairList pairs;
    line.erase(line.length() - 1, 1);
    std::vector<std::wstring> parts;
    boost::split(parts, line, boost::is_any_of(L"\t"));
    boost::trim(parts.at(0));
    if( parts.at(0) != L"\t" && !parts.at(0).empty()) {
        curDay = parts.at(0);
    }
    bool skipExtraTab = false;
    skipExtraTab = parts.at(0).empty();
    parts.erase(parts.begin(), parts.begin() + 1);
    parts = boost::remove_erase_if( parts, [&curDay](const std::wstring& val) { return (val == curDay) || (val == curDay + std::wstring(L".")); });

    std::vector<std::wstring> parts2;
    boost::split(parts2, line2, boost::is_any_of(L"\t"));

    auto it = parts.begin(), end = parts.end();
    auto it2 = parts2.begin(), end2 = parts2.end();
    int groupIndex = 0;
    while( groupIndex != groups.size()) {
        ++it2;
        ++it2;
        Pair newPair;
        newPair.group = groups.at( groupIndex);
        boost::erase_all(newPair.group, " ");
        newPair.day = curDay;
        newPair.time = *it;
        newPair.pair = *(++it);
        boost::trim(newPair.pair);
        if((newPair.pair == warDetectionWord) || (*it2 == warDetectionWord)) {
            newPair.isWarDay = true;
            pairs.push_back(newPair);
        } else {
            if((std::find(ignoreWords.begin(), ignoreWords.end(), newPair.pair) == ignoreWords.end()) && (std::find(ignoreWords.begin(), ignoreWords.end(), *it2) == ignoreWords.end())) {
                boost::trim(*it);
                boost::trim(*it2);
                //std::wcout << newPair.group << ": " << *it << " -- " << *it2 << std::endl;
                auto pps = parseFeuEntry(*it, *it2, newPair, isMi);
                pairs.insert(pairs.end(), pps.begin(), pps.end());
            }
        }
        ++it2;
        ++groupIndex;
        ++it;
        if(skipExtraTab) {
            ++it;
        }
    }
    return pairs;
}

std::wstring Parser::toJson( const PairList& pairs) {
    QJsonArray pairsJson;
    auto it = pairs.begin(), end = pairs.end();
    for( ;it != end; ++it) {
        QJsonObject obj;
        obj["day"] = QString::fromStdWString(it->day);
        obj["group"] = QString::fromStdWString(it->group);
        obj["isWarDay"] = it->isWarDay;
        obj["pair"] = QString::fromStdWString(it->pair);
        obj["time"] = QString::fromStdWString(it->time);
        obj["week"] = QString::fromStdWString(it->week);
        obj["location"] = QString::fromStdWString(it->location);
        obj["type"] = QString::fromStdWString(it->type);
        obj["teacher"] = QString::fromStdWString(it->teacher);
        pairsJson.push_back(obj);
    }
    QJsonDocument doc(pairsJson);
    return QString(doc.toJson()).toStdWString();
}

QJsonDocument* Parser::parseFvt( QString filename) {
    //std::wfstream in("timetables/fvt.txt");
    std::wfstream in( filename.toStdString());

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    while( std::getline(in, line)) {
        PairList newPairs = parseFvtPairsLine( groupsList, curDay, line);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    std::wofstream out;
//    out.open("timetables/fvt.json");
//    out << toJson(pairs) << std::endl;
//    out.close();
    return new QJsonDocument( QJsonDocument::fromJson( QString::fromStdWString( toJson(pairs)).toUtf8()));
}

void Parser::parseFeu1( QString filename) {
//    std::wfstream in("timetables/FEU.txt");
    std::wfstream in( filename.toStdString());

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    std::wstring line2;
    while( std::getline(in, line) && std::getline(in, line2)) {
        PairList newPairs = parseFeu1PairsLine( groupsList, curDay, line, line2, false);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    std::wofstream out;
    out.open("timetables/FEU.json");
    out << toJson(pairs) << std::endl;
    out.close();
}

void Parser::parseMiLl1( QString filename) {
    std::wfstream in("timetables/mi_ll_1.txt");

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    std::wstring line2;
    while( std::getline(in, line) && std::getline(in, line2)) {
        PairList newPairs = parseFeu1PairsLine( groupsList, curDay, line, line2, true);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    std::wofstream out;
    out.open("timetables/mi_ll_1.json");
    out << toJson(pairs) << std::endl;
    out.close();
}

void Parser::parseFmt( QString filename) {
    std::wfstream in("timetables/fmt.txt");

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    std::wstring line2;
    while( std::getline(in, line) && std::getline(in, line2)) {
        PairList newPairs = parseFeu1PairsLine( groupsList, curDay, line, line2, true);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    std::wofstream out;
    out.open("timetables/fmt.json");
    out << toJson(pairs) << std::endl;
    out.close();
}

void Parser::parseMiLl2( QString filename) {
    std::wfstream in("timetables/mi_ll_2.txt");

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    std::wstring line2;
    while( std::getline(in, line) && std::getline(in, line2)) {
        PairList newPairs = parseFeu1PairsLine( groupsList, curDay, line, line2, true);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    std::wofstream out;
    out.open("timetables/mi_ll_2.json");
    out << toJson(pairs) << std::endl;
    out.close();
}

void Parser::parseMiLl3( QString filename) {
    /*
    "pair": "Патол.анатомия,клин.пат.анат. Купрюшина Н.В.",
    "pair": "Общ.хир.-я, луч.диагностика Митрошин",
    "pair": "ПВБ,ЛД Олейников, Томашевская, Елисеева",
    "pair": "Невр.,мед.генет., нейрох. Ермолаева А.И.",
    */
    std::wfstream in("timetables/mi_lf_ll_3k.txt");

    PairList pairs;

    std::wstring line;
    std::getline(in, line);
    std::vector<std::wstring> groupsList = parseGroups(line);

    std::wstring curDay;
    std::wstring line2;
    while( std::getline(in, line) && std::getline(in, line2)) {
        PairList newPairs = parseFeu1PairsLine( groupsList, curDay, line, line2, true);
        std::copy(newPairs.begin(), newPairs.end(), std::back_inserter(pairs));
    }

    for( int i = 0; i < pairs.size(); ++i) {
        {
            boost::wregex regx(L"((Купрюшина Н\\.В\\.)|(Митрошин)|(Олейников, Томашевская, Елисеева)|(Ермолаева А\\.И\\.))", boost::wregex::icase);
            boost::wsmatch res;
            if( boost::regex_search(pairs.at(i).pair, res, regx)) {
                pairs.at(i).teacher = res[1];
                pairs.at(i).pair = boost::regex_replace(pairs.at(i).pair, regx, "");
                boost::trim(pairs.at(i).pair);
                boost::trim(pairs.at(i).teacher);
            }
        }
    }

    std::wofstream out;
    out.open("timetables/mi_lf_ll_3k.json");
    out << toJson(pairs) << std::endl;
    out.close();
}


void Parser::parseFPITE( QString filename) {

    //TODO: переименовать файлы на русский, чтобы имена групп были на русском

    QMap<int, QString> daysLetters;
    daysLetters.insert(0, "пн");
    daysLetters.insert(1, "вт");
    daysLetters.insert(2, "ср");
    daysLetters.insert(3, "чт");
    daysLetters.insert(4, "пт");
    daysLetters.insert(5, "сб");
    daysLetters.insert(6, "вс");

    QDir fpiteJSONdir("timetables/fpite_src");
    fpiteJSONdir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList listOfFiles = fpiteJSONdir.entryInfoList();

    QJsonArray fpiteTimetable;
    bool bothWeeksAreDifferent = false;

    for(int i=0; i<listOfFiles.size(); i++){
        QFileInfo currentFileInfo = listOfFiles.at(i);

        QFile currentFile( currentFileInfo.absoluteFilePath());

        if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"can't open this shit";
        }

        QString data = currentFile.readAll();

        QJsonDocument timeTable = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject dataObject = timeTable.object();
        QStringList days = dataObject.keys();

        for(int j=0;j<dataObject.size();j++){
            QJsonObject dayRecord = dataObject.value(days.at(j)).toObject();
            QStringList dayKeys = dayRecord.keys();

            for(int k=0; k<dayRecord.size(); k++){

                QJsonObject event;
                std::wstring string;
                bool solution = false;

                event.insert("group", currentFileInfo.baseName());
                event.insert("time", dayKeys.at(k));
                event.insert("day", daysLetters.value(j));

                if(bothWeeksAreDifferent==true){

                    bothWeeksAreDifferent = false;
                    QJsonObject secondWeek = dayRecord.value(dayKeys.at(k)).toObject();
                    event.insert("week", 2);
                    string = secondWeek.value("2").toString().toStdWString();
                    std::wcout<<"3rd: "<<string<<std::endl;
                } else {

                    if(dayRecord.value(dayKeys.at(k)).isObject()){

                        bothWeeksAreDifferent = true;
                        QJsonObject firstWeek = dayRecord.value(dayKeys.at(k)).toObject();
                        event.insert("week", 1);
                        string = firstWeek.value("1").toString().toStdWString();

                        --k;
                    }
                    else{
                        event.insert("week", 3);
                        string = dayRecord.value(dayKeys.at(k)).toString().toStdWString();
                    }
                }

                if(string == QString("Д Е Н Ь").toStdWString()){

                    std::wstring string2 = dayRecord.value(dayKeys.at(k+1)).toString().toStdWString();

                    if (string2 == QString("В О Е Н Н О Й").toStdWString()){

                        event.insert("isWarDay", true);
                        event.insert("pair", "День военной подготовки");
                        event.insert("time", "08:00");
                        event.insert("location", "Корпус №2");
                        event.insert("teacher", "");
                        event.insert("type", "");
                        solution = true;
                    }

                    k=k+2;
                }

                if(string == QString("Ф И З И Ч Е С К А Я К У Л Ь Т У Р А").toStdWString()){
                    event.insert("pair", "Физческая культура");
                    event.insert("isWarDay", false);
                    event.insert("location", "Индивидуально");
                    event.insert("teacher", "");
                    event.insert("type", "");
                    solution = true;
                }

                std::wcout<<"string is: "<<string<<std::endl;

                boost::wsmatch res;

                boost::wregex regex(L"(.*) лб (\\d.?-\\d\\d\\d.?)", boost::wregex::icase);
                if(boost::regex_search(string, res, regex) && (solution==false)){
                    event.insert("pair", QString::fromStdWString(res[1]));
                    event.insert("location", QString::fromStdWString(res[2]));
                    event.insert("type", "Лабораторная");
                    event.insert("teacher", "");
                    event.insert("isWarDay", false);
                    solution = true;
                    qDebug()<<"***** 1 ******";
                }

                boost::wsmatch res1;

                boost::wregex regex1(L"(.*) Лек[ция.]*(.*) (\\d.?-\\d\\d\\d.?)", boost::wregex::icase);
                if(boost::regex_search(string, res1, regex1) && (solution==false)){
                    event.insert("pair", QString::fromStdWString(res1[1]));
                    event.insert("location", QString::fromStdWString(res1[3]));
                    event.insert("type", "Лекция");
                    event.insert("teacher", QString::fromStdWString(res1[2]));
                    event.insert("isWarDay", false);
                    solution = true;
                    qDebug()<<"***** 2 ******";
                }

                boost::wsmatch res3;

                boost::wregex regex2(L"(.*) (\\d.?-\\d\\d\\d.?)", boost::wregex::icase);
                if(boost::regex_search(string, res3, regex2) && (solution==false)){
                    event.insert("pair", QString::fromStdWString(res3[1]));
                    event.insert("location", QString::fromStdWString(res3[2]));
                    event.insert("type", "Лабораторная");
                    event.insert("teacher", "");
                    event.insert("isWarDay", false);
                    qDebug()<<"***** 3 ******";
                }

                std::wcout<<"res: "<<res[1]<<" "<<res[2]<<" "<<res[3]<<std::endl;



                fpiteTimetable.append(event);
            }
        }


        //qDebug()<<fpiteTimetable;
        currentFile.close();
    }

    QJsonDocument doc = (QJsonDocument) fpiteTimetable;
    QByteArray rawDoc = doc.toJson();
    QFile file("FPITE.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"Something gone wrong";
    }
    file.write(rawDoc);
}


//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//    std::locale::global( std::locale(""));

//    //parseFvt();
//    //parseFeu1();
//    parseFPITE();
//    parseMiLl1();
//    parseFmt();
//    parseMiLl2();
//    parseMiLl3();
//}

