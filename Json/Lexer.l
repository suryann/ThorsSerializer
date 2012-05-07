
%option c++
%option noyywrap
%option prefix="JsonBase"

%{

#define  IN_LEXER
#include "ParserShiftReduce.h"
#include "ParserInterface.h"

%}

DIGIT           [0-9]
DIGIT1          [1-9]
INTNUM          {DIGIT1}{DIGIT}*
FRACT           "."{DIGIT}+
FLOAT           ({INTNUM}|0){FRACT}?
EXP             [eE][+-]?{DIGIT}+
NUMBER          -?{FLOAT}{EXP}?

UNICODE         \\u[A-Fa-f0-9]{4}
ESCAPECHAR      \\["\\/bfnrt]
CHAR            [^"\\]|{ESCAPECHAR}|{UNICODE}
STRING          \"{CHAR}*\"

WHITESPACE      [ \t\n]


%%

\{              {return '{';}
\}              {return '}';}
\[              {return '[';}
\]              {return ']';}
,               {return ',';}
:               {return ':';}
true            {return yy::ParserShiftReduce::token::JSON_TRUE;}
false           {return yy::ParserShiftReduce::token::JSON_FALSE;}
null            {return yy::ParserShiftReduce::token::JSON_NULL;}
{STRING}        {return yy::ParserShiftReduce::token::JSON_STRING;}
{NUMBER}        {return yy::ParserShiftReduce::token::JSON_NUMBER;}

{WHITESPACE}    {/*IGNORE*/}
.               {throw ThorsAnvil::Json::ParsingError("Invalid Character in Lexer");}

%%




