
/*
    #define THORSANVIL_SERIALIZE_JsonAttribute(className, member)                                                                           \
    #define THORSANVIL_SERIALIZE_JsonAttribute_1(className, member, SerTraits)                                                              \
    #define THORSANVIL_SERIALIZE_JsonAttributeAccess(className, member, accessor)                                                           \

    template<typename T, typename S>
    struct MPLForEachActivateTrait;
    template<typename T>
    struct MPLForEachActivateTrait<T, std::ostream>
    template<typename T>
    struct MPLForEachActivateTrait<T, ThorsAnvil::Json::ScannerSax>

    template<typename T, typename S>
    class MPLForEachActivateItem


    template<typename T, typename A>
    struct JsonSerialize
    template<typename T, typename A>
    struct JsonDeSerialize

    template<typename T, typename A>
    struct JsonSerializeItem

    template<typename SerTraits, typename I, bool EnablePod = boost::is_fundamental<I>::value>
    class JsonImportAction: public ThorsAnvil::Json::SaxAction
    template<typename SerTraits, typename I>
    class JsonImportAction<SerTraits, I, false>: public ThorsAnvil::Json::SaxAction
    template<typename SerTraits>
    class JsonImportAction<SerTraits, std::string, false>: public ThorsAnvil::Json::SaxAction

    template<typename SerTraits, typename T, typename I>
    ThorsAnvil::Json::SaxAction* new_JsonImportAction(T& dst, I T::* memberPtr);
    template<typename SerTraits, typename T, typename I>
    ThorsAnvil::Json::SaxAction* new_JsonImportAction(T& dst, I T::* memberPtr)

    template<typename T, typename MP, typename SerTraits>
    class JsonSerialElementAccessor


    enum PrinterBaseType {Invalid, Map, Array};

    template<PrinterBaseType>
    struct JsonSerializeBrace

    template<typename T>
    struct JsonSerializeTraits
    template<typename K, typename V>
    struct JsonSerializeTraits<std::map<K, V> >

    template<typename T, typename MemberToSerialize = typename JsonSerializeTraits<T>::SerializeInfo>
    struct MemberScanner
    template<typename T>
    struct MemberScanner<T, T>
    template<typename T>
    struct MemberScanner<T, void>

    template<typename T, typename MemberToSerialize = typename JsonSerializeTraits<T>::SerializeInfo>
    struct MemberPrinter
    template<typename T>
    struct MemberPrinter<T, T>
    template<typename T>
    struct MemberPrinter<T, void>

    struct JsonSerializer
*/

#ifndef THORSANVIL_SERIALIZE_JSON_SERIALIZE_H
#define THORSANVIL_SERIALIZE_JSON_SERIALIZE_H

#include "json/ScannerSax.h"
#include "json/ParserShiftReduce.tab.hpp"
#include <boost/mpl/at.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
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
        template<typename T, typename Printer>
        struct Exporter;

        template<typename S, typename T>
        Exporter<T, typename S::template Printer<T> > exportObj(T const& object);


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
        template<typename SerializeItem>
        void operator()(SerializeItem const& item) const
        {
            typedef typename boost::mpl::at_c<typename SerializeItem::SerializeType, Traits::SerializeActionIndex>::type  SerializeAction;
            SerializeAction::activate(item, pump, object);
        }
};

/*
 * Objects of this type get stored in the
 *      JsonSerializeTraits::SerializeMap
 *  This is what the user create with the macros above. The default A is JsonSerialElementAccessor
 *  But user can define their own action for complex objects This wrapper is merely a vehicle for
 *  calling the A methods in a controlled manner.
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
template<typename SerTraits, typename I, bool EnablePod = boost::is_fundamental<I>::value>
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
template<typename SerTraits, typename I>
class JsonImportAction<SerTraits, I, false>: public ThorsAnvil::Json::SaxAction
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
            boost::mpl::for_each<typename SerTraits::SerializeMap>(MPLForEachActivateItem<I, ThorsAnvil::Json::ScannerSax>(parser, memberRef));
        }
};
template<typename SerTraits>
class JsonImportAction<SerTraits, std::string, false>: public ThorsAnvil::Json::SaxAction
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
template<typename SerTraits, typename T, typename I>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(T& dst, I T::* memberPtr);

/*
 * Need a function template to create the correct JsonImportAction()
 */
template<typename SerTraits, typename T, typename I>
ThorsAnvil::Json::SaxAction* new_JsonImportAction(T& dst, I T::* memberPtr)
{
    return new JsonImportAction<SerTraits, I>(dst.*memberPtr);
}
/*
 * Default accessors for fundamental types std::string
 * The serialize () Wraps the object to be de-serialized in a SerializeExport object that has the will then
 *                  serialize the object to a stream.
 * The action()     Creates a JsonImportAction() that is registered with the SAX parser that just reads the
 *                  Item directly into the object. If the object is a compound type it uses the SerializeMap
 *                  to register subsequent actions recursively so we always read directly into an object
 *                  not a copy.
 */
struct JsonSerializer;
template<typename T, typename MP, typename SerTraits>
class JsonSerialElementAccessor
{
    MP          memberPtr;
    public:
    JsonSerialElementAccessor(MP mp): memberPtr(mp)    {}
    void serialize(T const& src, std::ostream& stream) const
    {
        stream << exportObj<JsonSerializer>(src.*memberPtr);
    }
    std::auto_ptr<ThorsAnvil::Json::SaxAction>      action(T& dst) const
    {
        std::auto_ptr<ThorsAnvil::Json::SaxAction>  action(new_JsonImportAction<SerTraits>(dst, memberPtr));
        return action;
    }
};



enum PrinterBaseType {Invalid, Map, Array};

/* Default Serialization Traits:
 * Used by all types without their own specific serialization traits.
 */
template<PrinterBaseType>
struct JsonSerializeBrace
{
    static char braces[];
};

template<typename T>
struct JsonSerializeTraits
{
    typedef T                     SerializeInfo;
    static PrinterBaseType const  type    = Invalid;
};
/* Move This */
template<typename K, typename V>
struct JsonSerializeTraits<std::map<K, V> >
{
    typedef void                  SerializeInfo;
    static PrinterBaseType const  type    = Map;
};

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
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T& destination)
    {
    }
};
template<typename T>
struct MemberScanner<T, void>
{
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T& destination)
    {}
};
template<typename T, typename MemberToSerialize = typename JsonSerializeTraits<T>::SerializeInfo>
struct MemberPrinter
{
    void operator()(std::ostream& stream, T const& source)
    {
        typedef typename boost::mpl::at<typename JsonSerializeTraits<T>::SerializeInfo, boost::integral_constant<int,0> >::type  FirstType;
        typedef typename boost::mpl::pop_front<typename JsonSerializeTraits<T>::SerializeInfo>::type                             AllButFirstType;

        MPLForEachActivateItem<T, std::ostream>(stream, source)(FirstType().makeFirst());
        boost::mpl::for_each<AllButFirstType>(MPLForEachActivateItem<T, std::ostream>(stream, source));
    }
};
template<typename T>
struct MemberPrinter<T, T>
{
    void operator()(std::ostream& stream, T const& source)
    {
        stream << source;
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
    template<typename T>
    struct Parser: ThorsAnvil::Json::ScannerSax
    {
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

    template<typename T ,PrinterBaseType base = JsonSerializeTraits<T>::type>
    struct Printer
    {
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




        }
    }
}


#endif

