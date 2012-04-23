
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_VECTOR_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_VECTOR_H

#include "JsonSerializer.h"

#include <vector>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename SerializeInfo, typename T, bool EnablePod = boost::is_fundamental<T>::value>
class JsonArrImportAction: public ThorsAnvil::Json::SaxAction
{
    typedef std::vector<T>      LocalType;
    LocalType&                  destination;

    public:
        JsonArrImportAction(LocalType& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&)
        {}
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const& value)
        {
            destination.push_back(value.getValue<T>());
        }
};

template<typename T, typename A, typename RegisterKey>
struct JsonSerialize<std::vector<T>, A, RegisterKey>
{
    static void activate(JsonSerializeItem<std::vector<T>, A, RegisterKey> const& item, std::ostream& stream, std::vector<T> const& src)
    {
        item.accessor.serialize(src, stream);
    }
};


template<typename SerializeInfo, typename T>
class JsonArrImportAction<SerializeInfo, T, false>: public ThorsAnvil::Json::SaxAction
{
    typedef std::vector<T>      LocalType;
    LocalType&                  destination;

    public:
        JsonArrImportAction(LocalType& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {
            destination.push_back(T());
            boost::mpl::for_each<typename JsonSerializeTraits<T>::SerializeInfo>(MPLForEachActivateItem<T, ThorsAnvil::Json::ScannerSax>(parser, destination.back()));
        }
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const&)
        {}
};
template<typename SerializeInfo, typename T>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(std::vector<T>& dst)
{
    return new JsonArrImportAction<SerializeInfo,T>(dst);
}

template<typename T>
class JsonArrAttributeAccessor
{
    public:
    void serialize(std::vector<T> const& src, std::ostream& stream) const
    {
        if (!src.empty())
        {
            typename std::vector<T>::const_iterator loop = src.begin();
            stream << jsonInternalExport(*loop);

            for(++loop; loop != src.end(); ++loop)
            {
                stream << "," << jsonInternalExport(*loop);
            }
        }
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(std::vector<T>& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<typename JsonSerializeTraits<T>::SerializeInfo>(dst));
        return action;
    }
};

template<typename T>
struct JsonSerializeTraits<std::vector<T> >
{
    typedef std::vector<T>      LocalType;
    typedef JsonArrAttributeAccessor<T>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>   SerializeInfo;
    static JsonSerializeType const  type    = Array;
};



        }
    }
}

#endif




