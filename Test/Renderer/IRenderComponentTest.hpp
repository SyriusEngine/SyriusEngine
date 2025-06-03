#pragma once

#include <SyriusCore/SyriusCore.hpp>
#include <gtest/gtest.h>

#include "../../src/Communication/DispatcherManager.hpp"
#include "../../src/Threading/WorkerPool.hpp"

class IRenderComponentTest: public ::testing::Test {
private:
    static bool m_SetupDone;
protected:
    static Syrius::UP<Syrius::SyriusWindow> m_Window;
    static Syrius::ResourceView<Syrius::Context> m_Context;
    Syrius::SP<Syrius::WorkerPool> m_WorkerPool;
    Syrius::SP<Syrius::DispatcherManager> m_DispatcherManager;

protected:

    void SetUp() override {
        using namespace Syrius;

        if (!m_SetupDone) {
            WindowDesc wDesc;
            wDesc.width = 800;
            wDesc.height = 600;
            m_Window = createWindow(wDesc);

            ContextDesc cDesc;
            cDesc.api = SR_API_OPENGL;      // Most compatible
            m_Context = m_Window->createContext(cDesc);
            m_SetupDone = true;
        }

        m_WorkerPool = createSP<WorkerPool>();
        m_DispatcherManager = createSP<DispatcherManager>(m_WorkerPool);
    }

    void TearDown() override {

    }
};