#include "../../../../src/Renderer/RenderGraph/RenderGraphContainer.hpp"
#include "../../../../src/Renderer/RenderGraph/Stores/LightStore.hpp"
#include "../../IRenderComponentTest.hpp"

using namespace Syrius;
using namespace Syrius::Renderer;

class TestLightStore: public IRenderComponentTest {
protected:
    void SetUp() override {
        IRenderComponentTest::SetUp();
        m_Container = createUP<RenderGraphContainer>();
    }

protected:
    UP<RenderGraphContainer> m_Container;
};

TEST_F(TestLightStore, CreateLight) {
    m_Container->createData<LightStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto lightStore = m_Container->getData<LightStore>();
    const auto& keyLightMap = lightStore->get_m_KeyLightMap();
    const auto& lightData = lightStore->get_m_LightData();

    auto l1 = createLight({glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f)});
    auto l2 = createLight({glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f)});
    auto l3 = createLight({glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(3.0f, 3.0f, 3.0f)});

    EXPECT_EQ(keyLightMap.size(), 3);
    EXPECT_EQ(lightData.m_LightCount.x, 3); // Count is stored in the X variable, rest is padding

    EXPECT_TRUE(keyLightMap.find(l1) != keyLightMap.end());
    EXPECT_TRUE(keyLightMap.find(l2) != keyLightMap.end());
    EXPECT_TRUE(keyLightMap.find(l3) != keyLightMap.end());

    EXPECT_NEAR(lightData.positions[keyLightMap.at(l1)].r, 5.0f, 1e-4);
    EXPECT_NEAR(lightData.positions[keyLightMap.at(l2)].g, 5.0f, 1e-4);
    EXPECT_NEAR(lightData.positions[keyLightMap.at(l3)].b, 5.0f, 1e-4);
}