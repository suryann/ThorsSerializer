
#ifndef THORSANVIL_JSON_PARSER_SCANNER_DOM_H
#define THORSANVIL_JSON_PARSER_SCANNER_DOM_H

#include "ParserInterface.h"


namespace ThorsAnvil
{
    namespace Json
    {

        class ScannerDom
        {
            SMART_OWNED_PTR<JsonMap>      map;
            SMART_OWNED_PTR<JsonArray>    array;
            JsonObjectType                objectType;

            public:
            template<typename Parser>
            JsonObjectType parse(std::istream& stream);
            SMART_OWNED_PTR<JsonMap>&     getMap()       { return map;}
            SMART_OWNED_PTR<JsonArray>&   getArray()     { return array;}
        };
    }
}

#endif

