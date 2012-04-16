#include "JsonSerializer.h"


template<>
char ThorsAnvil::Serialize::Json::JsonSerializeBrace<ThorsAnvil::Serialize::Json::Invalid>::braces[]= "  ";

template<>
char ThorsAnvil::Serialize::Json::JsonSerializeBrace<ThorsAnvil::Serialize::Json::Map>::braces[]    = "{}";

template<>
char ThorsAnvil::Serialize::Json::JsonSerializeBrace<ThorsAnvil::Serialize::Json::Array>::braces[]  = "[]";


