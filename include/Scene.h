#pragma once 
#include <VAO.h>
#include <VBO.h>
#include <EBO.h>
#include <Shader.h>
#include <Camera.h>
#include <Maze.h>
#include <Renderer.h>
#include <errorHandle.h>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <OpenGL/glu.h>

class Scene {
private:
    Camera *camera;
    static Scene *instance;
    Maze *maze;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;
    Renderer *verticalWallRenderer;
    Renderer *horizontalWallRenderer;
    Renderer *floorRenderer;
    Renderer *cornerRenderer;
    Renderer *cubeRenderer;
public:
    Scene();
    ~Scene();

    static Scene* getInstance();
    static void renderWrapper();
    static void normalKeyWrapper(unsigned char key, int x, int y);
    static void specialKeyWrapper(int key, int x, int y);
    static void mouseMoveWrapper(int x, int y);
    static void cleanupWrapper();

    void start(int *argc, char **argv);
    void init();
    void render();
    void processNormalKeys(unsigned char key, int x, int y);
    void processSpecialKeys(int key, int x, int y);
    void processMouseMovement(int xoffset, int yoffset);
    void cleanup();
};