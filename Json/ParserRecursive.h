
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

        int JsonValueParse(int val, SMART_OWNED_PTR<JsonValue>& value);
        int JsonMapValueListParse(int val, SMART_OWNED_PTR<JsonMap>& ma);
        int JsonArrayValueListParse(int val, SMART_OWNED_PTR<JsonArray>& array);
        int JsonMapParse(int val, SMART_OWNED_PTR<JsonMap>& map);
        int JsonArrayParse(int val, SMART_OWNED_PTR<JsonArray>& array);
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

