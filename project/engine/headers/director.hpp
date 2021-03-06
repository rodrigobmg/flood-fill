#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

/**
* Director Class
*    This class controls all scenes. And also choses which scene is going to be
*    presented in the moment.
*    The scope of this class is Global.
*/

#include <string>
#include <map>

#include "scene.hpp"

#include "pointer_macros.h"
DEF_PTR(Director, DirectorPtr);

class Director{

public:
    static void addScene(ScenePtr scene);
    static void removeScene(std::string label);

    //Returns a specific scene
    static ScenePtr getScene(std::string label);

    //Returns the current scene
    static ScenePtr getScene();

    static void setScene(std::string label);

    static void updateScene();
    static void renderScene();

private:
    static const std::string NO_VALID_SCENE;

    static std::string currentScene;
    static std::map<std::string, ScenePtr> scenes;

};

#endif
