#include <Scene.h>
#include <iostream>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Renderer.h>
#include <iostream>

Scene::Scene() { }
Scene::~Scene() { }

Scene* Scene::instance = nullptr;
Scene* Scene::getInstance() {
    if (instance == nullptr) {
        instance = new Scene();
    }
    return instance;
}

void Scene::renderWrapper() {
    Scene::getInstance()->render();
}

void Scene::normalKeyWrapper(unsigned char key, int x, int y) {
    Scene::getInstance()->processNormalKeys(key, x, y);
}

void Scene::specialKeyWrapper(int key, int x, int y) {
    Scene::getInstance()->processSpecialKeys(key, x, y);
}

void Scene::mouseMoveWrapper(int x, int y) {
    Scene::getInstance()->processMouseMovement(x, y);
}

void Scene::cleanupWrapper() {
    Scene::getInstance()->cleanup();
}

void Scene::start(int *argc, char **argv) {
    // set the display mode
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_SINGLE| GLUT_RGB | GLUT_DEPTH);
    // initialize glut
    glutInit(argc, argv);
    // create the window
    glutInitWindowSize(800, 600);
    glutCreateWindow("Labyrinth"); 
    // glutFullScreen(); 
    // initialize the program 
    this->init();
    // set the display function
    GLCall(glutDisplayFunc(renderWrapper));
    // set the reshape function
    GLCall(glutIdleFunc(renderWrapper));
    // set the keyboard function
    GLCall(glutKeyboardFunc(normalKeyWrapper));
    // set the special keyboard function
    GLCall(glutSpecialFunc(specialKeyWrapper));
    // set the mouse function
    GLCall(glutPassiveMotionFunc(mouseMoveWrapper));
    // set the menu function
    GLCall(glutCreateMenu(NULL));
    // set close function
    GLCall(glutWMCloseFunc(cleanupWrapper));
    // main loop
    GLCall(glutMainLoop());
}

