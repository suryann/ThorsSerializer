
#ifndef THORSANVIL_SERIALIZE_JSON_SERIALIZE_H
#define THORSANVIL_SERIALIZE_JSON_SERIALIZE_H

/* Content:
 *
 * Used to define how a class is imported/exported via the Json Serialized
 *
 * Conventions:
 *      T:      The type of the base class
 *      I:      The type of the member
 *      MP:     The type of the member pointer
 *
 * Every type that is serializeable has a class JsonSerializeTraits.
 * For basic types (int/float etc) no definition is required and the default template version works
 * For compound types you need to define the type SerializeInfo.
 *      SerializeInfo:           It is a mpl vector of types.
 *                              Each type (in the vector) defines how to serialize
 *                              a member of the compound type.
 *
 * Boilerplate  code to create the appropriate types for SerializeInfo.
 *      #define THORSANVIL_SERIALIZE_JsonAttribute(className, member)
 *
 * Example:
 *      class MyClass
 *      {
 *          // STUFF

 *          // If any members are private and need to be serialized then
 *          // JsonSerializeTraits<MyClass> must be a friend of the class so it can generate the appropriate code
 *          // The macro THORSANVIL_SERIALIZE_JsonAttribute will try to access these members (See below)
 *
 *          friend class JsonSerializeTraits<MyClass>;
 *      };
 *
 *      namespace ThorsAnvil { namespace Serialize { namespace Json {
 *      template<>
 *      class JsonSerializeTraits<MyClass>
 *      {
 *          THORSANVIL_SERIALIZE_JsonAttribute(MyClass, member1);
 *          THORSANVIL_SERIALIZE_JsonAttribute(MyClass, member2);
 *          THORSANVIL_SERIALIZE_JsonAttribute(MyClass, member3);
 *          typedef boost::mps::vector<member1, member2, member3>   SerializeInfo;
 *      };
 *      }}}
 *
 * If you can not access members directly (ie you are trying to write serialization to code that is not yours (std::vector))
 * Then you will need to write your own accessors type to simulate them members you want to serialize:
 *
 *      namespace ThorsAnvil { namespace Serialize { namespace Json {
 *      template<typename T>
 *      class JsonSerializeTraits<std::vector<T> >
 *      {
 *          MIY TODO Example needs updating.
 *          The classes have changed slightly.
 *
 *          // Can not access the members of vector directly.
 *          // So we serialize it as two elements:
 *          //      A size property (so we can reserve space (if we see it first)
 *          //      An elements property. This serialize the elemnts as a Json Array 
 *          //      For each of these we need to write specialized class's to access the public API
 *          //      See ArraySizeAccessor and ArrayElementAccessor for a detailed example
 *          //
 *          THORSANVIL_SERIALIZE_JsonAttributeAccess(std::vector<T>, size,     ArraySizeAccessor);
 *          THORSANVIL_SERIALIZE_JsonAttributeAccess(std::vector<T>, elements, ArrayElementAccessor);
 *          typedef boost::mps::vector<size, elements>              SerializeInfo;
 *      };
 *      }}}
 *
 * Now we can serialize/deserialize with:
 *      std::cout << jsonExport(myClassObj) << "\n";
 *      std::cin  >> jsonInport(myClassObj);
 *
 * Same for std::vector
 *      std::vector<MyClass>  vec;  // Will serialize any fundamental type or type with JsonSerializeTraits<> specialized for it
 *                                  // If JsonSerializeTraits<> is not specialized for a compound type you get a compile time
 *                                  // error
 *      std::cout << jsonExport(vec) << "\n";
 *      std::cin  >> jsonImport(vec);
 */
#include "json/ScannerSax.h"
#include "json/ParserShiftReduce.tab.hpp"
#include <boost/mpl/at.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/typeof/typeof.hpp>
#include <iostream>


