

#include "ScannerSax.h"
#include "LexerJson.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"

#include <sstream>
#include <list>

namespace ThorsAnvil
{
    namespace Json
    {

class ScannerSaxInterface: public ParserCleanInterface
{
    std::list<int> currentArrayIndex;
    ScannerSax&     parent;
    public:
    ScannerSaxInterface(ScannerSax& p): parent(p)   {}
    virtual void            mapOpen()                                           { parent.push_mapAction();}
    virtual void            mapClose()                                          { parent.pop_mapAction();}
    virtual std::string*    mapKeyNote(std::string* k)                          { SMART_OWNED_PTR<std::string> ak(k);                                         parent.preActivate(*ak);     return ak.release();}
    virtual JsonMapValue*   mapCreateElement(std::string* k,JsonValue* val)     { SMART_OWNED_PTR<std::string> ak(k);   SMART_OWNED_PTR<JsonValue> aval(val); parent.activate(*ak, *aval); return NULL;}
    virtual void            arrayOpen()                                         { currentArrayIndex.push_front(0); parent.push_arrAction(); parent.preActivate(currentArrayIndex.front());}
    virtual void            arrayClose()                                        { currentArrayIndex.pop_front();   parent.pop_arrAction();}
    virtual JsonArray*      arrayNote(JsonArray* arr)                           { SMART_OWNED_PTR<JsonArray>   aarr(arr); parent.preActivate(currentArrayIndex.front());return NULL;}
    virtual JsonValue*      arrayCreateElement(JsonValue* val)                  { SMART_OWNED_PTR<JsonValue>   aval(val); parent.activate(currentArrayIndex.front()++, *aval);return NULL;}
    virtual JsonValue*      valueParseString(std::string* str)                  { SMART_OWNED_PTR<std::string> astr(str); return new JsonStringItem(astr);}
    virtual JsonValue*      valueParseNumber(std::string* num)                  { SMART_OWNED_PTR<std::string> anum(num); return new JsonNumberItem(anum);}
    virtual JsonValue*      valueParseBool(bool val)                            {                                         return new JsonBoolItem(val);}
    virtual JsonValue*      valueParseNULL()                                    {                                         return new JsonNULLItem();}
    virtual std::string*    getStringLexer(LexerJson& lexer)                    {                                         return new std::string(getString(lexer));}
    virtual std::string*    getNumberLexer(LexerJson& lexer)                    {                                         return new std::string(getNumber(lexer));}
};
    }
}

using namespace ThorsAnvil::Json;

ScannerSax::ScannerSax()
    : mapActions(1, ScannerMapActionMap())
    , arrActions(1, ScannerArrActionMap())
    , mapAction(mapActions.begin())
    , arrAction(arrActions.begin())
{
    mapActions.push_front(ScannerMapActionMap());
    arrActions.push_front(ScannerArrActionMap());
}

ScannerSax::~ScannerSax()
{
}

template<typename Parser>
void ScannerSax::parse(std::istream& src)
{
    ScannerSaxInterface     scanner(*this);
    LexerJson               lexer(src);
    Parser                  parser(lexer, scanner);

    parser.parse();
}

ActionRefNote ScannerSax::registerActionOnAllMapItems(SMART_OWNED_PTR<SaxAction> action)
{
    // \xFF is an invalid UTF-8 character
    // The parser will never generate mapItem of this string
    return registerAction("\xFF", SMART_OWNED_MOVE(action));
}
ActionRefNote ScannerSax::registerAction(std::string const& mapItem, SMART_OWNED_PTR<SaxAction> action)
{
    SaxAction*&  location    = mapActions.front()[mapItem];
    SMART_OWNED_PTR<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}

ActionRefNote ScannerSax::registerActionOnAllArrItems(SMART_OWNED_PTR<SaxAction> action)
{
    SaxAction*&  location    = arrActions.front().map[-1];
    SMART_OWNED_PTR<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}
ActionRefNote ScannerSax::registerActionNext(SMART_OWNED_PTR<SaxAction> action)
{
    int          index       = arrActions.front().max++;
    SaxAction*&  location    = arrActions.front().map[index];
    SMART_OWNED_PTR<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}

void ScannerSax::replaceAction(ActionRefNote oldActionRef, SMART_OWNED_PTR<SaxAction> action)
{
    SaxAction*& location    = *reinterpret_cast<SaxAction**>(oldActionRef);
    SMART_OWNED_PTR<SaxAction>  oldLocation(location);
    location                = action.release();
}

SaxAction* ScannerSax::getAction(std::string const& mapItem)
{
    SaxAction*                          action  = NULL;
    ScannerMapActionMap::const_iterator find1;
    ScannerMapActionMap::const_iterator find2;
    if ((find1 = mapAction->find(mapItem)) != mapAction->end())
    {
        action  = find1->second;
    }
    else if ((find2 = mapAction->find("\xFF")) != mapAction->end())
    {
        action  = find2->second;
    }
    return action;
}
SaxAction* ScannerSax::getAction(int index)
{
    SaxAction*                          action  = NULL;
    ScannerArrActionMap::const_iterator find;
    if ((find = arrAction->find(index)) != arrAction->end())
    {
        action  = find->second;
    }
    else if ((find = arrAction->find(-1)) != arrAction ->end())
    {
        action  = find->second;
    }
    return action;
}

void ScannerSax::preActivate(std::string const& mapItem)
{
    SaxAction*  action = getAction(mapItem);
    if (action != NULL)
    {
        action->doPreAction(*this, Key(mapItem));
    }
}
void ScannerSax::preActivate(int index)
{
    SaxAction*  action = getAction(index);
    if (action != NULL)
    {
        action->doPreAction(*this, Key(index));
    }
}

void ScannerSax::activate(std::string const& mapItem, JsonValue const& value)
{
    SaxAction*  action = getAction(mapItem);
    if (action != NULL)
    {
        action->doAction(*this, Key(mapItem), value);
    }
}
void ScannerSax::activate(int index, JsonValue const& value)
{
    SaxAction*  action = getAction(index);
    if (action != NULL)
    {
        action->doAction(*this, Key(index), value);
    }
}

template void ScannerSax::parse<yy::ParserShiftReduce>(std::istream& src);
template void ScannerSax::parse<ThorsAnvil::Json::ParserRecursive>(std::istream& src);


