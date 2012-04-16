

#include "gtest/gtest.h"
#include "ScannerDom.h"
#include "ParserShiftReduce.tab.hpp"
#include "ParserRecursive.h"
#include <sstream>

TEST(ScannerDom, ShiftReduceScanMapEmpty)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<yy::ParserShiftReduce>(json) == ThorsAnvil::Json::JsonMapObject);

    std::auto_ptr<ThorsAnvil::Json::JsonMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, ShiftReduceScanArrayEmpty)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<yy::ParserShiftReduce>(json) == ThorsAnvil::Json::JsonArrayObject);

    std::auto_ptr<ThorsAnvil::Json::JsonArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}

TEST(ScannerDom, RecursiveScanMapEmpty)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<ThorsAnvil::Json::ParserRecursive>(json) == ThorsAnvil::Json::JsonMapObject);

    std::auto_ptr<ThorsAnvil::Json::JsonMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, RecursiveScanArrayEmpty)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<ThorsAnvil::Json::ParserRecursive>(json) == ThorsAnvil::Json::JsonArrayObject);

    std::auto_ptr<ThorsAnvil::Json::JsonArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}


TEST(ScannerDom, ShiftReduceScanMap)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<yy::ParserShiftReduce>(json) == ThorsAnvil::Json::JsonMapObject);

    std::auto_ptr<ThorsAnvil::Json::JsonMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, ShiftReduceScanArray)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<yy::ParserShiftReduce>(json) == ThorsAnvil::Json::JsonArrayObject);

    std::auto_ptr<ThorsAnvil::Json::JsonArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}

TEST(ScannerDom, RecursiveScanMap)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<ThorsAnvil::Json::ParserRecursive>(json) == ThorsAnvil::Json::JsonMapObject);

    std::auto_ptr<ThorsAnvil::Json::JsonMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, RecursiveScanArray)
{
    ThorsAnvil::Json::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<ThorsAnvil::Json::ParserRecursive>(json) == ThorsAnvil::Json::JsonArrayObject);

    std::auto_ptr<ThorsAnvil::Json::JsonArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}


