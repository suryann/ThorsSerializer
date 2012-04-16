
#ifndef THORSANVIL_SIERALIZE_SERIALIZE_H
#define THORSANVIL_SIERALIZE_SERIALIZE_H

#include <iostream>


namespace ThorsAnvil
{
    namespace Serialize
    {


template<typename T, typename Parser>
struct Importer
{
    Importer(T& obj)
        : object(obj)
    {}

    T&          object;
    friend std::istream& operator>>(std::istream& stream, Importer<T, Parser> const& dst)
    {
        Parser    importer(dst.object);
        importer.parse(stream);

        return stream;
    }
};

template<typename S, typename T>
Importer<T, typename S::template Parser<T> > importObj(T& object)           {return Importer<T, typename S::template Parser<T> >(object);}


template<typename T, typename Printer>
struct Exporter
{
    Exporter(T const& obj)
        : object(obj)
    {}

    T const&    object;
    friend std::ostream& operator<<(std::ostream& stream, Exporter<T, Printer> const& src)
    {
        Printer     printer(src.object);
        printer.print(stream);

        return stream;
    }
};

template<typename S, typename T>
Exporter<T, typename S::template Printer<T> > exportObj(T const& object)     {return Exporter<T, typename S::template Printer<T> >(object);}



    }
}


#endif

