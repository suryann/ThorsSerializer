

#include "ParserRecursive.h"
#include "ParserShiftReduce.h"

using namespace ThorsAnvil::Json;

int ParserRecursive::error(int /*val*/, std::string const& msg)
{
    throw ThorsAnvil::Json::ParsingError(msg);
}

int ParserRecursive::JsonValueParse(int val, SMART_OWNED_PTR<JsonValue>& value)
{
    switch(val)
    {
        case '{':                                       {
                                                            SMART_OWNED_PTR<JsonMap>      map;
                                                            int result = JsonMapParse(yylex(), map);
                                                            if (result == 0)
                                                            {   value.reset(pi.valueParseMap(map.release()));
                                                            }
                                                            return result;
                                                        }
        case '[':                                       {
                                                            SMART_OWNED_PTR<JsonArray>    array;
                                                            int result = JsonArrayParse(yylex(), array);
                                                            if (result == 0)
                                                            {   value.reset(pi.valueParseArray(array.release()));
                                                            }
                                                            return result;
                                                        }
        case yy::ParserShiftReduce::token::JSON_STRING: value.reset(pi.valueParseString(pi.getStringLexer(lexer)));return 0;
        case yy::ParserShiftReduce::token::JSON_NUMBER: value.reset(pi.valueParseNumber(pi.getNumberLexer(lexer)));return 0;
        case yy::ParserShiftReduce::token::JSON_TRUE:   value.reset(pi.valueParseBool(true));    return 0;
        case yy::ParserShiftReduce::token::JSON_FALSE:  value.reset(pi.valueParseBool(false));   return 0;
        case yy::ParserShiftReduce::token::JSON_NULL:   value.reset(pi.valueParseNULL());        return 0;
        default:break;
    }
    return error(val, "syntax error");
}
int ParserRecursive::JsonMapValueListParse(int val, SMART_OWNED_PTR<JsonMap>& map)
{
    if (val == yy::ParserShiftReduce::token::JSON_STRING)
    {
        SMART_OWNED_PTR<std::string>  key(pi.getStringLexer(lexer));
        if ((val = yylex()) == ':')
        {
            key.reset(pi.mapKeyNote(key.release()));
            SMART_OWNED_PTR<JsonValue>    value;
            if ((val = JsonValueParse(yylex(), value)) == 0)
            {
                SMART_OWNED_PTR<JsonMapValue>   mapValue(pi.mapCreateElement(key.release(), value.release()));
                if (map.get() == NULL)
                {
                    map.reset(pi.mapCreate(mapValue.release()));
                }
                else
                {
                    map.reset(pi.mapAppend(map.release(), mapValue.release()));
                }
                val = yylex();
                switch(val)
                {
                    case '}':   return 0;
                    case ',':   return JsonMapValueListParse(yylex(), map);
                    default:break;
                }
            }
        }
    }
    return error(val, "syntax error");
}

int ParserRecursive::JsonArrayValueListParse(int val, SMART_OWNED_PTR<JsonArray>& array)
{
    SMART_OWNED_PTR<JsonValue>    value;
    if ((val = JsonValueParse(val, value)) == 0)
    {
        value.reset(pi.arrayCreateElement(value.release()));
        if (array.get() == NULL)
        {
            array.reset(pi.arrayCreate(value.release()));
        }
        else
        {
            array.reset(pi.arrayAppend(array.release(), value.release()));
        }
        val = yylex();
        switch(val)
        {
            case ']':   return 0;
            case ',':   array.reset(pi.arrayNote(array.release())); return JsonArrayValueListParse(yylex(), array);
            default:break;
        }
    }
    return error(val, "syntax error");
} 
int ParserRecursive::JsonMapParse(int val, SMART_OWNED_PTR<JsonMap>& map)
{
    pi.mapOpen();
    int result;
    if (val == '}')
    {
        map.reset(pi.mapCreate());
        result = 0;   /* Empty Map */
    }
    else
    {
        result = JsonMapValueListParse(val, map);
    }
    pi.mapClose();
    return result;
}
int ParserRecursive::JsonArrayParse(int val, SMART_OWNED_PTR<JsonArray>& array)
{
    pi.arrayOpen();
    int result;
    if (val == ']')
    {
        array.reset(pi.arrayCreate());
        result = 0;   /* Empty Array */
    }
    else
    {
        result = JsonArrayValueListParse(val, array);
    }
    pi.arrayClose();
    return result;
}
int ParserRecursive::parseJosnObject(int val)
{
    int                           result;
    SMART_OWNED_PTR<JsonMap>      map;
    SMART_OWNED_PTR<JsonArray>    array;
    switch(val)
    {
        case '{':   result = JsonMapParse(yylex(), map);     pi.doneMap(map.release());    return result;
        case '[':   result = JsonArrayParse(yylex(), array); pi.doneAray(array.release()); return result;
        default:break;
    }
    return error(val, "syntax error");
}
int ParserRecursive::parse()
{
    return parseJosnObject(yylex());
}

