#include "tutorial_level.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"
#include "switch.hpp"
#include "global_variables.hpp"
#include "collision_manager.hpp"
#include "object.hpp"
#include "render_engine.hpp"
#include "load_manager.hpp"
#include "material_manager.hpp"
#include "regular_polygons_render.hpp"
#include "textured_polygons_render.hpp"
#include "water_stream_render.hpp"
#include "water_render.hpp"
#include "water_particle_render.hpp"
#include "normal_map_render.hpp"
#include "camera_polygons_render.hpp"
#include "debug_render.hpp"
#include "active_terrain.hpp"
#include "render_grid.hpp"
#include "shadow_occluder_render.hpp"
#include "normal_map_border_render.hpp"
#include "time_manager.hpp"
#include "text.hpp"
#include "text_render.hpp"
#include "menu.hpp"

TutorialLevel::TutorialLevel() : LevelTemplate("testLevel4.txt"), timer(0.0f) {}

void TutorialLevel::setup(){
    INFO("Generating Tutorial Level...");

    readFile();
    initalizeGrid();
    // createRenders();
    createMenuRenders();

    createLevel();

    Uniform3DGridPtr<int> typeGrid = getTypeGrid();
    gridCenter = glm::vec3((typeGrid->getMaxX() - typeGrid->getMinX())/2.0f,
                           (typeGrid->getMaxY() - typeGrid->getMinY())/2.0f,
                           (typeGrid->getMinZ() - typeGrid->getMaxZ())/2.0f);

    waterSurfaceManager = WaterSurfaceManagerPtr(new WaterSurfaceManager());
    addGameObject(waterSurfaceManager);
    
    INFO("Removal String so less of make");
    INFO("Setting up the cameras for the Test Level...");
    CameraPtr cam1(new Camera(glm::vec3(4, 10, -5), gridCenter,
                             glm::vec3(0, 1, 0)));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");
    setCullingCamera("Camera1");
    
    CameraPtr cam2(new Camera(glm::vec3(0, 1, 0), glm::vec3(-6, -3, 6),
                             glm::vec3(0, 1, 0)));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("DebugCamera", cam2);

    l1 = LightPtr(new Light(glm::vec3(1), 30.0f, glm::vec3(0, 30, 0)));
    l1->setPosition(l1->getDirection()*1.0f);
    l1->setViewMatrix(glm::lookAt(
        l1->getDirection(),
        gridCenter, glm::vec3(0, 1, 0)));
    l1->setProjectionMatrix(glm::ortho<float>(-100,100,-100,100,-100,100));

    addLight("Sun", l1);
}

void TutorialLevel::update(){
    if (Menu::isNewLevel()) {
        createLevelRenders();
        //unfix camera and move to starting location
        getCamera("Camera1")->setEye(glm::vec3(4, 10, -5));
        getCamera("Camera1")->setTarget(glm::vec3(4,4, -10));
        //Text
        levelTitle = TextPtr(new Text("Level1", glm::vec4(0, 0, 0, 1), glm::vec2(0, 0), "FourPixel", 75));
        levelTitle->setPosition(glm::vec2(0 - levelTitle->getTextWidth()/2.0, 0));
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText(levelTitle);
    
        // Important stuff
        INFO("Setting up the player for the Test Level...");
        player = PlayerPtr(new Player(getCamera("Camera1"), 2));
        player->setup();
        getCamera("Camera1")->fix();
        addGameObject("player" , player);
        CollisionManager::addCollisionObjectToList(player);
        
        
        debugPlayer = DebugPlayerPtr(new DebugPlayer(getCamera("DebugCamera")));
        debugPlayer->setup();
        addGameObject("debugPlayer" , debugPlayer);   

        Menu::setNewLevel(false);
    } else {
        
        if(debugPlayer->isActive()){
            ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
            setMainCamera("DebugCamera");
            getCamera("Camera1")->fix();
        } else {
            setMainCamera("Camera1");
            getCamera("Camera1")->fix(false, true, false);
        }
        
        
        timer += TimeManager::getDeltaTime() / 15.0;
        l1->setDirection(glm::vec3(20.0 * sin(timer * 3.1), 5.0 * sin(timer * 3.4 + 5.0) + 30.0, 20.0 * sin(timer * 3.8 + 2.0)));
        l1->setPosition(gridCenter + l1->getDirection()*1.0f);
        l1->setViewMatrix(glm::lookAt(
                              gridCenter + l1->getDirection(),
                              gridCenter, glm::vec3(0, 1, 0)));

        if (!Menu::isActive()) {
            glm::vec4 titleColor = levelTitle->getColor();
            if(titleColor.w > 0){
                titleColor.w -= TimeManager::getDeltaTime()*0.3;
                levelTitle->setColor(titleColor);
            }
        } else {
            getCamera("Camera1")->setEye(glm::vec3((glm::sin(timer) + 1) * typeGrid->getMaxX()/2.0, typeGrid->getMaxY()+10, (glm::cos(timer) + 1) * -(typeGrid->getMinZ())));
        }
    }
}

