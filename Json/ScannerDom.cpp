
#include "ScannerDom.h"
#include "LexerJson.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"

using namespace ThorsAnvil::Json;

class ScannerDomInterface: public ParserDomInterface
{
    JsonObjectType&               objectTypeRef;
    SMART_OWNED_PTR<JsonMap>&     mapRef;
    SMART_OWNED_PTR<JsonArray>&   arrayRef;
    public:
    ScannerDomInterface(JsonObjectType& objectType, SMART_OWNED_PTR<JsonMap>&  map, SMART_OWNED_PTR<JsonArray>& array)
        : objectTypeRef(objectType)
        , mapRef(map)
        , arrayRef(array)
    {}

    virtual void            doneMap(JsonMap* map)           { mapRef.reset(map);    objectTypeRef = JsonMapObject; }
    virtual void            doneAray(JsonArray* array)      { arrayRef.reset(array);objectTypeRef = JsonArrayObject; }
};

template<typename Parser>
JsonObjectType ScannerDom::parse(std::istream& stream)
{
    map.release();
    array.release();

    ScannerDomInterface     scanner(objectType, map, array);
    LexerJson               lexer(stream);
    Parser                  parser(lexer, scanner);

    // If this fails it throws.
    parser.parse();

    return objectType;
}

template JsonObjectType ScannerDom::parse<yy::ParserShiftReduce>(std::istream& stream);
template JsonObjectType ScannerDom::parse<ThorsAnvil::Json::ParserRecursive>(std::istream& stream);


