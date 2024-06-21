#include <gtest/gtest.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/NameComponent.h>

class SceneTest : public testing::Test
{
protected:
    void SetUp() override
    {
        scene = new vrm::Scene();
    }

    void TearDown() override
    {
        delete scene;
    }

    vrm::Scene* scene;
};

TEST_F(SceneTest, CreateEntityNoName)
{
    for (size_t i = 0; i < 100; i++)
    {
        vrm::Entity entity = scene->createEntity();
        EXPECT_EQ(entity.hasComponent<vrm::NameComponent>(), true);
        EXPECT_EQ(entity.getComponent<vrm::NameComponent>().name, "Entity_" + std::to_string(i));
    }
}

TEST_F(SceneTest, CreateEntityWithName)
{
    vrm::Entity entity = scene->createEntity("TestEntity");
    EXPECT_EQ(entity.hasComponent<vrm::NameComponent>(), true);
    EXPECT_EQ(entity.getComponent<vrm::NameComponent>().name, "TestEntity");
}

TEST_F(SceneTest, EntityExists)
{
    vrm::Entity entity = scene->createEntity("TestEntity");
    EXPECT_EQ(scene->entityExists("TestEntity"), true);
    EXPECT_EQ(scene->entityExists("NonExistentEntity"), false);
}

TEST_F(SceneTest, CreateMultipleEntitiesWithSameName)
{
    vrm::Entity entity1 = scene->createEntity("TestEntity");
    
    EXPECT_ANY_THROW(scene->createEntity("TestEntity"));
}

TEST_F(SceneTest, CreateEntityWithEmptyName)
{
    vrm::Entity entity = scene->createEntity("");
    EXPECT_EQ(entity.hasComponent<vrm::NameComponent>(), true);
    EXPECT_EQ(entity.getComponent<vrm::NameComponent>().name, "");
}

TEST_F(SceneTest, GetEntityByHandle)
{
    vrm::Entity entity = scene->createEntity();
    vrm::Entity entity2 = scene->getEntity(entity);
    EXPECT_EQ(entity, entity2);
}

TEST_F(SceneTest, GetEntityByHandleInvalid)
{
    EXPECT_ANY_THROW(scene->getEntity(vrm::Entity()));
}

TEST_F(SceneTest, GetEntityByName)
{
    vrm::Entity entity = scene->createEntity("TestEntity");
    vrm::Entity entity2 = scene->getEntity("TestEntity");
    EXPECT_EQ(entity, entity2);
}

TEST_F(SceneTest, GetEntityByNameInvalid)
{
    EXPECT_ANY_THROW(scene->getEntity("NonExistentEntity"));
}

TEST_F(SceneTest, DestroyEntity)
{
    vrm::Entity entity = scene->createEntity("TestEntity");
    EXPECT_NO_THROW(scene->destroyEntity(entity));
    EXPECT_ANY_THROW(scene->getEntity("TestEntity"));
}

TEST_F(SceneTest, DestroyEntityTwice)
{
    vrm::Entity entity = scene->createEntity("TestEntity");
    EXPECT_NO_THROW(scene->destroyEntity(entity));
    EXPECT_ANY_THROW(scene->destroyEntity(entity));

}

TEST_F(SceneTest, GetEntityByDestroyedHandle)
{
    vrm::Entity entity = scene->createEntity();
    entt::entity handle = entity;
    scene->destroyEntity(entity);
    EXPECT_ANY_THROW(scene->getEntity(handle));
}