/*void TutorialLevel::createRenders(){
    INFO("Creating Renders...");

    RenderEngine::addRenderElement("camera", RenderElementPtr(new CameraPolygonsRender()), 1);
    RenderEngine::addRenderElement("regular", RenderElementPtr(new RegularPolygonsRender()), 1);
    RenderEngine::addRenderElement("debug", RenderElementPtr(new DebugRender()), -5);
    RenderEngine::addRenderElement("normalmap", RenderElementPtr(new NormalMapRender()), 1);
    RenderEngine::addRenderElement("normalmap-border", RenderElementPtr(new NormalMapBorderRender()), 1);
    RenderEngine::addRenderElement("textured", RenderElementPtr(new TexturedPolygonsRender()), 1);
    RenderEngine::addRenderElement("water", RenderElementPtr(new WaterRender()), 4);
    RenderEngine::addRenderElement("water-particle", RenderElementPtr(new WaterParticleRender()), 4);
    RenderEngine::addRenderElement("water-stream", RenderElementPtr(new WaterStreamRender()), 4);
    RenderEngine::addRenderElement("shadow", RenderElementPtr(new ShadowOccluderRender()), 1);
    RenderEngine::addRenderElement("text", RenderElementPtr(new TextRender()), 10);

    RenderEngine::setRenderGrid(RenderGridPtr(new RenderGrid(typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
                                               typeGrid->getMinX(), typeGrid->getMaxX(),
                                               typeGrid->getMinY(), typeGrid->getMaxY(),
                                               typeGrid->getMinZ(), typeGrid->getMaxZ())));
                                               }*/

void TutorialLevel::createMenuRenders(){
    INFO("Creating Renders...");

    RenderEngine::addRenderElement("regular", RenderElementPtr(new RegularPolygonsRender()), 1);

    RenderEngine::addRenderElement("normalmap", RenderElementPtr(new NormalMapRender()), 1);
    RenderEngine::addRenderElement("normalmap-border", RenderElementPtr(new NormalMapBorderRender()), 1);
    RenderEngine::addRenderElement("textured", RenderElementPtr(new TexturedPolygonsRender()), 1);

    RenderEngine::addRenderElement("shadow", RenderElementPtr(new ShadowOccluderRender()), 1);

    RenderEngine::setRenderGrid(RenderGridPtr(new RenderGrid(typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
                                               typeGrid->getMinX(), typeGrid->getMaxX(),
                                               typeGrid->getMinY(), typeGrid->getMaxY(),
                                               typeGrid->getMinZ(), typeGrid->getMaxZ())));
}



void TutorialLevel::createLevelRenders(){

    RenderEngine::addRenderElement("debug", RenderElementPtr(new DebugRender()), -5);
    RenderEngine::addRenderElement("camera", RenderElementPtr(new CameraPolygonsRender()), 1);
    RenderEngine::addRenderElement("water", RenderElementPtr(new WaterRender()), 4);
    RenderEngine::addRenderElement("water-particle", RenderElementPtr(new WaterParticleRender()), 4);
    RenderEngine::addRenderElement("water-stream", RenderElementPtr(new WaterStreamRender()), 4);
    RenderEngine::addRenderElement("text", RenderElementPtr(new TextRender()), 10);
}
