#pragma once

#include <SyriusCore/SyriusCore.hpp>
#include <gtest/gtest.h>

#include "../../src/Communication/DispatcherManager.hpp"
#include "../../src/Threading/WorkerPool.hpp"
#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

using namespace Syrius;

class IRenderComponentTest: public ::testing::Test {
private:
    static bool m_SetupDone;
protected:
    static UP<SyriusWindow> m_Window;
    static ResourceView<Context> m_Context;
    SP<WorkerPool> m_WorkerPool;
    SP<DispatcherManager> m_DispatcherManager;

protected:

    void SetUp() override;

    void TearDown() override;

    MeshID createMesh() const;

    void createMesh(MeshID meshID) const;

    void destroyMesh(MeshID meshID) const;

    InstanceID createInstance(MeshID meshID) const;

    void destroyInstance(InstanceID instanceID) const;

    void setInstanceTransformation(InstanceID instanceID, const Transform& transform) const;

    MaterialID createMaterial() const;

    void destroyMaterial(MaterialID materialID) const;

    void setMeshMaterial(MeshID meshID, MaterialID materialID) const;
};