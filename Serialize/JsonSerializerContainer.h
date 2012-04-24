
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

template<typename SerializeInfo, typename C, bool EnablePod = boost::is_fundamental<typename ContainerTraits<C>::DataType>::value, bool isConstContainer = ContainerTraits<C>::isConstContainer>
class JsonContainerImportAction;

template<typename SerializeInfo, typename C>
class JsonContainerImportAction<SerializeInfo, C, false, true>: public ThorsAnvil::Json::SaxAction
{
    typedef C                                       LocalType;
    typedef typename ContainerTraits<C>::ValueType  ValueType;
    LocalType&                  destination;
    ValueType                   nextValue;

    public:
        JsonContainerImportAction(LocalType& dst)
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
template<typename SerializeInfo, typename C>
class JsonContainerImportAction<SerializeInfo, C, true, true>: public ThorsAnvil::Json::SaxAction
{
    C&            destination;
    public:
        JsonContainerImportAction(C& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {}
        // Read fundamental type directly into the member
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const& key, JsonValue const& value)
        {
            destination.insert(value.getValue<typename ContainerTraits<C>::DataType>());
        }
};
template<typename SerializeInfo, typename C>
class JsonContainerImportAction<SerializeInfo, C, false, false>: public ThorsAnvil::Json::SaxAction
{
    typedef C                                       LocalType;
    typedef typename ContainerTraits<C>::DataType   DataType;
    LocalType&      destination;

    public:
        JsonContainerImportAction(LocalType& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const& key)
        {
            destination.push_back(DataType());
            boost::mpl::for_each<typename JsonSerializeTraits<DataType>::SerializeInfo>(MPLForEachActivateItem<DataType, ThorsAnvil::Json::ScannerSax>(parser, destination.back()));
        }
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const&)
        {}
};

template<typename SerializeInfo, typename C>
class JsonContainerImportAction<SerializeInfo, C, true, false>: public ThorsAnvil::Json::SaxAction
{
    typedef C                                       LocalType;
    typedef typename ContainerTraits<C>::DataType   DataType;
    LocalType&      destination;

    public:
        JsonContainerImportAction(LocalType& dst)
            : destination(dst)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&)
        {}
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const& value)
        {
            destination.push_back(value.getValue<DataType>());
        }
};


template<typename SerializeInfo, typename C>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(C& dst)
{
    return new JsonContainerImportAction<SerializeInfo,C>(dst);
}


template<typename C>
class JsonContainerAttributeAccessor
{
    public:
    void serialize(C const& src, std::ostream& stream) const
    {
        if (!src.empty())
        {
            typename C::const_iterator loop = src.begin();
            stream << jsonInternalExport(*loop);

            for(++loop; loop != src.end(); ++loop)
            {
                stream << "," << jsonInternalExport(*loop);
            }
        }
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(C& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<typename JsonSerializeTraits<typename ContainerTraits<C>::DataType>::SerializeInfo>(dst));
        return action;
    }
};

        }
    }
}

#endif




