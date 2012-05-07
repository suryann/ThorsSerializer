
#include "ParserInterface.h"
#include "JsonException.h"
#include "LexerJson.h"

#include <boost/lexical_cast.hpp>

#include <string>
#include <iterator>

#include <stdint.h>

template<typename C>
struct TransformJsonStringIter: std::iterator<std::input_iterator_tag, char, ptrdiff_t, char*,char&>
{
    C&       cont;
    bool     lastWasSlash;
    int      unicodeCount;
    uint32_t unicodeValue;
    TransformJsonStringIter(C& c)
        : cont(c)
        , lastWasSlash(false)
        , unicodeCount(0)
    {}
    TransformJsonStringIter& operator++()       {return *this;}
    TransformJsonStringIter& operator*()        {return *this;}
    void                     operator=(char x)
    {
        if (unicodeCount)
        {
            if (unicodeCount == 6)
            {
                if (x != '\\')  { throw ThorsAnvil::Json::ParsingError("Surrogate pair(No Slash): \\uD8xx Must be followed by \\uDCxx");}
                --unicodeCount;
            }
            else if (unicodeCount == 5)
            {
                if (x != 'u')   { throw ThorsAnvil::Json::ParsingError("Surrogate pair(No u): \\uD8xx Must be followed by \\uDCxx");}
                --unicodeCount;
            }
            else
            {
                unicodeValue <<= 4;
                unicodeValue += ('0' <= x && x <= '9') ? (x - '0') : 10 + (('A' <= x && x <= 'F') ? (x - 'A') : (x - 'a'));
                --unicodeCount;
                if (unicodeCount == 0)
                {
                    if (unicodeValue <= 0x7F)
                    {
                        // Encode as single UTF-8 character
                        cont.push_back(unicodeValue);
                    }
                    else if (unicodeValue <= 0x7FF)
                    {
                        // Encode as two UTF-8 characters
                        cont.push_back(0xC0 |((unicodeValue >>  6)));
                        cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                    }
                    else if (unicodeValue <= 0xFFFF)
                    {
                        if ((unicodeValue & 0xFC00) != 0xD800)
                        {
                            // Encode as three UTF-8 characters
                            cont.push_back(0xE0 |((unicodeValue >> 12)));
                            cont.push_back(0x80 |((unicodeValue >>  6) & 0x3F));
                            cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                        }
                        else
                        {
                            // We have a found first part of surrogate pair
                            unicodeCount    = 6;
                        }
                    }
                    else
                    {
                        // Surrogate pair
                        if ((unicodeValue & 0xFC00FC00) != 0xD800DC00){ throw ThorsAnvil::Json::ParsingError("Surrogate pair(No DC): \\uD8xx Must be followed by \\uDCxx");}

                        // Decode surrogate pair
                        unicodeValue    = 0x00010000 | ((unicodeValue & 0x03FF0000) >> 6) | (unicodeValue & 0x000003FF);

                        // Encode as 4 UTF-8 characters
                        cont.push_back(0xF0 |((unicodeValue >> 18)));
                        cont.push_back(0x80 |((unicodeValue >> 12) & 0x3F));
                        cont.push_back(0x80 |((unicodeValue >>  6) & 0x3F));
                        cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                    }
                }
            }
        }
        else if (lastWasSlash)
        {
            switch(x)
            {
                case '"':   cont.push_back('"');    break;
                case '\\':  cont.push_back('\\');   break;
                case '/':   cont.push_back('/');    break;
                case 'b':   cont.push_back('\b');   break;
                case 'f':   cont.push_back('\f');   break;
                case 'n':   cont.push_back('\n');   break;
                case 'r':   cont.push_back('\r');   break;
                case 't':   cont.push_back('\t');   break;
                case 'u':   unicodeCount = 4; unicodeValue = 0; break;
            }
            lastWasSlash    = false;
        }
        else
        {
            if (x == '\\')
            {   lastWasSlash    = true;
            }
            else
            {   cont.push_back(x);
            }
        }
    }
};
template<typename C> TransformJsonStringIter<C> make_TransformJsonStringIter(C& cont)   {return TransformJsonStringIter<C>(cont);}

std::string ThorsAnvil::Json::getString(LexerJson& lexer)
{
    char const*     begin   = lexer.YYText();
    char const*     end     = begin + lexer.YYLeng();

    std::string     result;
    // Use begin+1 and end-1 to remove the quotes
    std::copy(begin+1, end-1, make_TransformJsonStringIter(result));

    return result;
}
std::string ThorsAnvil::Json::getNumber(LexerJson& lexer)
{
    return std::string(lexer.YYText(), lexer.YYLeng());
}



