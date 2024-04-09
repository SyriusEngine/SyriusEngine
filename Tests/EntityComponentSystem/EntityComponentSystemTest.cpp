#include "EntityComponentSystemTest.hpp"

void EntityComponentSystemTest::SetUp() {
    Test::SetUp();
}

void EntityComponentSystemTest::TearDown() {
    Test::TearDown();
}

TEST_F(EntityComponentSystemTest, ComponentIDGeneration) {
    auto moveID = Component<MoveComponent>::getID();
    auto healthID = Component<HealthComponent>::getID();

    EXPECT_NE(moveID, 0);
    EXPECT_NE(healthID, 0);
    EXPECT_NE(moveID, healthID);

    auto idMap = ComponentBase::getComponentIDMap();
    EXPECT_EQ(idMap.size(), 2);
    EXPECT_TRUE(idMap.find(typeid(MoveComponent)) != idMap.end());
    EXPECT_TRUE(idMap.find(typeid(HealthComponent)) != idMap.end());
}

TEST_F(EntityComponentSystemTest, ComponentDuplicates){
    auto moveID = Component<MoveComponent>::getID();
    auto moveID2 = Component<MoveComponent>::getID();

    EXPECT_EQ(moveID, moveID2);

    auto healthID = Component<HealthComponent>::getID();
    auto healthID2 = Component<HealthComponent>::getID();

    EXPECT_EQ(healthID, healthID2);
}

TEST_F(EntityComponentSystemTest, CreateEntity){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();
    auto e2 = ecs.createEntity();

    EXPECT_NE(e1, 0);
    EXPECT_NE(e2, 0);
    EXPECT_NE(e1, e2);

    auto entities = ecs.getEntities();
    EXPECT_EQ(entities.size(), 2);
    EXPECT_TRUE(entities.find(e1) != entities.end());
    EXPECT_TRUE(entities.find(e2) != entities.end());
    EXPECT_TRUE(entities[e1].empty());
    EXPECT_TRUE(entities[e2].empty());
}


TEST_F(EntityComponentSystemTest, AddComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();
    auto e2 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);
    ecs.addComponent<HealthComponent>(e2, 95);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));
    EXPECT_FALSE(ecs.hasComponent<MoveComponent>(e2));
    EXPECT_TRUE(ecs.hasComponent<HealthComponent>(e2));
    EXPECT_FALSE(ecs.hasComponent<HealthComponent>(e1));
}

TEST_F(EntityComponentSystemTest, AddComponentDuplicate){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);
    EXPECT_DEATH(ecs.addComponent<MoveComponent>(e1, 3.0f, 4.0f), "");
}

TEST_F(EntityComponentSystemTest, GetComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();
    auto e2 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);
    ecs.addComponent<HealthComponent>(e2, 95);

    auto& moveComponent = ecs.getComponent<MoveComponent>(e1);
    EXPECT_EQ(moveComponent.getX(), 1.0f);
    EXPECT_EQ(moveComponent.getY(), 2.0f);

    auto& healthComponent = ecs.getComponent<HealthComponent>(e2);
    EXPECT_EQ(healthComponent.getHealth(), 95);
}

TEST_F(EntityComponentSystemTest, GetComponentNonexistingComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();

    EXPECT_DEATH(ecs.getComponent<MoveComponent>(e1), "");
}

TEST_F(EntityComponentSystemTest, GetComponentNonexistingEntity){
    EntityComponentSystem ecs;

    EXPECT_DEATH(ecs.getComponent<MoveComponent>(6464), "");
}

TEST_F(EntityComponentSystemTest, RemoveSingleComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));

    ecs.removeComponent<MoveComponent>(e1);

    EXPECT_FALSE(ecs.hasComponent<MoveComponent>(e1));
}

TEST_F(EntityComponentSystemTest, RemoveMultipleComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();
    auto e2 = ecs.createEntity();
    auto e3 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);
    ecs.addComponent<MoveComponent>(e2, 3.0f, 4.0f);
    ecs.addComponent<MoveComponent>(e3, 5.0f, 6.0f);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));
    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e2));
    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e3));

    ecs.removeComponent<MoveComponent>(e1);

    EXPECT_FALSE(ecs.hasComponent<MoveComponent>(e1));
    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e2));
    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e3));

    auto move2 = ecs.getComponent<MoveComponent>(e2);
    EXPECT_EQ(move2.getX(), 3.0f);
    EXPECT_EQ(move2.getY(), 4.0f);

    auto move3 = ecs.getComponent<MoveComponent>(e3);
    EXPECT_EQ(move3.getX(), 5.0f);
    EXPECT_EQ(move3.getY(), 6.0f);
}

TEST_F(EntityComponentSystemTest, RemoveNonexistingComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();

    EXPECT_DEATH(ecs.removeComponent<MoveComponent>(e1), "");
}

TEST_F(EntityComponentSystemTest, RemoveComponentAndAddComponent){
    EntityComponentSystem ecs;

    auto e1 = ecs.createEntity();
    auto e2 = ecs.createEntity();

    ecs.addComponent<MoveComponent>(e1, 1.0f, 2.0f);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));
    auto move1 = ecs.getComponent<MoveComponent>(e1);
    EXPECT_EQ(move1.getX(), 1.0f);
    EXPECT_EQ(move1.getY(), 2.0f);

    ecs.removeComponent<MoveComponent>(e1);
    EXPECT_FALSE(ecs.hasComponent<MoveComponent>(e1));

    ecs.addComponent<MoveComponent>(e2, 3.0f, 4.0f);
    ecs.addComponent<MoveComponent>(e1, 5.0f, 6.0f);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));
    auto move2 = ecs.getComponent<MoveComponent>(e2);
    EXPECT_EQ(move2.getX(), 3.0f);
    EXPECT_EQ(move2.getY(), 4.0f);

    EXPECT_TRUE(ecs.hasComponent<MoveComponent>(e1));
    auto move3 = ecs.getComponent<MoveComponent>(e1);
    EXPECT_EQ(move3.getX(), 5.0f);
    EXPECT_EQ(move3.getY(), 6.0f);
}