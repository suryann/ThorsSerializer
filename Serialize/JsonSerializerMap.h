
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
    static bool const isConstContainer  = true;
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

template<typename V>
class JsonContainerAttributeAccessor<std::map<std::string, V> >
{
    public:
    void serialize(std::map<std::string,V> const& src, std::ostream& stream) const
    {
        if (!src.empty())
        {
            typename std::map<std::string,V>::const_iterator loop = src.begin();
            stream << jsonInternalExport(loop->first) << ":" << jsonInternalExport(loop->second);

            for(++loop; loop != src.end(); ++loop)
            {
                stream << "," << jsonInternalExport(loop->first) << ":" << jsonInternalExport(loop->second);
            }
        }
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(std::map<std::string,V>& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<typename JsonSerializeTraits<V>::SerializeInfo>(dst));
        return action;
    }
};

template<typename SerializeInfo, typename V>
class JsonContainerImportAction<SerializeInfo, std::map<std::string,V>, true, true>: public ThorsAnvil::Json::SaxAction
{
    std::map<std::string,V>&            destination;
    public:
        JsonContainerImportAction(std::map<std::string,V>& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {}
        // Read fundamental type directly into the member
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const& key, JsonValue const& value)
        {
            destination[key.mapKey] = value.getValue<V>();
        }
};

template<typename SerializeInfo, typename V>
class JsonContainerImportAction<SerializeInfo, std::map<std::string, V>, false, true>: public ThorsAnvil::Json::SaxAction
{
    std::map<std::string,V>&            destination;
    public:
        JsonContainerImportAction(std::map<std::string,V>& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {
            boost::mpl::for_each<SerializeInfo>(MPLForEachActivateItem<V, ThorsAnvil::Json::ScannerSax>(parser, destination[key.mapKey]));
        }
        // Read fundamental type directly into the member
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const&)
        {}
};
template<typename K, typename V>
struct JsonSerializeTraits<std::map<K, V> >
{
    static JsonSerializeType const  type    = Array;

    typedef std::map<K, V>                              LocalType;
    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};

template<typename V>
struct JsonSerializeTraits<std::map<std::string, V> >
{
    static JsonSerializeType const  type    = Map;

    typedef std::map<std::string, V>                    LocalType;
    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericMapAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};


        }
    }
}

#endif




