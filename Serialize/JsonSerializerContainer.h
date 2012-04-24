
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_CONTAINER_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_CONTAINER_H

#include "JsonSerializer.h"

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {


template<typename C>
struct ContainerTraits;

template<typename SerializeInfo, typename C, bool EnablePod = boost::is_fundamental<typename ContainerTraits<C>::DataType>::value>
class JsonMapImportAction: public ThorsAnvil::Json::SaxAction
{
    typedef C                                       LocalType;
    typedef typename ContainerTraits<C>::ValueType  ValueType;
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
class JsonMapImportAction<SerializeInfo, std::map<std::string,V>, true>: public ThorsAnvil::Json::SaxAction
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
class JsonMapImportAction<SerializeInfo, std::map<std::string, V>, false>: public ThorsAnvil::Json::SaxAction
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



template<typename SerializeInfo, typename C>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(C& dst)
{
    return new JsonMapImportAction<SerializeInfo,C>(dst);
}

template<typename C>
class JsonMapAttributeAccessor;

template<typename K, typename V>
class JsonMapAttributeAccessor<std::map<K,V> >
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
class JsonMapAttributeAccessor<std::map<std::string, V> >
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


        }
    }
}

#endif




