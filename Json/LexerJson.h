

#ifndef THORS_ANVIL_JSON_PARSER_LEXER_JSON_H
#define THORS_ANVIL_JSON_PARSER_LEXER_JSON_H


#ifndef IN_LEXER
#ifdef  yyFlexLexer
#undef  yyFlexLexer
#endif
#define yyFlexLexer JsonBaseFlexLexer
#include <FlexLexer.h>
#endif

namespace ThorsAnvil
{
    namespace Json
    {
        
class ParserInterface;
class LexerJson: public JsonBaseFlexLexer
{
    public:
        LexerJson(std::istream& input);
        int yylex(ParserInterface& pi);

        virtual void LexerError(const char* msg);
};

    }
}

#endif

