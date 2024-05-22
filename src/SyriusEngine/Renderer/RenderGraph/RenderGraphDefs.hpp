#pragma once

#include "../RendererInclude.hpp"

namespace Syrius{

    typedef uint64 PassID;
    typedef std::vector<PassID> PassDependencies;

    template<typename T>
    class PassIDGenerator{
    public:
        static PassID getID(){
            static PassID id = generateID();
            return id;
        }
    };

    class Pass;

    struct PassDesc{
        std::string name;
        PassID id;
        PassDependencies dependencies;
        Pass* pAddress;
    };

}