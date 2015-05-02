#include "test_level.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"
#include "global_variables.hpp"
#include "collision_manager.hpp"

TestLevel::TestLevel() : LevelTemplate("testLevel.txt"){}

void TestLevel::setup(){
    INFO("Generanting Test Level...");
    createLevel();

    INFO("Setting up the cameras for the Test Level...");
    Camera * cam1 = new Camera(glm::vec3(1, 8, -3), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");

    Camera * cam2 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    INFO("Setting up the player for the Test Level...");
    player = new Player(cam1);
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    addCamera("DebugCamera", cam2);
}

void TestLevel::update(){
    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else{
        setMainCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }
}