// initialize the program
void Scene::init(void)
{
    GLCall(glutSetCursor(GLUT_CURSOR_NONE));
    const GLfloat PI = 3.141592;
    // set the background color white
    GLCall(glClearColor(1.0f, 1.0f, 1.0f, 0.0f));
    int mazeSize = 8, padding = 10, floorSize;
    maze = new Maze(mazeSize);
    maze->generate(0, 0);
    mazeSize += 2;
    floorSize = mazeSize * 2 + padding * 2;
    maze->create();
    maze->print();
    WALL **mazeArray = maze->getMaze();
    int **corners, **horizontalWalls, **verticalWalls, **cubes, **floor, **collisionMatrix;

    floor = new int*[floorSize];
    corners = new int*[mazeSize];
    horizontalWalls = new int*[mazeSize];
    verticalWalls = new int*[mazeSize];
    collisionMatrix = new int*[2 * mazeSize];
    cubes = new int*[mazeSize];
    for (int i = 0; i < 2 * mazeSize ; i++) {
        collisionMatrix[i] = new int[2 * mazeSize];
    }
    for (int i = 0; i <floorSize; i++) {
        floor[i] = new int[mazeSize + 2 * padding];
    }
    for (int i = 0; i < mazeSize; i++) {
        corners[i] = new int[mazeSize];
        horizontalWalls[i] = new int[mazeSize];
        verticalWalls[i] = new int[mazeSize];
        cubes[i] = new int[mazeSize];
    }

    for (int i = 0; i < 2 * mazeSize ; i++)
        for (int j = 0; j < 2 * mazeSize ; j++)
            collisionMatrix[i][j] = 0;

    for (int i = 0; i <floorSize; i++) {
        for (int j = 0; j <floorSize; j++) {
            floor[i][j] = 1;
        }
    }

    for(int i = 0; i < mazeSize; i++) {
        for(int j = 0; j < mazeSize; j++) {
            corners[i][j] = 0;
            horizontalWalls[i][j] = 0;
            verticalWalls[i][j] = 0;
            cubes[i][j] = 0;
        }
    }

    for(int i = 0; i < mazeSize; i++) {
        for(int j = 0; j < mazeSize; j++) {
            std :: cout << mazeArray[i][j] << ' ';
            switch(mazeArray[i][j]){
            case HORIZONTAL:
                horizontalWalls[i][j] = 1;
                collisionMatrix[2 * i][2 * j] = 1;
                if(i != 0) {
                    collisionMatrix[2 * i - 1][2 * j] = 1;
                }
                else
                    collisionMatrix[2 * i][2 * j + 1] = 1;
                break;
            case VERTICAL:
                verticalWalls[i][j] = 1;
                collisionMatrix[2 * i][2 * j] = 1;
                collisionMatrix[2 * i][2 * j + 1] = 1;
            case CORNER:
                corners[i][j] = 1;
                collisionMatrix[2 * i][2 * j] = 1;
                collisionMatrix[2 * i + 1][2 * j] = 1;
                collisionMatrix[2 * i + 1][2 * j + 1] = 1;
                break;
            case CUBE:
                cubes[i][j] = 1;
                collisionMatrix[2 * i][2 * j] = 1;
                collisionMatrix[2 * i + 1][2 * j] = 1;
                collisionMatrix[2 * i + 1][2 * j + 1] = 1;
                break;
            default:
                break;
            }
        }
        std :: cout << std :: endl;
    }

    // print collision matrix
    std :: cout << "\n\nCollision Matrix:" << std :: endl;
    for(int i = 0; i < 2 * mazeSize - 1; i++) {
        for(int j = 0; j < 2 * mazeSize - 1; j++) {
            std :: cout << collisionMatrix[i][j] << ' ';
        }
        std :: cout << std :: endl;
    }

    // print horizontals
    std :: cout << "\n\nHorizontals:" << std :: endl;
    for(int i = 0; i < mazeSize; i++) {
        for(int j = 0; j < mazeSize; j++) {
            std :: cout << horizontalWalls[i][j] << ' ';
        }
        std :: cout << std :: endl;
    }

    // print verticals
    std :: cout << "\n\nVerticals:" << std :: endl;
    for(int i = 0; i < mazeSize; i++) {
        for(int j = 0; j < mazeSize; j++) {
            std :: cout << verticalWalls[i][j] << ' ';
        }
        std :: cout << std :: endl;
    }

    GLfloat wall[] =
	{
		0.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f,

        0.0f,  0.0f, 0.5f, 1.0f,
        1.0f,  0.0f, 0.5f, 1.0f,
        1.0f,  1.0f, 0.5f, 1.0f,
        0.0f,  1.0f, 0.5f, 1.0f,
    };

    GLfloat cube[] =
	{
		0.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f,

        0.0f,  0.0f, 1.0f, 1.0f,
        1.0f,  0.0f, 1.0f, 1.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
        0.0f,  1.0f, 1.0f, 1.0f,
    };

    GLfloat corner[] = 
    {
		0.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f,

        0.0f,  0.0f, 0.5f, 1.0f, // 0
        1.0f,  0.0f, 0.5f, 1.0f,
        1.0f,  1.0f, 0.5f, 1.0f,
        0.0f,  1.0f, 0.5f, 1.0f, // 3

        0.5f,  0.0f, 0.5f, 1.0f, // 1
        0.5f,  1.0f, 0.5f, 1.0f, // 2

        0.0f,  0.0f, 1.0f, 1.0f, // 4 
        0.5f,  0.0f, 1.0f, 1.0f, // 5
        0.5f,  1.0f, 1.0f, 1.0f, // 6
        0.0f,  1.0f, 1.0f, 1.0f, // 7
    };

    GLfloat tile[] = 
    {
        0.0f,  -0.25f, 0.0f, 1.0f,
        1.0f,  -0.25f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, 1.0f,

        0.0f,  -0.25f, 1.0f, 1.0f,
        1.0f,  -0.25f, 1.0f, 1.0f,
        1.0f,  0.0f, 1.0f, 1.0f,
        0.0f,  0.0f, 1.0f, 1.0f,
    };

	GLuint wallIndices[] =
	{
        1, 2, 0,   0, 2, 3, 
        2, 3, 6,   6, 3, 7,
        7, 3, 4,   4, 3, 0,
        4, 0, 5,   5, 0, 1,
        1, 2, 5,   5, 2, 6,
        5, 6, 4,   4, 6, 7, 
    };

	// indicii pentru varfuri
	GLuint cornerIndices[] =
	{
	    1, 2, 0,   0, 2, 3, 
	    2, 3, 6,   6, 3, 7,
	    7, 3, 4,   4, 3, 0,
	    4, 0, 5,   5, 0, 1,
	    1, 2, 5,   5, 2, 6,
	    5, 6, 4,   4, 6, 7, 

        8, 9, 4,   4, 9, 7, 
        9, 7, 12,   12, 7, 13,
        13, 7, 10,   10, 7, 4,
        10, 4, 11,   11, 4, 8,
        8, 9, 11,   11, 9, 12,
        11, 12, 10,   10, 12, 13, 
	};
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projectionMatrix = glm::infinitePerspective(PI / 2.0f, (float) glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01f);

    glm::mat4 scaledMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
    floorRenderer = new Renderer(tile, wallIndices, sizeof(tile), sizeof(wallIndices) / sizeof(GLuint));
    floorRenderer->instance(floor,floorSize, floorSize, 1, glm::translate(glm::mat4(1.0f), glm::vec3(-padding, -0.5f, -padding)));
    cubeRenderer = new Renderer(cube, wallIndices, sizeof(cube), sizeof(wallIndices) / sizeof(GLuint));
    cubeRenderer->instance(cubes, mazeSize, mazeSize, 2, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)));
    cornerRenderer = new Renderer(corner, cornerIndices, sizeof(corner), sizeof(cornerIndices) / sizeof(GLuint));
    cornerRenderer->instance(corners, mazeSize, mazeSize, 2, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0)) 
        * scaledMatrix);
    verticalWallRenderer = new Renderer(wall, wallIndices, sizeof(wall), sizeof(wallIndices) / sizeof(GLuint));
    verticalWallRenderer->instance(verticalWalls, mazeSize, mazeSize, 2, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) 
        * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0)) 
        * scaledMatrix);
    horizontalWallRenderer = new Renderer(wall, wallIndices, sizeof(wall), sizeof(wallIndices) / sizeof(GLuint));
    horizontalWallRenderer->instance(horizontalWalls, mazeSize, mazeSize, 2, scaledMatrix);

    for (int i = 0; i < 2 * mazeSize ; i++) {
        delete collisionMatrix[i];
    }
    for (int i = 0; i <floorSize; i++) {
        delete floor[i];
    }
    for (int i = 0; i < mazeSize; i++) {
        delete corners[i];
        delete horizontalWalls[i];
        delete verticalWalls[i];
        delete cubes[i];
    }

    delete corners;
    delete horizontalWalls;
    delete verticalWalls;
    delete cubes;
    delete floor;
    delete collisionMatrix;
}

