
#ifndef THORS_ANVIL_SERIALIZE_JSON_JSON_SERIALIZER_BASIC_H
#define THORS_ANVIL_SERIALIZE_JSON_JSON_SERIALIZER_BASIC_H

#include "JsonSerializer.h"

namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {

/*
 * Specialization for bool
 */
template<>
struct MemberPrinter<bool>
{
    // boolean members need to print true/false
    void operator()(std::ostream& stream, bool const& source)
    {
        stream << std::boolalpha << source;
    }
};


/*
 * Specialization for std::string
 */
template<typename SerializeInfo>
class JsonImportAction<SerializeInfo, std::string, false>: public ThorsAnvil::Json::SaxAction
{
    std::string&              memberRef;
    public:
        JsonImportAction(std::string& mr)
            : memberRef(mr)
        {}

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&){}
        // Specialization for std::string (as it is supported directly in json)
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const& value)
        {
            memberRef   = value.getValue<std::string>();
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

/*
 * Specialization for pointers
 */
template<typename T>
struct JsonSerializeTraits<T*>
{
    static JsonSerializeType const  type    = Invalid;
    typedef void*                    SerializeInfo;
};
template<typename T>
struct MemberScanner<T*, void*>
{
    void operator()(ThorsAnvil::Json::ScannerSax& scanner, T*& destination)
    {}
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

        virtual void doPreAction(ThorsAnvil::Json::ScannerSax& parser, ThorsAnvil::Json::Key const&)
        {
            memberRef   = new I;

            MemberScanner<I>   scanner;
            scanner(parser, *memberRef);
        }
        virtual void doAction(ThorsAnvil::Json::ScannerSax&, ThorsAnvil::Json::Key const&, JsonValue const& element)
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

        }
    }
}


#endif

