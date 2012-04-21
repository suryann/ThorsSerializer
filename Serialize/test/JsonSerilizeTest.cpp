
#include "gtest/gtest.h"
#include "json.h"

#include <map>

using ThorsAnvil::Serialize::jsonImport;
using ThorsAnvil::Serialize::jsonExport;


template<typename T>
std::string testAction(std::string const& expected)
{
    T                   object;

    std::stringstream   input(expected);
    input  >> jsonImport(object);

    std::stringstream   output;
    output << jsonExport(object);

    return output.str();
}

void ValidateSerializedStrings(std::string lhs, std::string rhs)
{
    lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
    rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());

    ASSERT_EQ(lhs, rhs);
}


TEST(JsonSerialize, JsonMap)
{
    std::string input   = "{}";
    std::string result  = testAction<std::map<std::string, int> >(input);
    ValidateSerializedStrings(input, result);
}

class EmptyJsonClass
{};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<EmptyJsonClass>
{
    typedef void                  SerializeInfo;
    static JsonSerializeType const  type    = Map;
};
}}}

TEST(JsonSerialize, EmptyJsonClass)
{
    std::string input   = "{}";
    std::string result  = testAction<EmptyJsonClass>(input);
    ValidateSerializedStrings(input, result);
}

class OneMemberJsonClass
{
public:
    int value;
    friend class ThorsAnvil::Serialize::Json::JsonSerializeTraits<OneMemberJsonClass>;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<OneMemberJsonClass>
{
    THORSANVIL_SERIALIZE_JsonAttribute(OneMemberJsonClass, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, OneMemberJsonClass)
{
    std::string input   = "{\"value\": 15}";
    std::string result  = testAction<OneMemberJsonClass>(input);
    ValidateSerializedStrings(input, result);
}

struct NestedJsonTest
{
    OneMemberJsonClass  nested;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<NestedJsonTest>
{
    THORSANVIL_SERIALIZE_JsonAttribute(NestedJsonTest, nested);
    typedef boost::mpl::vector<nested>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, NestedJsonTest)
{
    std::string input   = "{\"nested\": {\"value\": 105}}";
    std::string result  = testAction<NestedJsonTest>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestFloat
{
    float value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestFloat>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestFloat, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestFloat)
{
    std::string input   = "{\"value\": 1234.45}";
    std::string result  = testAction<TestFloat>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestDouble
{
    double value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestDouble>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestDouble, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestDouble)
{
    std::string input   = "{\"value\": 1234.45}";
    std::string result  = testAction<TestDouble>(input);
    ValidateSerializedStrings(input, result);
}


// #####

struct TestShort
{
    short value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestShort>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestShort, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestShort)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestShort>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestInt
{
    int value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestInt>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestInt, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestInt)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestInt>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestLong
{
    long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestLong>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestLong>(input);
    ValidateSerializedStrings(input, result);
}


// #####
#if 0
struct TestLongLong
{
    long long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestLongLong>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestLongLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestLongLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestLongLong>(input);
    ValidateSerializedStrings(input, result);
}
#endif

// #####

struct TestUnsignedShort
{
    unsigned short value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedShort>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedShort, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestUnsignedShort)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedShort>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestUnsignedInt
{
    unsigned int value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedInt>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedInt, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestUnsignedInt)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedInt>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestUnsignedLong
{
    unsigned long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedLong>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestUnsignedLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedLong>(input);
    ValidateSerializedStrings(input, result);
}


// #####
#if 0
struct TestUnsignedLongLong
{
    unsigned long long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedLongLong>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedLongLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestUnsignedLongLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedLongLong>(input);
    ValidateSerializedStrings(input, result);
}
#endif


// #####

struct TestBool
{
    bool value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestBool>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestBool, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestBoolTrue)
{
    std::string input   = "{\"value\": true}";
    std::string result  = testAction<TestBool>(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, TestBoolFalse)
{
    std::string input   = "{\"value\": false}";
    std::string result  = testAction<TestBool>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestString
{
    std::string value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestString>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestString, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestString)
{
    std::string input   = "{\"value\": \"Hi There this is a string\"}";
    std::string result  = testAction<TestString>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestPointer
{
    TestString*     value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestPointer>
{
    THORSANVIL_SERIALIZE_JsonAttribute(TestPointer, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
    static  JsonSerializeType const       type    = Map;
};
}}}

TEST(JsonSerialize, TestPointerNULL)
{
    std::string input   = "{\"value\": null }";
    std::string result  = testAction<TestPointer>(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, TestPointerNotNull)
{
    std::string input   = "{\"value\": {\"value\":\"plop\" }}";
    std::string result  = testAction<TestPointer>(input);
    ValidateSerializedStrings(input, result);
}