#define THORSANVIL_SERIALIZE_JsonAttribute(className, member)                                                                           \
    typedef BOOST_TYPEOF(((className*)NULL)->member)    JsonAttribute ## member ## Type;                                                \
    THORSANVIL_SERIALIZE_JsonAttribute_1(className, member, JsonSerializeTraits<JsonAttribute ## member ## Type>)

#define THORSANVIL_SERIALIZE_JsonAttribute_1(className, member, SerTraits)                                                              \
    typedef BOOST_TYPEOF(&className::member)            JsonAttribute ## member ## TypePtr;                                             \
    typedef JsonSerialElementAccessor<className, JsonAttribute ## member ## TypePtr, SerTraits>  JsonAttribute ## member ## Accessor;   \
    struct member: JsonSerializeItem<className, JsonAttribute ## member ## Accessor>                                                    \
    {                                                                                                                                   \
        member()                                                                                                                        \
            : JsonSerializeItem<className, JsonAttribute ## member ## Accessor>(#member, &className::member)                            \
        {}                                                                                                                              \
    }
#define THORSANVIL_SERIALIZE_JsonAttributeAccess(className, member, accessor)                                                           \
    struct member: JsonSerializeItem<className, accessor>                                                                               \
    {                                                                                                                                   \
        member()                                                                                                                        \
            : JsonSerializeItem<className, accessor>(#member, accessor())                                                               \
        {}                                                                                                                              \
    }



namespace ThorsAnvil
{
    namespace Serialize
    {

        template<typename T, typename Parser>
        struct Importer;
                    
        template<typename T, typename Printer>
        struct Exporter;

        namespace Json
        {



/* Class used by boost::mpl::for_each. Nothing special simple lamba's will replace them in the future */
/*
 *  T       The object Type
 *  S       The source type (parser or std::ostream)
 *
 * This is basically trying to templatize and remove the need for multiple action objects that
 * are called from mpl::for_each
 */
template<typename T, typename S>
struct MPLForEachActivateTrait;

template<typename T>
struct MPLForEachActivateTrait<T, std::ostream>
{
    typedef const T                 ObjectType;
    static int const                SerializeActionIndex    = 0;
};
template<typename T>
struct MPLForEachActivateTrait<T, ThorsAnvil::Json::ScannerSax>
{
    typedef T                       ObjectType;
    static int const                SerializeActionIndex    = 1;
};
template<typename T, typename S>
class MPLForEachActivateItem
{
    typedef          MPLForEachActivateTrait<T, S>  Traits;
    typedef typename Traits::ObjectType             ObjectType;

    S&              pump;
    ObjectType&     object;
    public:
        MPLForEachActivateItem(S& p, ObjectType& obj)
            : pump(p)
            , object(obj)
        {}
        // Depending on if the pump is a stream or a scanner
        // Calls      JsonSerialize::activate()
        // or         JsonDeSerialize::activate()
        template<typename SerializeItem>
        void operator()(SerializeItem const& item) const
        {
            typedef typename boost::mpl::at_c<typename SerializeItem::SerializeType, Traits::SerializeActionIndex>::type  SerializeAction;
            SerializeAction::activate(item, pump, object);
        }
};

/*
 * Objects of this type get stored in the
 *      JsonSerializeTraits::SerializeInfo
 *  This is what the user create with the macros above. The default A is JsonSerialElementAccessor
 *  But user can define their own action for complex objects This wrapper is merely a vehicle for
 *  calling the A methods in a controlled manner.
 *
 * Note: These classes are not designed to be used directly but via the macros:
 *          THORSANVIL_SERIALIZE_JsonAttribute
 *          THORSANVIL_SERIALIZE_JsonAttributeAccess
 *  See the notes by these macros for details
 */
template<typename T, typename A>
struct JsonSerializeItem;

template<typename T, typename A>
struct JsonSerialize
{
    static void activate(JsonSerializeItem<T, A> const& item, std::ostream& stream, T const& src)
    {
        if (!item.first)
        {   stream << ',';
        }
        stream << '"' << item.memberName << '"' << ": ";
        item.accessor.serialize(src, stream);
    }
};
template<typename T, typename A>
struct JsonDeSerialize
{
    static void activate(JsonSerializeItem<T, A> const& item, ThorsAnvil::Json::ScannerSax& parser, T& dst)
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>    action(item.accessor.action(dst));
        parser.registerAction(item.memberName, action);
    }
};
template<typename T, typename A>
struct JsonSerializeItem
{
    std::string memberName;
    A           accessor;
    bool        first;

    JsonSerializeItem(std::string const& name, A const& ac): memberName(name), accessor(ac), first(false) {}
    JsonSerializeItem& makeFirst() {first = true;return *this;}

    typedef    JsonSerialize<T,A>      Serialize;
    typedef    JsonDeSerialize<T,A>    DeSerialize;

    typedef boost::mpl::vector<Serialize, DeSerialize>      SerializeType;
};
/*
 * Importing
 * ============
 *
 * The JsonImportAction defines a class that is registered with a Json SAX parser
 * so that we can register callback actions for particular attributes.
 *
 * For fundamental types json is read directly into the value.
 * For compound types when the attribute is reached additional callbacks are registered
 * for each of the compound members that needs to be de-serialized (this is done recursively)
 * So we can de-serialize arbitrary json structures.
 */
template<typename SerializeInfo, typename I, bool EnablePod = boost::is_fundamental<I>::value>
class JsonImportAction: public ThorsAnvil::Json::SaxAction
{
    I&              memberRef;
    public:
        JsonImportAction(I& mr)
            : memberRef(mr)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax&){}
        // Read fundamental type directly into the member
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, JsonValue const& value)
        {
            memberRef   = value.getValue<I>();
        }
};
template<typename SerializeInfo, typename I>
class JsonImportAction<SerializeInfo, I, false>: public ThorsAnvil::Json::SaxAction
{
    I&              memberRef;
    public:
        JsonImportAction(I& mr)
            : memberRef(mr)
        {}

        virtual void doAction(ThorsAnvil::Json::ScannerSax&, JsonValue const&){}
        // Compound type. Register callback for each member.
        //                This is done when the attribute is reached in json not before
        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser)
        {
            boost::mpl::for_each<SerializeInfo>(MPLForEachActivateItem<I, ThorsAnvil::Json::ScannerSax>(parser, memberRef));
        }
};
template<typename SerializeInfo>
class JsonImportAction<SerializeInfo, std::string, false>: public ThorsAnvil::Json::SaxAction
{
    std::string&              memberRef;
    public:
        JsonImportAction(std::string& mr)
            : memberRef(mr)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax&){}
        // Specialization for std::string (as it is supported directly in json)
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, JsonValue const& value)
        {
            memberRef   = value.getValue<std::string>();
        }
};

