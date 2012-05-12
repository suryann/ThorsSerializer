
#ifndef THORSANVIL_JSON_JSON_DOM
#define THORSANVIL_JSON_JSON_DOM

#include "JsonException.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace ThorsAnvil
{
    namespace Json
    {

/* Functions:
 * Json-Dom Object
 *      JsonValue;
 *      JsonMap;
 *      JsonArray;
 *
 * Internal Objects
 *      JsonValue Traits for conversion on extraction
 *          struct ParseTrait
 *      JsonValue derived type for different internal objects:
 *          struct JsonValue
 *          struct JsonString
 *          struct JsonNumber
 *          struct JsonBool
 *          struct JsonNULL
 *          struct JsonMap
 *          struct JsonArray
 *
 */

/*
 * Classes that are used to build a DOM of a Json-Object
 */
struct JsonValue;
typedef boost::ptr_map<std::string, JsonValue> JsonMap;
typedef boost::ptr_vector<JsonValue>           JsonArray;
enum JsonObjectType { JsonMapObject, JsonArrayObject };
struct JsonObject
{
    JsonObjectType  type;
    union
    {
        JsonMap*    map;
        JsonArray*  array;
    } data;
};
typedef std::pair<std::auto_ptr<std::string>,std::auto_ptr<JsonValue> >     JsonMapValue;

/*
 * Json only supports three types:
 *      String
 *      double
 *      bool
 *
 * The ParseTraits class provides a mapping from all other POD types to these basic types.
 * It is used in JsonValue::getValue() to retrieve a value of appropriate type from the
 * JsonValue object which is then converted to the type you want.
 *
 * e.g.
 *      int x = pv->getValue<int>(); Will get the JsonValue() internal state as a long
 *                                   truncating the value towards zero. Normal long to int
 *                                   conversion will be done before returning the value
 *                                   which may overflow the value.
 */
template<typename T> struct ParseTrait {};

template<> struct ParseTrait<long double>       { typedef double       GetType;};
template<> struct ParseTrait<double>            { typedef double       GetType;};
template<> struct ParseTrait<float>             { typedef double       GetType;};

//template<> struct ParseTrait<long long>         { typedef long         GetType;};
template<> struct ParseTrait<long>              { typedef long         GetType;};
template<> struct ParseTrait<int>               { typedef long         GetType;};
template<> struct ParseTrait<short>             { typedef long         GetType;};
template<> struct ParseTrait<signed char>       { typedef long         GetType;};

//template<> struct ParseTrait<unsigned long long>{ typedef long         GetType;};
template<> struct ParseTrait<unsigned long>     { typedef long         GetType;};
template<> struct ParseTrait<unsigned int>      { typedef long         GetType;};
template<> struct ParseTrait<unsigned short>    { typedef long         GetType;};
template<> struct ParseTrait<unsigned char>     { typedef long         GetType;};

template<> struct ParseTrait<bool>              { typedef bool         GetType;};

template<> struct ParseTrait<std::string>       { typedef std::string  GetType;};


/*
 * The base type of all values extracted from Json
 */
std::ostream& operator<<(std::ostream& stream, JsonValue const& node);
std::ostream& operator<<(std::ostream& stream, JsonMap   const& node);
std::ostream& operator<<(std::ostream& stream, JsonArray const& node);

struct JsonValue
{
    virtual ~JsonValue()       {}

    template<typename T>
    T getValue() const
    {
        typename ParseTrait<T>::GetType     value;
        this->setValue(value);
        return value;
    }
    virtual void print(std::ostream& /*stream*/) const {throw std::runtime_error("Invalid Json");}

    private:
    virtual void setValue(long&)        const { throw InvalidConversion();}
    virtual void setValue(double&)      const { throw InvalidConversion();}
    virtual void setValue(bool&)        const { throw InvalidConversion();}
    virtual void setValue(std::string&) const { throw InvalidConversion();}
};
struct JsonStringItem: JsonValue
{
    std::auto_ptr<std::string>     value;
    JsonStringItem(std::auto_ptr<std::string>& data): value(data) {}

    virtual void print(std::ostream& stream) const    { stream << '"' << *value << '"'; }
    private:
        virtual void setValue(std::string&       dst)  const {dst = *value;}
};
struct JsonNumberItem: JsonValue
{
    std::auto_ptr<std::string>     value;
    JsonNumberItem(std::auto_ptr<std::string>& data): value(data)   {}

    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
        virtual void setValue(long&         dst)  const {dst = std::atol(value->c_str());}
        virtual void setValue(double&       dst)  const {dst = std::atof(value->c_str());}
};
struct JsonBoolItem: JsonValue
{
    bool                            value;
    JsonBoolItem(bool data): value(data)       {}

    virtual void print(std::ostream& stream) const    { stream << std::boolalpha << value; }
    private:
        virtual void setValue(bool&         dst)  const {dst = value;}
};
struct JsonNULLItem: JsonValue
{
    virtual void print(std::ostream& stream) const    { stream << "null"; }
    private:
        virtual void setValue(long&         dst)  const {dst= 0;}
        virtual void setValue(double&       dst)  const {dst= 0.0;}
        virtual void setValue(bool&         dst)  const {dst= false;}
        virtual void setValue(std::string&  dst)  const {dst.clear();}
};
struct JsonMapItem: JsonValue
{
    std::auto_ptr<JsonMap>          value;
    JsonMapItem(std::auto_ptr<JsonMap>& data): value(data)    {}

    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
};
struct JsonArrayItem: JsonValue
{
    std::auto_ptr<JsonArray>        value;
    JsonArrayItem(std::auto_ptr<JsonArray>& data): value(data)    {}

    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
};

inline std::ostream& operator<<(std::ostream& stream, JsonValue const& node)
{
    node.print(stream);
    return stream;
}
inline std::ostream& operator<<(std::ostream& stream, JsonMap const& node)
{
    stream << "{";
    if (!node.empty())
    {
        JsonMap::const_iterator loop = node.begin();
        stream << "\"" << loop->first << "\": " << (*loop->second);

        for(++loop; loop != node.end(); ++loop)
        {
            stream << ", \"" << loop->first << "\": " << (*loop->second);
        }
    }
    stream << "}";

    return stream;
}
inline std::ostream& operator<<(std::ostream& stream, JsonArray const& node)
{
    stream << "[";
    if (!node.empty())
    {
        JsonArray::const_iterator loop = node.begin();
        stream << (*loop);

        for(++loop; loop != node.end(); ++loop)
        {
            stream << ", " << (*loop);
        }
    }
    stream << "]";

    return stream;
}

    }
}

#endif

