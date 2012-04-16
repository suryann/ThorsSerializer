
#ifndef THORSANVIL_jSON_PARSER_PARSE_RECURSIVE_H
#define THORSANVIL_jSON_PARSER_PARSE_RECURSIVE_H

#include "ParserInterface.h"
#include "LexerJson.h"


namespace ThorsAnvil
{
    namespace Json
    {

class LexerJson;

class ParserRecursive
{
        LexerJson&         lexer;
        ParserInterface&    pi;

        int JsonValueParse(int val, std::auto_ptr<JsonValue>& value);
        int JsonMapValueListParse(int val, std::auto_ptr<JsonMap>& ma);
        int JsonArrayValueListParse(int val, std::auto_ptr<JsonArray>& array);
        int JsonMapParse(int val, std::auto_ptr<JsonMap>& map);
        int JsonArrayParse(int val, std::auto_ptr<JsonArray>& array);
        int parseJosnObject(int val);

        int yylex() { return lexer.yylex(pi);}
        int error(int val, std::string const& msg);

    public:
        ParserRecursive(LexerJson& l, ParserInterface& p)
            : lexer(l)
            , pi(p)
        {}
        int parse();
};
    }
}

#endif