/*
 * Need a function template to create the correct JsonImportAction()
 */
template<typename SerializeInfo, typename T, typename I>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(T& dst, I T::* memberPtr)
{
    return new JsonImportAction<SerializeInfo, I>(dst.*memberPtr);
}


enum JsonSerializeType {Invalid, Map, Array}; 
/* Default Serialization Traits:
 * Used by all types without their own specific serialization traits.
 */
template<JsonSerializeType>
struct JsonSerializeBrace
{
    static char braces[];
};

template<typename T>
struct JsonSerializeTraits
{
    typedef T                     SerializeInfo;
    static JsonSerializeType const  type    = Invalid;
};
/* Move This */
template<typename K, typename V>
struct JsonSerializeTraits<std::map<K, V> >
{
    typedef void                  SerializeInfo;
    static JsonSerializeType const  type    = Map;
};

/*
 * The MemberScanner is used to register callbacks that will read sub-members from the json object
 */
template<typename T, typename MemberToSerialize = typename JsonSerializeTraits<T>::SerializeInfo>
struct MemberScanner
{
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T& destination)
    {
        boost::mpl::for_each<typename JsonSerializeTraits<T>::SerializeInfo>(MPLForEachActivateItem<T, ThorsAnvil::Json::ScannerSax>(scanner, destination));
    }
};
template<typename T>
struct MemberScanner<T, T>
{
    // A normal type with no SerializeInfo has no members thus no need to register callbacks.
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T& destination)
    {}
};
template<typename T>
struct MemberScanner<T, void>
{
    // A normal type with no SerializeInfo has no members thus no need to register callbacks.
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T& destination)
    {}
};

/*
 * The MemberPrinter prints each member of an object.
 */
template<typename T, typename MemberToSerialize = typename JsonSerializeTraits<T>::SerializeInfo>
struct MemberPrinter
{
    void operator()(std::ostream& stream, T const& source)
    {
        typedef typename boost::mpl::at<typename JsonSerializeTraits<T>::SerializeInfo, boost::integral_constant<int,0> >::type  FirstType;
        typedef typename boost::mpl::pop_front<typename JsonSerializeTraits<T>::SerializeInfo>::type                             AllButFirstType;

        MPLForEachActivateItem<T, std::ostream>     itemPrinter(stream, source);

        // Print the first member (Call makeFirst() means no comma is printed)
        itemPrinter(FirstType().makeFirst());

        // For each of the other members use a loop a proceed each object with a comma
        boost::mpl::for_each<AllButFirstType>(itemPrinter);
    }
};
template<typename T>
struct MemberPrinter<T, T>
{
    // A normal object just prints itself.
    void operator()(std::ostream& stream, T const& source)
    {
        stream << source;
    }
};
template<>
struct MemberPrinter<bool>
{
    // boolean members need to print true/false
    void operator()(std::ostream& stream, bool const& source)
    {
        stream << std::boolalpha << source;
    }
};
template<>
struct MemberPrinter<std::string>
{
    // strings need to be quoted.
    void operator()(std::ostream& stream, std::string const& source)
    {
        stream << '"' << source << '"';
    }
};
template<typename T>
struct MemberPrinter<T, void>
{
    void operator()(std::ostream& stream, T const& source)
    {}
};


