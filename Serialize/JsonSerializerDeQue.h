
#ifndef THORSANVIL_SERIALIZE_JSON_SERIAlIZE_DEQUE_H
#define THORSANVIL_SERIALIZE_JSON_SERIAlIZE_DEQUE_H

#include "JsonSerializer.h"

#include <deque>

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

template<typename T>
struct ContainerTraits<std::deque<T> >
{
    static bool const isConstContainer  = false;
    typedef     T               DataType;
};


template<typename T>
struct JsonSerializeTraits<std::deque<T> >
{
    static JsonSerializeType const  type    = Array;

    typedef std::deque<T>                               LocalType;
    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};



        }
    }
}

#endif




