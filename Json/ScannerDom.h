
#ifndef THORSANVIL_JSON_PARSER_SCANNER_DOM_H
#define THORSANVIL_JSON_PARSER_SCANNER_DOM_H

#include "ParserInterface.h"


namespace ThorsAnvil
{
    namespace Json
    {

        class ScannerDom
        {
            std::auto_ptr<JsonMap>      map;
            std::auto_ptr<JsonArray>    array;
            JsonObjectType              objectType;

            public:
            template<typename Parser>
            JsonObjectType parse(std::istream& stream);
            std::auto_ptr<JsonMap>&      getMap()       { return map;}
            std::auto_ptr<JsonArray>&    getArray()     { return array;}
        };
    }
}

#endif

