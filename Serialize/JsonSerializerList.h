
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_LIST_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_LIST_H

#include "JsonSerializer.h"

#include <list>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename T>
struct ContainerTraits<std::list<T> >
{
    static bool const isConstContainer  = false;
    typedef     T               DataType;
};


template<typename T>
struct JsonSerializeTraits<std::list<T> >
{
    static JsonSerializeType const  type    = Array;

    typedef std::list<T>                                LocalType;
    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(LocalType, Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};



        }
    }
}

#endif




