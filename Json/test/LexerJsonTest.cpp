
#include "gtest/gtest.h"
#include "LexerJson.h"
#include "ParserInterface.h"
#include "ParserShiftReduce.tab.hpp"

#include <boost/lexical_cast.hpp>
#include <sstream>


TEST(LexerJson, EmptyStream)
{
    std::stringstream                       jsonElements("");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    ASSERT_TRUE(parser.yylex(cleanInterface) == 0);
}

TEST(LexerJson, BasicElements)
{
    std::stringstream                       jsonElements("{} [] , : true false null");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    ASSERT_TRUE(parser.yylex(cleanInterface) == '{');
    ASSERT_TRUE(parser.yylex(cleanInterface) == '}');
    ASSERT_TRUE(parser.yylex(cleanInterface) == '[');
    ASSERT_TRUE(parser.yylex(cleanInterface) == ']');
    ASSERT_TRUE(parser.yylex(cleanInterface) == ',');
    ASSERT_TRUE(parser.yylex(cleanInterface) == ':');
    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_TRUE);
    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_FALSE);
    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_NULL);
}

template<typename T>
bool validateNumber(ThorsAnvil::Json::LexerJson& parser, ThorsAnvil::Json::ParserInterface& interface, T const& value)
{
    bool parseOK   = parser.yylex(interface) == yy::ParserShiftReduce::token::JSON_NUMBER;
    if (parseOK)
    {
        parseOK = boost::lexical_cast<T>(ThorsAnvil::Json::getNumber(parser)) == value;
    }
    return parseOK;
}

TEST(LexerJson, Number)
{
    // Definition of Json number: http://www.json.org/number.gif

    std::stringstream                       jsonElements("0 0.1 0.9 0e1 0e99 0E+2 0E-3 -0 1 2.1 3.9 4e1 5e99 6E+2 7E-3 -8 9  10 21.1 32.9 43e1 54e99 65E+2 76E-3 -87 98");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, 0));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0.1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0.9));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0e1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0e99));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0e+2));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 0e-3));
    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, -0));


    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, 1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 2.1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 3.9));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 4e1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 5e99));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 6e+2));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 7e-3));
    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, -8));
    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, 9));

    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, 10));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 21.1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 32.9));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 43e1));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 54e99));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 65e+2));
    ASSERT_TRUE(validateNumber<double>(parser, cleanInterface, 76e-3));
    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, -87));
    ASSERT_TRUE(validateNumber<int>(parser, cleanInterface, 98));
}

bool validateString(ThorsAnvil::Json::LexerJson& parser, ThorsAnvil::Json::ParserInterface& interface, std::string const& value)
{
    bool parseOK   = parser.yylex(interface) == yy::ParserShiftReduce::token::JSON_STRING;
    if (parseOK)
    {
        parseOK = ThorsAnvil::Json::getString(parser) == value;
    }
    return parseOK;
}
TEST(LexerJson, String)
{
    std::stringstream                       jsonElements("\"Text\" \"\\\"\"  \"\\\\\" \"\\/\" \"\\b\" \"\\f\" \"\\n\" \"\\r\" \"\\t\" \"\\u0020\" \"\\u00a0\" \"\\u1Fd5\" \"\\uD834\\uDD1E\"");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;


    ASSERT_TRUE(validateString(parser, cleanInterface, "Text"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\""));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\\"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "/"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\b"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\f"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\n"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\r"));
    ASSERT_TRUE(validateString(parser, cleanInterface, "\t"));


    /*
     * The Unicode code point 002O is encoded in UTF-8 as 20
     * see http://www.fileformat.info/info/unicode/char/20/index.htm
     */
    ASSERT_TRUE(validateString(parser, cleanInterface, "\x020"));


    /*
     * The Unicode code point 00AO is encoded in UTF-8 as C2 A0
     * see http://www.fileformat.info/info/unicode/char/A0/index.htm
     */
    ASSERT_TRUE(validateString(parser, cleanInterface, "\x0c2\x0a0"));

    /*
     * The Unicode code point 1FD5 is encoded in UTF-8 as E1 BF 95
     * see http://www.fileformat.info/info/unicode/char/1FD5/index.htm
     */
    ASSERT_TRUE(validateString(parser, cleanInterface, "\x0e1\x0bf\x095"));

    /*
     * The unicode code point 1D11E is encoded in UTF-8 as 0xF0 0x9D 0x84 0x9E
     * Note it is encode in Json as UTF-16 surrogate pair 0xD834 0xDD1E
     * http://www.fileformat.info/info/unicode/char/1D11E/index.htm
     */
    ASSERT_TRUE(validateString(parser, cleanInterface, "\x0F0\x09D\x084\x09E"));

}

TEST(LexerJson, InvalidCharacter)
{
    std::stringstream                       jsonElements("\\ ");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    // Only valid chaacters are "{}[]:, true false null <number> <string>"
    // Find '\' character so throw
    ASSERT_THROW(parser.yylex(cleanInterface), ThorsAnvil::Json::ParsingError);
}

TEST(LexerJson, InvalidSlash)
{
    std::stringstream                       jsonElements("\"\\ \"");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    // Inside a string all characters are valid.
    // But a '\' must be followed by a valid escape code
    ASSERT_THROW(parser.yylex(cleanInterface), ThorsAnvil::Json::ParsingError);
}

TEST(LexerJson, InvalidUnicode)
{
    std::stringstream                       jsonElements("\"\\uG \"");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;

    ASSERT_THROW(parser.yylex(cleanInterface), ThorsAnvil::Json::ParsingError);
}

TEST(LexerJson, InvalidUnicodeSurogatePair)
{
    std::stringstream                       jsonElements("\"\\uD801 \" \"\\uD801\\n\" \"\\uD801\\u0020\"");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);
    ThorsAnvil::Json::ParserCleanInterface  cleanInterface;


    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_STRING);
    ASSERT_THROW(ThorsAnvil::Json::getString(parser), ThorsAnvil::Json::ParsingError);

    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_STRING);
    ASSERT_THROW(ThorsAnvil::Json::getString(parser), ThorsAnvil::Json::ParsingError);


    ASSERT_TRUE(parser.yylex(cleanInterface) == yy::ParserShiftReduce::token::JSON_STRING);
    ASSERT_THROW(ThorsAnvil::Json::getString(parser), ThorsAnvil::Json::ParsingError);
}

TEST(LexerJson, ValidateError)
{
    std::stringstream                       jsonElements("");
    ThorsAnvil::Json::LexerJson             parser(jsonElements);

    ASSERT_THROW(parser.LexerError("Test Failure"), ThorsAnvil::Json::ParsingError);
}

