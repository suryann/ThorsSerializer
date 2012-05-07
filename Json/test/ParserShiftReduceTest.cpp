
#include "gtest/gtest.h"
#include "ParserShiftReduce.h"
#include "ParserInterface.h"
#include "LexerJson.h"
#include <sstream>

TEST(ParserShiftReduce, EmptyMap)
{
    std::stringstream                       json("{}");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, EmptyArray)
{
    std::stringstream                       json("[]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithNumber)
{
    std::stringstream                       json("[ 1 ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithString)
{
    std::stringstream                       json("[ \"String\" ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithBoolTrue)
{
    std::stringstream                       json("[ true ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithBoolFalse)
{
    std::stringstream                       json("[ false ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithNull)
{
    std::stringstream                       json("[ null ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithArray)
{
    std::stringstream                       json("[ [] ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, ArrayWithMap)
{
    std::stringstream                       json("[ {} ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithNumber)
{
    std::stringstream                       json("{ \"item\":1 }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithString)
{
    std::stringstream                       json("{ \"item\":\"String\" }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithBoolTrue)
{
    std::stringstream                       json("{ \"item\":true }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithBoolFalse)
{
    std::stringstream                       json("{ \"item\":false }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithNull)
{
    std::stringstream                       json("{ \"item\":null }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithArray)
{
    std::stringstream                       json("{ \"item\":[] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MapWithMap)
{
    std::stringstream                       json("{ \"item\":{} }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MultiItemArray)
{
    std::stringstream                       json("[ 12, \"String\", true, false, null, { \"Item\": 1, \"Plop\": 2}, [ 1, 2] ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, MultiItemMap)
{
    std::stringstream                       json("{ \"I1\": 12, \"I2\": \"String\", \"I3\": true, \"I4\": false, \"I5\": null, \"I6\": { \"Item\": 1, \"Plop\": 2}, \"I7\": [ 1, 2] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(ParserShiftReduce, BadMapValue)
{
    std::stringstream                       json("{ \"I1\": ] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserShiftReduce, BadArrayValue)
{
    std::stringstream                       json("{ ] }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserShiftReduce, BadMapMissingComma)
{
    std::stringstream                       json("{ \"I1\": 12 \"I2\": 13 }");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserShiftReduce, BadArrayMissingComma)
{
    std::stringstream                       json("[ 12 13 ]");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}

TEST(ParserShiftReduce, NotHighLevelObject)
{
    std::stringstream                       json("12");
    ThorsAnvil::Json::LexerJson             lexer(json);
    ThorsAnvil::Json::ParserCleanInterface  interface;
    yy::ParserShiftReduce                   parser(lexer, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Json::ParsingError);
}




