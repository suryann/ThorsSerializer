
#include "LexerJson.h"
#include "ParserInterface.h"

using namespace ThorsAnvil::Json;

LexerJson::LexerJson(std::istream& input)
    : JsonBaseFlexLexer(&input, &std::cerr)
{}

int LexerJson::yylex(ParserInterface& pi)
{
    return pi.lexResult(JsonBaseFlexLexer::yylex());
}

void LexerJson::LexerError(const char* msg)
{
    throw ParsingError(msg);
}

