#include "level_manager.hpp"
#include "director.hpp"
#include "collision_manager.hpp"
#include "render_engine.hpp"
#include "test_level.hpp"
#include "temp_level.hpp"
#include "time_manager.hpp"

#define MAX_LEVEL 2

int curLevel = 0;
bool LevelManager::levelFinished = false;
float LevelManager::timeStamp = 0; 
//public:
void LevelManager::nextLevel() {
    if(TimeManager::getTimeStamp() -timeStamp < .2)
        return;
    timeStamp = TimeManager::getTimeStamp(); 

    curLevel++;

    if(curLevel > MAX_LEVEL)
        curLevel = 1;

    switch (curLevel) { 
      case 1:
        setupFirstLevel();    
        break;
      case 2:
        removeFirstLevel();
        setupSecondLevel();
        break; 
    }
    levelFinished = false;
}

void LevelManager::resetLevel() {
    switch (curLevel) { 
      case 1:
        removeFirstLevel();
        setupFirstLevel();
        break;
      case 2:
        removeSecondLevel();
        setupSecondLevel();
        break; 
    }
}

//private:
void LevelManager::setupFirstLevel() {
    TempLevelPtr level1(new TempLevel());
    Director::addScene(level1);
    Director::setScene("testLevel.txt");
}

void LevelManager::removeFirstLevel() {
    CollisionManager::removeAllCollisionObjects();
    RenderEngine::resetRenderEngine();
    Director::removeScene("testLevel.txt");
}

void LevelManager::setupSecondLevel() {
    TestLevelPtr level2(new TestLevel());
    Director::addScene(level2);
    Director::setScene("testLevel3.txt");
}

void LevelManager::removeSecondLevel() {
    CollisionManager::removeAllCollisionObjects();
    RenderEngine::resetRenderEngine();
    Director::removeScene("testLevel3.txt");
}