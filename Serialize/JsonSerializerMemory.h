
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
    typedef std::pair<K, V>                   LocalType;
    typedef void                              ParentType;
    static JsonSerializeType const  type    = Map;

    THORSANVIL_SERIALIZE_JsonAttribute(first);
    THORSANVIL_SERIALIZE_JsonAttribute(second);

    typedef boost::mpl::vector<first,second>   SerializeInfo;
};



        }
    }
}

#endif




