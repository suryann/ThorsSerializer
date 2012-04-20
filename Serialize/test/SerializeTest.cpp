
#include "gtest/gtest.h"
#include "Serialize.h"
#include "JsonSerializer.h"

#include <map>

template<typename T>
std::string testAction(std::string const& expected)
{
    T                   object;

    std::stringstream   input(expected);
    input >> ThorsAnvil::Serialize::importObj<ThorsAnvil::Serialize::Json::JsonSerializer>(object);

    std::stringstream   output;
    output << ThorsAnvil::Serialize::exportObj<ThorsAnvil::Serialize::Json::JsonSerializer>(object);

    return output.str();
}

void ValidateSerializedStrings(std::string lhs, std::string rhs)
{
    lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
    rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());

    ASSERT_EQ(lhs, rhs);
}


TEST(Serialize, JsonMap)
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

TEST(Serialize, EmptyJsonClass)
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

TEST(Serialize, OneMemberJsonClass)
{
    std::string input   = "{\"value\": 15}";
    std::string result  = testAction<OneMemberJsonClass>("{ \"value\": 15}");
    ValidateSerializedStrings(input, result);
}


