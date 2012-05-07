
#ifndef  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H
#define  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H

#include "ParserInterface.h"

using ThorsAnvil::Json::ParserInterface;
using ThorsAnvil::Json::JsonValue;

using ThorsAnvil::Json::JsonMap;
using ThorsAnvil::Json::JsonArray;
using ThorsAnvil::Json::JsonMapValue;

using ThorsAnvil::Json::LexerJson;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#include "ParserShiftReduce.tab.hpp"
#pragma GCC diagnostic pop

#endif

