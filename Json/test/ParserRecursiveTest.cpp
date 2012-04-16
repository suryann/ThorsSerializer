
#include "gtest/gtest.h"
#include "ParserRecursive.h"
#include "ParserInterface.h"
#include "LexerJson.h"
#include <sstream>

TEST(ParserRecursive, EmptyMap)
{
    std::stringstream                       json("{}");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, EmptyArray)
{
    std::stringstream                       json("[]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithNumber)
{
    std::stringstream                       json("[ 1 ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithString)
{
    std::stringstream                       json("[ \"String\" ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithBoolTrue)
{
    std::stringstream                       json("[ true ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithBoolFalse)
{
    std::stringstream                       json("[ false ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithNull)
{
    std::stringstream                       json("[ null ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithArray)
{
    std::stringstream                       json("[ [] ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, ArrayWithMap)
{
    std::stringstream                       json("[ {} ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithNumber)
{
    std::stringstream                       json("{ \"item\":1 }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithString)
{
    std::stringstream                       json("{ \"item\":\"String\" }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithBoolTrue)
{
    std::stringstream                       json("{ \"item\":true }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithBoolFalse)
{
    std::stringstream                       json("{ \"item\":false }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithNull)
{
    std::stringstream                       json("{ \"item\":null }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithArray)
{
    std::stringstream                       json("{ \"item\":[] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MapWithMap)
{
    std::stringstream                       json("{ \"item\":{} }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MultiItemArray)
{
    std::stringstream                       json("[ 12, \"String\", true, false, null, { \"Item\": 1, \"Plop\": 2}, [ 1, 2] ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, MultiItemMap)
{
    std::stringstream                       json("{ \"I1\": 12, \"I2\": \"String\", \"I3\": true, \"I4\": false, \"I5\": null, \"I6\": { \"Item\": 1, \"Plop\": 2}, \"I7\": [ 1, 2] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserRecursive, BadMapValue)
{
    std::stringstream                       json("{ \"I1\": ] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserRecursive, BadArrayValue)
{
    std::stringstream                       json("{ ] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserRecursive, BadMapMissingComma)
{
    std::stringstream                       json("{ \"I1\": 12 \"I2\": 13 }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserRecursive, BadMapMissingColon)
{
    std::stringstream                       json("{ \"I1\" 12 }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserRecursive, BadArrayMissingComma)
{
    std::stringstream                       json("[ 12 13 ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserRecursive, NotHighLevelObject)
{
    std::stringstream                       json("12");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    ThorsAnvil::Json::ParserRecursive       parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}


