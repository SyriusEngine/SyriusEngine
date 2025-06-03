#include "IRenderComponentTest.hpp"

Syrius::UP<Syrius::SyriusWindow> IRenderComponentTest::m_Window = nullptr;
Syrius::ResourceView<Syrius::Context> IRenderComponentTest::m_Context;
bool IRenderComponentTest::m_SetupDone = false;