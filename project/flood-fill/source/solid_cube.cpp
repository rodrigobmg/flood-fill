#include "solid_cube.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "collision_manager.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

SolidCube::SolidCube(glm::vec3 _position) : GameObject(), CollisionObject(_position),
    position(_position) {}

void SolidCube::setup() {
    INFO("Creating a Solid Cube ...");

    cube = new Object(
               LoadManager::getMesh("cube.obj"),
               MaterialManager::getMaterial("FlatGrey"));

    cube->applyTexture(LoadManager::getTexture("VoxelTexture"));
    cube->enableTexture();

    cube->translate(position);

    RenderEngine::getRenderElement("textured")->addObject(cube);

    setCollisionID(1);
    setCanCollide(true);

    setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
    getBoundingBox()->setPosition(position);
}

void SolidCube::update(){
}

void SolidCube::collided(CollisionObject *){
}
