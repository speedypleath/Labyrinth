#include <Scene.h>
#include <iostream>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Renderer.h>

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
    int mazeSize = 8;
    maze = new Maze(mazeSize);
    maze->generate(0, 0);
    maze->create();
    maze->print();
    int **verticalWalls = maze->getVerticalWalls();
    int **horizontalWalls = maze->getHorizontalWalls();
    char **mazeMap = maze->getMaze();
    // create a cube
    GLfloat wall[] =
	{
		// punctele din planul z=-50   coordonate                   		
		-100.0f,  -50.0f, 0.0f, 1.0f,
		100.0f,  -50.0f,  0.0f, 1.0f,
		100.0f,  50.0f,  0.0f, 1.0f,
		-100.0f,  50.0f, 0.0f, 1.0f,
		// punctele din planul z=+50  coordonate                   		
		-100.0f,  -50.0f, 70.0f, 1.0f,
		100.0f,  -50.0f,  70.0f, 1.0f,
		100.0f,  50.0f,  70.0f, 1.0f,
		-100.0f,  50.0f, 70.0f, 1.0f,
    };

    GLfloat verticalWall[] =
	{
		// punctele din planul z=-50   coordonate                   		
		0.0f,  -50.0f, 200.0f, 1.0f,
		0.0f,  -50.0f,  0.0f, 1.0f,
		0.0f,  50.0f,  0.0f, 1.0f,
		0.0f,  50.0f, 200.0f, 1.0f,
		// punctele din planul z=+50  coordonate                   		
		100.0f,  -50.0f, 0.0f, 1.0f,
		100.0f,  -50.0f,  200.0f, 1.0f,
		100.0f,  50.0f,  200.0f, 1.0f,
		100.0f,  50.0f, 0.0f, 1.0f,
    };

	// indicii pentru varfuri
	GLuint Indices[] =
	{
	  1, 2, 0,   0, 2, 3,  //  Fata "de jos"
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7, //  Fata "de sus"
	  0, 1, 2, 3,  // Contur fata de jos
	  4, 5, 6, 7,  // Contur fata de sus
	  0, 4,
	  1, 5,
	  2, 6,
	  3, 7
	};

    verticalWallRenderer = new Renderer(wall, Indices, sizeof(wall), sizeof(Indices) / sizeof(GLuint));
    verticalWallRenderer->instance(horizontalWalls, mazeSize, mazeSize, glm::mat4(1.0f));
    horizontalWallRenderer = new Renderer(wall, Indices, sizeof(wall), sizeof(Indices) / sizeof(GLuint));
    horizontalWallRenderer->instance(verticalWalls, mazeSize, mazeSize, glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0)));
    camera = new Camera(glm::vec3(-100.0f, 0.0f, 350.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), verticalWalls, horizontalWalls);
    projectionMatrix = glm::infinitePerspective(PI / 2.0f, GLfloat(800.0f) / GLfloat(600.0f), 1.0f);
}

// render the program
void Scene::render(void)
{  
    
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));
    viewMatrix = camera->getViewMatrix();
    projectionMatrix = glm::infinitePerspective(glm::pi<float>() / 2.0f, GLfloat(800.0f) / GLfloat(600.0f), 1.0f) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0)) 
                * glm::translate(glm::mat4(1.0f), glm::vec3(-100.0, 0.0, 30.0));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc (GL_GREATER, 0.1);
    glEnable(GL_ALPHA_TEST);

    // int codCol = 0;
    // shader->setInt("codCol", codCol);
    // shader->setMat4("viewMatrix", viewMatrix);
    // shader->setMat4("projectionMatrix", projectionMatrix);

    
    verticalWallRenderer->drawInstanced(viewMatrix, projectionMatrix);
    horizontalWallRenderer->drawInstanced(viewMatrix, projectionMatrix);
	// GLCall(glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instanceCount));

    GLCall(glutSwapBuffers());
    GLCall(glFlush());

}



// process normal keys
void Scene::processNormalKeys(unsigned char key, int x, int y)
{
    maze->print();
    if(key == 'w')
    {
        camera->processKeyboard(UP, 8.0f);
    }
    else if(key == 's')
    {
       camera->processKeyboard(DOWN, 8.0f);
    }
    else if(key == 'a')
    {
       camera->processKeyboard(LEFT, 8.0f);
    }
    else if(key == 'd')
    {
       camera->processKeyboard(RIGHT, 8.0f);
    }
}

// process special keys
void Scene::processSpecialKeys(int key, int x, int y)
{
    if(key == 27)
    {
        exit(0);
    }
    if(key == GLUT_KEY_UP)
    {
       camera->processKeyboard(UP, 8.0f);
    }
    else if(key == GLUT_KEY_DOWN)
    {
       camera->processKeyboard(DOWN, 8.0f);
    }
    else if(key == GLUT_KEY_LEFT)
    {
       camera->processKeyboard(LEFT, 8.0f);
    }
    else if(key == GLUT_KEY_RIGHT)
    {
       camera->processKeyboard(RIGHT, 8.0f);
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
}