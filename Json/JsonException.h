
#ifndef THORSANVIL_JSON_JSON_EXCEPTION_H
#define THORSANVIL_JSON_JSON_EXCEPTION_H

#include <stdexcept>

namespace ThorsAnvil
{
    namespace Json
    {

/*
 * Exceptions
 */
class JsonException: public std::runtime_error
{   public: JsonException(std::string const& msg) : std::runtime_error(msg) {}
};

// Throw when trying to get an integer from a string or string from an integer etc.
class InvalidConversion: public JsonException
{   public: InvalidConversion(): JsonException("Invalid Conversion"){}};
// Thrown when there is something wrong with the input stream
class ParsingError: public JsonException
{   public: ParsingError(std::string const& msg): JsonException(msg) {}};


    }
}

#endif

