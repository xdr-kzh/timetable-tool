#ifndef PARSER_H
#define PARSER_H

#include "definitions.h"
#include <qjsondocument.h>

class Parser
{    
private:
    static StringList parseGroups( std::wstring line);
    static PairList parseFvtPairsLine( const StringList& groups, std::wstring& curDay, std::wstring line);
    static bool feuLineHasLocation( const std::wstring& line);
    static Pair parseFeuEntry(std::wstring s, Pair newPair, bool isMi);
    static PairList parseFeuEntry( std::wstring s1, std::wstring s2, Pair tmpPair, bool isMi);
    static PairList parseFeu1PairsLine( const StringList& groups, std::wstring& curDay, std::wstring line, std::wstring line2, bool isMi);
    static std::wstring toJson( const PairList& pairs);

public:
    static QJsonDocument* parseFvt( QString filename);
    static void parseFeu1( QString filename);
    static void parseMiLl1( QString filename);
    static void parseFmt( QString filename);
    static void parseMiLl2( QString filename);
    static void parseMiLl3( QString filename);
    static void parseFPITE( QString filename);
};

#endif // PARSER_H
