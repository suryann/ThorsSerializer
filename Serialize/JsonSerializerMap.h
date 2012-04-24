
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MAP_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MAP_H

#include "JsonSerializer.h"
#include "JsonSerializerMemory.h"
#include "JsonSerializerContainer.h"

#include <map>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename K, typename V>
struct ContainerTraits<std::map<K,V> >
{
    typedef     V               DataType;
    typedef     std::pair<K,V>  ValueType;
};

template<typename V, typename A, typename RegisterKey>
struct JsonSerialize<std::map<std::string,V>, A, RegisterKey, Map>
{
    static void activate(JsonSerializeItem<std::map<std::string,V>, A, RegisterKey> const& item, std::ostream& stream, std::map<std::string,V> const& src)
    {
        item.accessor.serialize(src, stream);
    }
};

template<typename K, typename V>
struct JsonSerializeTraits<std::map<K, V> >
{
    static JsonSerializeType const  type    = Array;

    typedef std::map<K, V>                          LocalType;
    typedef JsonMapAttributeAccessor<LocalType>     Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>     SerializeInfo;
};

template<typename V>
struct JsonSerializeTraits<std::map<std::string, V> >
{
    static JsonSerializeType const  type    = Map;

    typedef std::map<std::string, V>                LocalType;
    typedef JsonMapAttributeAccessor<LocalType>     Accessor;
    THORSANVIL_SERIALIZE_JsonGenericMapAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>     SerializeInfo;
};


        }
    }
}

#endif




