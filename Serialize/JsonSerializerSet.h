
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_SET_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_SET_H

#include "JsonSerializerContainer.h"
#include <set>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename T>
struct ContainerTraits<std::set<T> >
{
    typedef     T               DataType;
    typedef     T               ValueType;
};


template<typename T>
struct JsonSerializeTraits<std::set<T> >
{
    static JsonSerializeType const  type    = Array;
    typedef std::set<T>                                 LocalType;
    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};



        }
    }
}

#endif




