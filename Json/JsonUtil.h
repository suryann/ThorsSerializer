
#ifndef THORSANVIL_JSON_JSON_UTIL_H
#define THORSANVIL_JSON_JSON_UTIL_H

#include "JsonDom.h"

namespace ThorsAnvil
{
    namespace Json
    {

void mergeJsonDom(JsonMap&   dst, JsonMap&   src, std::string const& errorMsg);
void mergeJsonDom(JsonArray& dst, JsonArray& src, std::string const& errorMsg);

    }
}


#endif