// render the program
void Scene::render(void)
{  
    
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    viewMatrix = camera->getViewMatrix();

    floorRenderer->drawInstanced(viewMatrix, projectionMatrix);
    cubeRenderer->draw(viewMatrix, projectionMatrix);
    cornerRenderer->draw(viewMatrix, projectionMatrix);
    verticalWallRenderer->drawInstanced(viewMatrix, projectionMatrix);
    horizontalWallRenderer->drawInstanced(viewMatrix, projectionMatrix);

    GLCall(glutSwapBuffers());
    GLCall(glFlush());

}



// process normal keys
void Scene::processNormalKeys(unsigned char key, int x, int y)
{
    float cameraSpeed = 0.1f;
    if(key == 'w')
    {
        camera->processKeyboard(UP, cameraSpeed);
    }
    else if(key == 's')
    {
       camera->processKeyboard(DOWN, cameraSpeed);
    }
    else if(key == 'a')
    {
       camera->processKeyboard(LEFT, cameraSpeed);
    }
    else if(key == 'd')
    {
       camera->processKeyboard(RIGHT, cameraSpeed);
    }
}

// process special keys
void Scene::processSpecialKeys(int key, int x, int y)
{
    float cameraSpeed = 0.1f;
    if(key == 27)
    {
        exit(0);
    }
    if(key == GLUT_KEY_UP)
    {
       camera->processKeyboard(UP, cameraSpeed);
    }
    else if(key == GLUT_KEY_DOWN)
    {
       camera->processKeyboard(DOWN, cameraSpeed);
    }
    else if(key == GLUT_KEY_LEFT)
    {
       camera->processKeyboard(LEFT, cameraSpeed);
    }
    else if(key == GLUT_KEY_RIGHT)
    {
       camera->processKeyboard(RIGHT, cameraSpeed);
    }
}

void Scene::processMouseMovement(int x, int y)
{
    camera->processMouseMovement(x, y);
}

// cleanup the program
void Scene::cleanup(void)
{
    delete camera;
    delete verticalWallRenderer;
    delete horizontalWallRenderer;
    delete cornerRenderer;
    delete cubeRenderer;
}