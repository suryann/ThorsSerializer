
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MEMORY_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MEMORY_H

#include "JsonSerializer.h"

#include <memory>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename K, typename V>
struct JsonSerializeTraits<std::pair<K,V> >
{
    static JsonSerializeType const  type    = Map;
    typedef std::pair<K, V>      LocalType;
    THORSANVIL_SERIALIZE_JsonAttribute(LocalType, first);
    THORSANVIL_SERIALIZE_JsonAttribute(LocalType, second);

    typedef boost::mpl::vector<first,second>   SerializeInfo;
};



        }
    }
}

#endif




