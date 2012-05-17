

#include "gtest/gtest.h"
#include "JsonUtil.h"
#include "JsonDom.h"
#include "ParserShiftReduce.h"
#include "ScannerDom.h"

using namespace ThorsAnvil::Json;

TEST(JsonUtil, MergeArrays)
{
    ScannerDom      scanner;

    std::stringstream   data1("[1, 2, 3, 4]");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonArray>    array1 = SMART_OWNED_MOVE(scanner.getArray());

    std::stringstream   data2("[1, 2, 3, 4]");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonArray>    array2 = SMART_OWNED_MOVE(scanner.getArray());

    mergeJsonDom(*array1, *array2, "Test");
    EXPECT_EQ(8, array1->size());
}


TEST(JsonUtil, MergeMaps)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 1, \"item2\": 2, \"item3\": 3, \"item4\": 4}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item5\": 1, \"item6\": 2, \"item7\": 3, \"item8\": 4}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    mergeJsonDom(*map1, *map2, "Test");
    EXPECT_EQ(8, map1->size());
}


TEST(JsonUtil, OverwiteMapPODElement)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 4}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item1\": 5}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    mergeJsonDom(*map1, *map2, "Test");
    EXPECT_EQ(1, map1->size());
    EXPECT_EQ(5, (*map1)["item1"].getValue<int>());
}


TEST(JsonUtil, OverwiteMapElement)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"t1\": 9}}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item1\": {\"t2\": 15}}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    mergeJsonDom(*map1, *map2, "Test");
    EXPECT_EQ(1, map1->size());

    JsonMap&    subMap  = *static_cast<JsonMapItem&>((*map1)["item1"]).value;
    EXPECT_EQ(9, (subMap)["t1"].getValue<int>());
    EXPECT_EQ(15, (subMap)["t2"].getValue<int>());
}


TEST(JsonUtil, AddElementsToArrayInMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item1\": {\"item5\": 34}}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    mergeJsonDom(*map1, *map2, "Test");
    EXPECT_EQ(1, map1->size());

    JsonArray&    subArray  = *static_cast<JsonArrayItem&>((*map1)["item1"]).value;
    EXPECT_EQ(4, subArray.size());
}


TEST(JsonUtil, AddArrayToArrayInMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item1\": [ 4, 5, {\"plop\": 8}]}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    mergeJsonDom(*map1, *map2, "Test");
    EXPECT_EQ(1, map1->size());

    JsonArray&    subArray  = *static_cast<JsonArrayItem&>((*map1)["item1"]).value;
    EXPECT_EQ(6, subArray.size());
}


TEST(JsonUtil, MergeNonMapIntoMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"plop\": 1}}");
    scanner.parse<yy::ParserShiftReduce>(data1);
    SMART_OWNED_PTR<JsonMap>    map1 = SMART_OWNED_MOVE(scanner.getMap());

    std::stringstream   data2("{ \"item1\": 12}");
    scanner.parse<yy::ParserShiftReduce>(data2);
    SMART_OWNED_PTR<JsonMap>    map2 = SMART_OWNED_MOVE(scanner.getMap());

    ASSERT_THROW(mergeJsonDom(*map1, *map2, "Test"), std::runtime_error);
}