struct JsonSerializer
{
    template<typename T, JsonSerializeType base = JsonSerializeTraits<T>::type>
    struct Parser: ThorsAnvil::Json::ScannerSax
    {
        typedef boost::mpl::bool_<base != Invalid>      Parsable;
        Parser(T& dst)
            : destination(dst)
        {
            MemberScanner<T>    memberScanner;
            memberScanner(*this, destination);
        }

        void parse(std::istream& stream)
        {
            ScannerSax::parse<yy::ParserShiftReduce>(stream);
        }

        T&  destination;
    };

    template<typename T ,JsonSerializeType base = JsonSerializeTraits<T>::type>
    struct Printer
    {
        typedef boost::mpl::bool_<base != Invalid>      Printable;
        Printer(T const& src)
            : source(src)
        {}

        void print(std::ostream& stream)
        {
            stream << JsonSerializeBrace<base>::braces[0];

            MemberPrinter<T>    memberPrinter;
            memberPrinter(stream, source);

            stream << JsonSerializeBrace<base>::braces[1];
        }

        T const&    source;
    };

};

template<typename T>
struct JsonSerializeTraits<T*>
{
    typedef void*                    SerializeInfo;
    static JsonSerializeType const  type    = Invalid;
};
template<typename T>
struct MemberScanner<T*, void*>
{
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T*& destination)
    {}
};
template<typename I>
class JsonImportAction<void*, I*, false>: public ThorsAnvil::Json::SaxAction
{
    I*&              memberRef;
    bool             ok;
    JsonImportAction(JsonImportAction const& copy);
    JsonImportAction& operator=(JsonImportAction const& copy);
    public:
        JsonImportAction(I*& mr)
            : memberRef(mr)
            , ok(false)
        {
            memberRef   = NULL;
        }
        ~JsonImportAction()
        {
            if (!ok)
            {
                delete memberRef;
                memberRef   = NULL;
            }
        }

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser)
        {
            memberRef   = new I;

            MemberScanner<I>   scanner;
            scanner(parser, *memberRef);
        }
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, JsonValue const& element)
        {
            /* Note: if element is NULL then we parsed an array or a map.
             *       This means that it was NOT a NULL pointer
             *       If the element is non NULL then we parsed a simple object (NULL/true/false/Number/String)
             *       But any non 'null' Json value would have generated an exception. So if &element is not NULL
             *       then it is a reference to a JsonNULLItem object. Which means we need to clean up.
             */
            if (&element != NULL)
            {
                delete memberRef;
                memberRef   = NULL;
            }
            ok  = true;
        }
};

template<typename T>
struct MemberPrinter<T*, void*>
{
    void operator()(std::ostream& stream, T* const& source)
    {
        BOOST_STATIC_ASSERT(JsonSerializer::template Printer<T>::Printable::value);
        if (source == NULL)
        {

            stream << "null";
        }
        else
        {
            stream << jsonInternalExport(*source);
        }
    }
};


/*
 * Default accessors for fundamental types std::string
 * The serialize()  Recursively calls jsonInternalExport() on the member to serialize the member.
 *
 * The action()     Creates a JsonImportAction() that is registered with the SAX parser that just reads the
 *                  Item directly into the object. If the object is a compound type it uses the SerializeInfo
 *                  to register subsequent actions recursively so we always read directly into an object
 *                  not a copy.
 */
template<typename T, typename MP, typename SerTraits>
class JsonSerialElementAccessor
{
    MP          memberPtr;
    public:
    JsonSerialElementAccessor(MP mp): memberPtr(mp)    {}
    void serialize(T const& src, std::ostream& stream) const
    {
        stream << jsonInternalExport(src.*memberPtr);
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(T& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<typename SerTraits::SerializeInfo>(dst, memberPtr));
        return action;
    }
};

/* Helper functions */
template<typename T>
Importer<T, typename JsonSerializer::template Parser<T> >   jsonInternalImport(T& object)
{
    return Importer<T, typename JsonSerializer::template Parser<T> >(object);
}

template<typename T>
Exporter<T, typename JsonSerializer::template Printer<T> >  jsonInternalExport(T const& object)
{
    return Exporter<T, typename JsonSerializer::template Printer<T> >(object);
}




        }
    }
}


#endif

