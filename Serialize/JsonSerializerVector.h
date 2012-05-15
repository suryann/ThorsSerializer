
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

template<typename T>
struct ContainerTraits<std::vector<T> >
{
    static bool const isConstContainer  = false;
    typedef     T               DataType;
};


template<typename T>
struct JsonSerializeTraits<std::vector<T> >
{
    typedef std::vector<T>                              LocalType;
    typedef void                                        ParentType;
    static JsonSerializeType const  type    = Array;

    typedef JsonContainerAttributeAccessor<LocalType>   Accessor;
    THORSANVIL_SERIALIZE_JsonGenericArrAttributeAccess(Accessor);
    typedef boost::mpl::vector<genericAccessor>         SerializeInfo;
};



        }
    }
}

#endif




