
#ifndef THORSANVIL_JSON_PARSER_PARSER_INTERFACE_H
#define THORSANVIL_JSON_PARSER_PARSER_INTERFACE_H

#include "JsonDom.h"
#include "JsonException.h"

#include <string>
#include <iostream>


namespace ThorsAnvil
{
    namespace Json
    {

    class LexerJson;
    class ParserInterface;

/* Functions:
 *      std::string getString(LexerJson& lexer);
 *      std::string getNumber(LexerJson& lexer);
 *      int         yylex(void*, LexerJson& lexer,ThorsAnvil::Json::ParserInterface& paser);
 *
 * Exceptions:
 *      class InvalidConversion
 *      class ParsingError
 *
 * Interface for Parsing
 *      struct ParserInterface
 *      struct ParserLogInterface   (generated debug output)
 *      struct ParserCleanInterface
 *      struct ParserDomInterface
 *
 */

/*
 * Useful free functions
 */
std::string getString(LexerJson& lexer);
std::string getNumber(LexerJson& lexer);
int         yylex(void*, LexerJson& lexer,ThorsAnvil::Json::ParserInterface& paser);

/*
 * The interface injected into the parser that will do the work.
 */
struct ParserInterface
{
    ~ParserInterface()  {}
    virtual void            doneMap(JsonMap* map)                               = 0;
    virtual void            doneAray(JsonArray* array)                          = 0;
    virtual void            mapOpen()                                           = 0;
    virtual void            mapClose()                                          = 0;
    virtual JsonMap*        mapCreate()                                         = 0;
    virtual JsonMap*        mapCreate(JsonMapValue* val)                        = 0;
    virtual JsonMap*        mapAppend(JsonMap* map, JsonMapValue* val)          = 0;
    virtual JsonMapValue*   mapCreateElement(std::string* k,JsonValue* val)     = 0;
    virtual std::string*    mapKeyNote(std::string* k)                          = 0;
    virtual void            arrayOpen()                                         = 0;
    virtual void            arrayClose()                                        = 0;
    virtual JsonArray*      arrayCreate()                                       = 0;
    virtual JsonArray*      arrayCreate(JsonValue* val)                         = 0;
    virtual JsonArray*      arrayAppend(JsonArray* arr, JsonValue* val)         = 0;
    virtual JsonArray*      arrayNote(JsonArray* arr)                           = 0;
    virtual JsonValue*      arrayCreateElement(JsonValue* val)                  = 0;
    virtual JsonValue*      valueParseMap(JsonMap* map)                         = 0;
    virtual JsonValue*      valueParseArray(JsonArray* arr)                     = 0;
    virtual JsonValue*      valueParseString(std::string* str)                  = 0;
    virtual JsonValue*      valueParseNumber(std::string* num)                  = 0;
    virtual JsonValue*      valueParseBool(bool value)                          = 0;
    virtual JsonValue*      valueParseNULL()                                    = 0;
    virtual std::string*    getStringLexer(LexerJson& lexer)                    = 0;
    virtual std::string*    getNumberLexer(LexerJson& lexer)                    = 0;
    virtual int             lexResult(int val)                                  { return val;}
};

struct ParseLogInterface: ParserInterface
{
    virtual void            doneMap(JsonMap*)                                   {std::cout << "JsonObject: JsonMap\n";}
    virtual void            doneAray(JsonArray*)                                {std::cout << "JsonObject: JsonArray\n";}
    virtual void            mapOpen()                                           {}
    virtual void            mapClose()                                          {std::cout << "JsonMap: { JsonMapValueListOpt }\n";}
    virtual JsonMap*        mapCreate()                                         {std::cout << "JsonMapValueListOpt: EMPTY\n";                               return NULL;}
    virtual JsonMap*        mapCreate(JsonMapValue*)                            {std::cout << "JsonMapValueList: JsonMapValue\n";                           return NULL;}
    virtual JsonMap*        mapAppend(JsonMap*, JsonMapValue*)                  {std::cout << "JsonMapValueList: JsonMapValueList , JsonMapValue\n";        return NULL;}
    virtual JsonMapValue*   mapCreateElement(std::string*,JsonValue*)           {std::cout << "JsonMapValue: JSON_STRING : JsonValue\n";                    return NULL;}
    virtual std::string*    mapKeyNote(std::string*)                            {                                                                           return NULL;}
    virtual void            arrayOpen()                                         {}
    virtual void            arrayClose()                                        {std::cout << "JsonArray: [ JsonArrayValueListOpt ]\n";}
    virtual JsonArray*      arrayCreate()                                       {std::cout << "JsonArrayValueListOpt: EMPTY\n";                             return NULL;}
    virtual JsonArray*      arrayCreate(JsonValue*)                             {std::cout << "JsonArrayValueList: JsonArrayValue\n";                       return NULL;}
    virtual JsonArray*      arrayAppend(JsonArray*, JsonValue*)                 {std::cout << "JsonArrayValueList: JsonArrayListItem JsonArrayValue\n";     return NULL;}
    virtual JsonArray*      arrayNote(JsonArray*)                               {std::cout << "JsonArrayListItem: JsonArrayValueList ','";                  return NULL;}
    virtual JsonValue*      arrayCreateElement(JsonValue*)                      {std::cout << "JsonArrayValue: JsonValue\n";                                return NULL;}
    virtual JsonValue*      valueParseMap(JsonMap*)                             {std::cout << "JsonValue: JsonMap\n";                                       return NULL;}
    virtual JsonValue*      valueParseArray(JsonArray*)                         {std::cout << "JsonValue: JsonArray\n";                                     return NULL;}
    virtual JsonValue*      valueParseString(std::string*)                      {std::cout << "JsonValue: JsonString\n";                                    return NULL;}
    virtual JsonValue*      valueParseNumber(std::string*)                      {std::cout << "JsonValue: JsonNumber\n";                                    return NULL;}
    virtual JsonValue*      valueParseBool(bool)                                {std::cout << "JsonValue: JsonTrue\n";                                      return NULL;}
    virtual JsonValue*      valueParseNULL()                                    {std::cout << "JsonValue: JsonFalse\n";                                     return NULL;}
    virtual std::string*    getStringLexer(LexerJson&)                          {std::cout << "JsonString: JSON_STRING\n";                                  return NULL;}
    virtual std::string*    getNumberLexer(LexerJson&)                          {std::cout << "JsonNumber: JSON_NUMBER\n";                                  return NULL;}
    virtual int             lexResult(int val)                                  {std::cout << "LEX(" << val << ")\n";                                       return val;}
};

struct ParserCleanInterface: ParserInterface
{
    virtual void            doneMap(JsonMap* map)                               { delete map;}
    virtual void            doneAray(JsonArray* array)                          { delete array;}
    virtual void            mapOpen()                                           {}
    virtual void            mapClose()                                          {}
    virtual JsonMap*        mapCreate()                                         { return NULL;}
    virtual JsonMap*        mapCreate(JsonMapValue* val)                        { delete val; return NULL;}
    virtual JsonMap*        mapAppend(JsonMap* map, JsonMapValue* val)          { std::auto_ptr<JsonMapValue> aval(val); delete map; return NULL;}
    virtual JsonMapValue*   mapCreateElement(std::string* k,JsonValue* val)     { std::auto_ptr<JsonValue> aval(val); delete k;   return NULL;}
    virtual std::string*    mapKeyNote(std::string* k)                          { delete k; return NULL;}
    virtual void            arrayOpen()                                         {}
    virtual void            arrayClose()                                        {}
    virtual JsonArray*      arrayCreate()                                       { return NULL;}
    virtual JsonArray*      arrayCreate(JsonValue* val)                         { delete val; return NULL;}
    virtual JsonArray*      arrayAppend(JsonArray* arr, JsonValue* val)         { std::auto_ptr<JsonArray> aarr(arr); delete val; return NULL;}
    virtual JsonArray*      arrayNote(JsonArray* arr)                           { delete arr; return NULL;}
    virtual JsonValue*      arrayCreateElement(JsonValue* val)                  { delete val; return NULL;}
    virtual JsonValue*      valueParseMap(JsonMap* map)                         { delete map; return NULL;}
    virtual JsonValue*      valueParseArray(JsonArray* arr)                     { delete arr; return NULL;}
    virtual JsonValue*      valueParseString(std::string* str)                  { delete str; return NULL;}
    virtual JsonValue*      valueParseNumber(std::string* num)                  { delete num; return NULL;}
    virtual JsonValue*      valueParseBool(bool)                                { return NULL;}
    virtual JsonValue*      valueParseNULL()                                    { return NULL;}
    virtual std::string*    getStringLexer(LexerJson&)                          { return NULL;}
    virtual std::string*    getNumberLexer(LexerJson&)                          { return NULL;}
};

struct ParserDomInterface: ParserCleanInterface
{
    virtual JsonMap*        mapCreate()                                         { return new JsonMap();}
    virtual JsonMap*        mapCreate(JsonMapValue* val)                        { std::auto_ptr<JsonMapValue>aval(val);std::auto_ptr<JsonMap> amap(new JsonMap());amap->insert(*(aval->first), aval->second);return amap.release();}
    virtual JsonMap*        mapAppend(JsonMap* map, JsonMapValue* val)          { std::auto_ptr<JsonMapValue>aval(val);std::auto_ptr<JsonMap> amap(map);          amap->insert(*(aval->first), aval->second);return amap.release();}
    virtual JsonMapValue*   mapCreateElement(std::string* k,JsonValue* val)     { std::auto_ptr<JsonValue>  aval(val);std::auto_ptr<std::string> ak(k);
                                                                                  std::auto_ptr<JsonMapValue> result(new JsonMapValue);result->first = ak;result->second = aval;
                                                                                  return result.release();
                                                                                }
    virtual std::string*    mapKeyNote(std::string* k)                          { return k;}
    virtual JsonArray*      arrayCreate()                                       { return new JsonArray();}
    virtual JsonArray*      arrayCreate(JsonValue* val)                         { std::auto_ptr<JsonValue>  aval(val);std::auto_ptr<JsonArray>   aarr(new JsonArray());aarr->push_back(aval); return aarr.release();}
    virtual JsonArray*      arrayAppend(JsonArray* arr, JsonValue* val)         { std::auto_ptr<JsonValue>  aval(val);std::auto_ptr<JsonArray>   aarr(arr);            aarr->push_back(aval); return aarr.release();}
    virtual JsonArray*      arrayNote(JsonArray* arr)                           { return arr;}
    virtual JsonValue*      arrayCreateElement(JsonValue* val)                  { std::auto_ptr<JsonValue>  aval(val); return aval.release();}

    virtual JsonValue*      valueParseMap(JsonMap* map)                         { std::auto_ptr<JsonMap>     amap(map); return new JsonMapItem(amap);}
    virtual JsonValue*      valueParseArray(JsonArray* arr)                     { std::auto_ptr<JsonArray>   aarr(arr); return new JsonArrayItem(aarr);}
    virtual JsonValue*      valueParseString(std::string* str)                  { std::auto_ptr<std::string> astr(str); return new JsonStringItem(astr);}
    virtual JsonValue*      valueParseNumber(std::string* num)                  { std::auto_ptr<std::string> anum(num); return new JsonNumberItem(anum);}
    virtual JsonValue*      valueParseBool(bool value)                          { return new JsonBoolItem(value);}
    virtual JsonValue*      valueParseNULL()                                    { return new JsonNULLItem();}

    virtual std::string*    getStringLexer(LexerJson& lexer)                    { return new std::string(getString(lexer));}
    virtual std::string*    getNumberLexer(LexerJson& lexer)                    { return new std::string(getNumber(lexer));}
};


    }
}

#endif

