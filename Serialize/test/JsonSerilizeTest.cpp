
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
    std::string result  = testAction<std::map<std::string, int> >("{}");
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
    std::string result  = testAction<EmptyJsonClass>("{}");
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
    std::string result  = testAction<OneMemberJsonClass>("{ \"value\": 15}");
    ValidateSerializedStrings(input, result);
}


