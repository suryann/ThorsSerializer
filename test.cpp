    #include <iostream>
    #include <string>
    #include "serialize/json.h"

    /* A class that you want to serialize. */
    class MyClass
    {   
        int         data1;
        float       data2;
        std::string data3;

        // This is only required if the members are private.
        friend struct ThorsAnvil::Serialize::Json::JsonSerializeTraits<MyClass>;
        public:
            MyClass(int a, float b, std::string const& c): data1(a), data2(b), data3(c) {}
            friend std::ostream& operator<<(std::ostream& stream, MyClass const& value)
            {   
                // Not need for serialization but we need to print out the results to show it worked.
                return stream << value.data1 << " : " << value.data2 << " : " << value.data3;
            }   
    };  

    /*  
     * Though there is no code involved, you do need to set up
     * this structure to tell the library what fields need to be serialized.
     */  
    namespace ThorsAnvil { namespace Serialize { namespace Json {
    template<>
    struct JsonSerializeTraits<MyClass>
    {   
        static JsonSerializeType const  type    = Array;

        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data1);
        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data2);
        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data3);
        typedef boost::mpl::vector<data1, data2, data3>         SerializeInfo;
    };  
    }}} 

    int main()
    {   
        MyClass     obj(56, 23.456, "Hi there");
        std::cout << obj << "\n";
        std::cout << ThorsAnvil::Serialize::jsonExport(obj) << "\n";

        std::cin  >> ThorsAnvil::Serialize::jsonImport(obj);
        std::cout << obj << "\n";
    }   


