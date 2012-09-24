Yet another JSON serialization library for C++

Build instructions (SIMPLE)

    make -f MakeSimple
    # Note this build a Serialize.a library
    #       No tests are run and no coverage test are done.
    #       The serialize library are mostly header files so you will still need these
    #       to include these from your own files.

Building Notes:
    This project can also use ThorMaker (which is just a set of makefiles) to build
    This not only builds the code but runs unit tests and checks the code coverage of the unit tests.

    Build instructions:
    ###################

    # Set up required 3rd party libraries boost/gtest
    cd third
    ./setup


    # The following builds the Json parser and installs the
    # release and debug libraries into build/lib and external headers into build/include
    cd ../Json
    make install

    # The following builds the Serialization libs and installs the
    # release and debug libraries into build/lib and external headers into build/include
    cd ../Serialize
    make install


Objective:

    The objective is to make serialization/de-serialization of C++ object to/from
    JSON trivial.

    This means:
        1) does not build a JSON object. Reads data directly into C++ object.
        2) In normal usage there should be NO need to write any code.
        3) User should not need to understand JSON or validate its input.
        4) Should work seamlessly with streams.
        5) Standard containers should automatically work

    I am not concerned about speed.
    Though my trivial test work just fine in terms of speed.
    
    The design was done with the primary goal of communicating with WEB-Servers
    that speak JSON. The main envisioned usage was for mobile devices were many
    small JSON objects are transfered in both directions.

Example: (see code in test.cpp for full code)

    /* A class that you want to serialize. */
    class MyClass
    {   
        int         data1;
        float       data2;
        std::string data3;

        // This is only required if the members are private.
        friend struct ThorsAnvil::Serialize::Json::JsonSerializeTraits<MyClass>;
    };  

    /*  
     * Though there is no code involved, you do need to set up
     * this structure to tell the library what fields need to be serialized.
     */  
    namespace ThorsAnvil { namespace Serialize { namespace Json {
    template<>
    struct JsonSerializeTraits<MyClass>
    {   
        // This says serialize as a JSON object.
        static JsonSerializeType const  type    = Map;

        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data1);
        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data2);
        THORSANVIL_SERIALIZE_JsonAttribute(MyClass, data3);

        // This type indicates what fields need to be serialize/de-serialzied.
        typedef boost::mpl::vector<data1, data2, data3>         SerializeInfo;
    };  
    }}} 

    # Build
    > g++ -o test test.cpp -lserialize -ljson

    # The MyClass object is serialize as:
    {"data1": 56 ,"data2": 23.456 ,"data3": "Hi there" }






