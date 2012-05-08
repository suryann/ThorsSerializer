
#ifndef  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H
#define  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H

#include "ParserInterface.h"

using ThorsAnvil::Json::ParserInterface;
using ThorsAnvil::Json::JsonValue;

using ThorsAnvil::Json::JsonMap;
using ThorsAnvil::Json::JsonArray;
using ThorsAnvil::Json::JsonMapValue;

using ThorsAnvil::Json::LexerJson;

#if (__GNUG__ > 4) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#endif

#pragma GCC diagnostic ignored "-Wparentheses"


#include "ParserShiftReduce.tab.hpp"


#if (__GNUG__ > 4) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#else
#pragma GCC diagnostic error "-Wformat"
#endif

#endif

