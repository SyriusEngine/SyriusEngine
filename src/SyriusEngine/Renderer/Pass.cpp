#include "Pass.hpp"

namespace Syrius{

    Pass::~Pass() = default;

    Pass::Pass(ResourceView<Context> &context, const std::string &name, const PassID id):
    m_Context(context){
        SR_PRECONDITION(!name.empty(), "Pass name is empty");
        SR_PRECONDITION(id != 0, "Pass id is 0")

        m_PassDesc.name = name;
        m_PassDesc.id = id;
        m_PassDesc.pAddress = this;

        SR_POSTCONDITION(m_Context.get() != nullptr, "Context is null");
        SR_POSTCONDITION(m_PassDesc.pAddress == this, "Pass is not pointing to itself");
    }

}
