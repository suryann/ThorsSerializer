
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MAP_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_MAP_H

#include "JsonSerializer.h"
#include "JsonSerializerMemory.h"

#include <map>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename SerializeInfo, typename K, typename V, bool EnablePod = boost::is_fundamental<V>::value>
class JsonMapImportAction: public ThorsAnvil::Json::SaxAction
{
    typedef std::map<K,V>       LocalType;
    typedef std::pair<K,V>      ValueType;
    LocalType&                  destination;
    ValueType                   nextValue;

    public:
        JsonMapImportAction(LocalType& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {
            boost::mpl::for_each<typename JsonSerializeTraits<ValueType>::SerializeInfo>(MPLForEachActivateItem<ValueType, ThorsAnvil::Json::ScannerSax>(parser, nextValue));
        }
        // Read fundamental type directly into the member
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const&)
        {
            destination.insert(nextValue);
        }
};
template<typename SerializeInfo, typename V>
class JsonMapImportAction<SerializeInfo, std::string, V, true>: public ThorsAnvil::Json::SaxAction
{
    std::map<std::string,V>&            destination;
    public:
        JsonMapImportAction(std::map<std::string,V>& dst)
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
class JsonMapImportAction<SerializeInfo, std::string, V, false>: public ThorsAnvil::Json::SaxAction
{
    std::map<std::string,V>&            destination;
    public:
        JsonMapImportAction(std::map<std::string,V>& dst)
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



template<typename SerializeInfo, typename K, typename V>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(std::map<K,V>& dst)
{
    return new JsonMapImportAction<SerializeInfo,K,V>(dst);
}

template<typename K, typename V>
class JsonMapAttributeAccessor
{
    public:
    void serialize(std::map<K,V> const& src, std::ostream& stream) const
    {
        if (!src.empty())
        {
            typename std::map<K,V>::const_iterator loop = src.begin();
            stream << "{\"first\":" << jsonInternalExport(loop->first) << ",\"second\":" << jsonInternalExport(loop->second) << "}";

            for(++loop; loop != src.end(); ++loop)
            {
                stream << "," << "{\"first\":" << jsonInternalExport(loop->first) << ",\"second\":" << jsonInternalExport(loop->second) << "}";
            }
        }
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(std::map<K,V>& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<typename JsonSerializeTraits<V>::SerializeInfo>(dst));
        return action;
    }
};
template<typename V>
class JsonMapAttributeAccessor<std::string, V>
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

template<typename K, typename V, typename A, typename RegisterKey>
struct JsonSerialize<std::map<K,V>, A, RegisterKey>
{
    static void activate(JsonSerializeItem<std::map<K,V>, A, RegisterKey> const& item, std::ostream& stream, std::map<K,V> const& src)
    {
        item.accessor.serialize(src, stream);
    }
};

template<typename K, typename V>
struct JsonSerializeTraits<std::map<K, V> >
{
    typedef std::map<K, V>                  LocalType;
    typedef JsonMapAttributeAccessor<K,V>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>   SerializeInfo;
    static JsonSerializeType const  type    = Array;
};

template<typename V>
struct JsonSerializeTraits<std::map<std::string, V> >
{
    typedef std::map<std::string, V>                  LocalType;
    typedef JsonMapAttributeAccessor<std::string,V>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericMapAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>   SerializeInfo;
    static JsonSerializeType const  type    = Map;
};


        }
    }
}

#endif




