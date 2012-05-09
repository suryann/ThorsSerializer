
%skeleton "lalr1.cc"
%require "2.1a"
%defines
%define "parser_class_name" "ParserShiftReduce"

%{

#include "ParserInterface.h"
#include "LexerJson.h"
#include <stdexcept>
#include <sstream>

using ThorsAnvil::Json::ParserInterface;
using ThorsAnvil::Json::JsonValue;

using ThorsAnvil::Json::JsonMap;
using ThorsAnvil::Json::JsonArray;
using ThorsAnvil::Json::JsonMapValue;

using ThorsAnvil::Json::LexerJson;

%}

%parse-param {LexerJson&            lexer}
%parse-param {ParserInterface&      pi}
%lex-param   {LexerJson&            lexer}
%lex-param   {ParserInterface&      pi}

%union {
    JsonMap*                            jsonMap;
    JsonArray*                          jsonArray;
    JsonMapValue*                       jsonMapValue;
    JsonValue*                          jsonValue;
    std::string*                        jsonString;
    std::string*                        jsonNumber;
} 

%token  JSON_STRING
%token  JSON_NUMBER
%token  JSON_TRUE
%token  JSON_FALSE
%token  JSON_NULL

%type   <jsonValue>     JsonValue
%type   <jsonString>    JsonString
%type   <jsonNumber>    JsonNumber

%type   <jsonMap>       JsonMap
%type   <jsonMap>       JsonMapValueListOpt
%type   <jsonMap>       JsonMapValueList
%type   <jsonMapValue>  JsonMapValue
%type   <jsonString>    JsonMapKey

%type   <jsonArray>     JsonArray
%type   <jsonArray>     JsonArrayValueListOpt
%type   <jsonArray>     JsonArrayValueList
%type   <jsonArray>     JsonArrayListItem
%type   <jsonValue>     JsonArrayValue

%%

JsonData                :   JsonObject                              {return 0;}

JsonObject              :   JsonMap                                 {pi.doneMap($1);}
                        |   JsonArray                               {pi.doneAray($1);}

JsonMap                 :   '{'                                     {pi.mapOpen();}
                            JsonMapValueListOpt
                            '}'                                     {pi.mapClose();$$=$3;}
JsonMapValueListOpt     :                                           {$$ = pi.mapCreate();}
                        |   JsonMapValueList                        {$$ = $1;}
JsonMapValueList        :   JsonMapValue                            {$$ = pi.mapCreate($1);}
                        |   JsonMapValueList ',' JsonMapValue       {$$ = pi.mapAppend($1, $3);}
JsonMapValue            :   JsonMapKey JsonValue                    {$$ = pi.mapCreateElement($1, $2);}
JsonMapKey              :   JsonString ':'                          {$$ = pi.mapKeyNote($1);}


JsonArray               :   '['                                     {pi.arrayOpen();}
                            JsonArrayValueListOpt
                            ']'                                     {pi.arrayClose();$$=$3;}
JsonArrayValueListOpt   :                                           {$$ = pi.arrayCreate();}
                        |   JsonArrayValueList                      {$$ = $1;}
JsonArrayValueList      :   JsonArrayValue                          {$$ = pi.arrayCreate($1);}
                        |   JsonArrayListItem JsonArrayValue        {$$ = pi.arrayAppend($1, $2);}
JsonArrayListItem       :   JsonArrayValueList ','                  {$$ = pi.arrayNote($1);}
JsonArrayValue          :   JsonValue                               {$$ = pi.arrayCreateElement($1);}

JsonValue               :   JsonMap                                 {$$ = pi.valueParseMap($1);}
                        |   JsonArray                               {$$ = pi.valueParseArray($1);}
                        |   JsonString                              {$$ = pi.valueParseString($1);}
                        |   JsonNumber                              {$$ = pi.valueParseNumber($1);}
                        |   JsonTrue                                {$$ = pi.valueParseBool(true);}
                        |   JsonFalse                               {$$ = pi.valueParseBool(false);}
                        |   JsonNull                                {$$ = pi.valueParseNULL();}

JsonString              :   JSON_STRING                             {$$ = pi.getStringLexer(lexer);}
JsonNumber              :   JSON_NUMBER                             {$$ = pi.getNumberLexer(lexer);}
JsonTrue                :   JSON_TRUE
JsonFalse               :   JSON_FALSE
JsonNull                :   JSON_NULL


%%

int ThorsAnvil::Json::yylex(void*, LexerJson& lexer, ParserInterface& pi)
{
    return lexer.yylex(pi);
}

void yy::ParserShiftReduce::error(yy::location const&, std::string const& msg)
{
    std::string         lastToken(lexer.YYText(), lexer.YYText() + lexer.YYLeng());
    std::stringstream  extended;
    extended << msg << " -> Last Token: " << lastToken << " At line: " << lexer.lineno();

    throw ThorsAnvil::Json::ParsingError(extended.str());
}

