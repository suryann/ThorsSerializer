
#ifndef  THORSANVIL_SERIALIZE_JSON_H
#define  THORSANVIL_SERIALIZE_JSON_H

/*
 * A convenience header for json serialization
 *
 * For normal usage of the json serialization this is the only interface you need to look at:
 *
 *      std::cout << jsonExport(myObj) << std::endl; 
 *      std::cin  >> jsonImport(myObj);
 *
 * If you need to define the interface to serialize a class see the file JsonSerialize.h
 */

#include "Serialize.h"
#include "JsonSerializer.h"
#include "JsonSerializerBasic.h"
#include "JsonSerializerMemory.h"
#include "JsonSerializerMap.h"
#include "JsonSerializerVector.h"

namespace ThorsAnvil
{
    namespace Serialize
    {

/* Helper functions */
template<typename T>
Importer<T, typename Json::JsonSerializer::template Parser<T> >   jsonImport(T& object)
{
    return importObj<Json::JsonSerializer, T>(object);
}

template<typename T>
Exporter<T, typename Json::JsonSerializer::template Printer<T> >  jsonExport(T const& object)
{
    return exportObj<Json::JsonSerializer, T>(object);
}

    }
}

#endif

