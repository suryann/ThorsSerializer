
#ifndef THORSANVIL_JSON_PARSER_SCANNER_SAX_H
#define THORSANVIL_JSON_PARSER_SCANNER_SAX_H

#include "ParserInterface.h"
#include <string>
#include <istream>
#include <list>
#include <map>

namespace  ThorsAnvil
{
    namespace Json
    {

struct Key
{
    std::string const   mapKey;
    int         const   arrKey;
    bool        const   isMapKey;

    Key(std::string const& key): mapKey(key), arrKey(0),   isMapKey(true) {}
    Key(int key):                mapKey(""),  arrKey(key), isMapKey(false){}
};

class ScannerSax;
struct SaxAction
{
    virtual ~SaxAction() {}
    virtual void doAction(ScannerSax&, Key const& key, JsonValue const&) = 0;
    virtual void doPreAction(ScannerSax&,Key const& key)                 = 0;
};


class ScannerSaxInterface;
typedef void*  ActionRefNote;
class ScannerSax
{
    template<typename Key>
    class PtrMap: public std::map<Key, SaxAction*>
    {
        void clean()
        {
            for(typename std::map<Key, SaxAction*>::const_iterator loop = this->begin(); loop != this->end(); ++loop)
            {
                delete loop->second;
            }
        }
        public:
        // Note: Copy constructor only works for empty map.
        // The copy constructor is only used to push these object into a list when they are empty
        // So that works OK
        ~PtrMap()
        {
            clean();
        }
        void clear()
        {
            clean();
            std::map<Key, SaxAction*>::clear();
        }
    };
    typedef PtrMap<std::string>                 ScannerMapActionMap;
    typedef std::list<ScannerMapActionMap>      ScannerMapActionList;
    struct ScannerArrActionMap
    {
        typedef PtrMap<int>::const_iterator     const_iterator;

        ScannerArrActionMap() : max(0)          {}
        void            clear()                 { max = 0; map.clear();}
        const_iterator  find(int index) const   {return map.find(index);}
        const_iterator  end()           const   {return map.end();}
        int                                 max;
        PtrMap<int>                         map;
    };
    typedef std::list<ScannerArrActionMap>      ScannerArrActionList;

    ScannerMapActionList                    mapActions;
    ScannerArrActionList                    arrActions;

    ScannerMapActionList::const_iterator    mapAction;
    ScannerArrActionList::const_iterator    arrAction;

    public:
        ~ScannerSax();
         ScannerSax();
        template<typename Parser>
        void            parse(std::istream& src);
        ActionRefNote   registerAction(std::string const& mapItem, SMART_OWNED_PTR<SaxAction> action);
        ActionRefNote   registerActionNext(SMART_OWNED_PTR<SaxAction> action);
        ActionRefNote   registerActionOnAllMapItems(SMART_OWNED_PTR<SaxAction> action);
        ActionRefNote   registerActionOnAllArrItems(SMART_OWNED_PTR<SaxAction> action);
        void            replaceAction(ActionRefNote oldActionRef, SMART_OWNED_PTR<SaxAction> action);

    private:
        friend class ScannerSaxInterface;
        void    preActivate(std::string const& mapItem);
        void    preActivate(int index);
        void    activate(std::string const& mapItem, JsonValue const& value);
        void    activate(int index, JsonValue const& value);

        SaxAction* getAction(std::string const& item);
        SaxAction* getAction(int index);

        void    push_mapAction()    {mapActions.push_front(ScannerMapActionMap());--mapAction;}
        void    push_arrAction()    {arrActions.push_front(ScannerArrActionMap());--arrAction;}
        void    pop_mapAction()     {mapActions.pop_front();mapActions.front().clear();++mapAction;}
        void    pop_arrAction()     {arrActions.pop_front();arrActions.front().clear();++arrAction;}
};


    }
}

#endif

