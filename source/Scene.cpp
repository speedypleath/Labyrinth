#include <Scene.h>
#include <iostream>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_SINGLE|GLUT_RGB);
    // initialize glut
    glutInit(argc, argv);
    // create the window
    glutCreateWindow("Labyrinth"); 
    glutFullScreen(); 
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
    glutSetCursor(GLUT_CURSOR_NONE);
    const GLfloat PI = 3.141592;
    // set the background color white
    GLCall(glClearColor(1.0f, 1.0f, 1.0f, 0.0f));
    maze = new Maze(7);
    maze->generate(0, 0);
    maze->print();
    instanceCount = maze->countVerticalWalls() + maze->countHorizontalWalls();
    int **verticalWalls = maze->getVerticalWalls();
    int **horizontalWalls = maze->getHorizontalWalls();
    // create a cube
    GLfloat Vertices[] =
	{
		// punctele din planul z=-50   coordonate                   		
		0.0f,  -50.0f, 0.0f, 1.0f,
		200.0f,  -50.0f,  0.0f, 1.0f,
		200.0f,  50.0f,  0.0f, 1.0f,
		0.0f,  50.0f, 0.0f, 1.0f,
		// punctele din planul z=+50  coordonate                   		
		0.0f,  -50.0f, 30.0f, 1.0f,
		200.0f,  -50.0f,  30.0f, 1.0f,
		200.0f,  50.0f,  30.0f, 1.0f,
		0.0f,  50.0f, 30.0f, 1.0f,
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

    	// Culorile instantelor
	glm::vec4 Colors[instanceCount];
	for (int n = 0; n < instanceCount; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		Colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		Colors[n][3] = 1.0f;
	}
    // for (int n = 0; n < maze->countVerticalWalls(); n++)
    //     Colors[n] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // for (int n = 0; n < maze->countHorizontalWalls(); n++)
    //     Colors[n + maze->countVerticalWalls()] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    int n = 0;
	glm::mat4 MatModel[instanceCount];
    for (int i = 0; i < 7; i++)
	    for (int j = 0; j < 7; j++)
            if(verticalWalls[i][j] == 1)
		        MatModel[n++] = glm::translate(glm::mat4(1.0f), glm::vec3(200 * i, 0.0, 200 * j));

    for (int i = 0; i < 7; i++)
	    for (int j = 0; j < 7; j++)
            if(horizontalWalls[i][j] == 1)
                MatModel[n++] =  glm::translate(glm::mat4(1.0f), glm::vec3(200 * i, 0.0, 200 * j)) * glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0.0, 1.0, 0.0));
    // position vbo
    vao = new VAO();
    ebo = new EBO(Indices, sizeof(Indices) / sizeof(GLuint));
    positionVBO = new VBO(Vertices, sizeof(Vertices));
    colorVBO = new VBO(Colors, sizeof(Colors));
    instanceVBO = new VBO(MatModel, sizeof(MatModel));
    shader = new Shader("resource/shader.vert", "resource/shader.frag");
    vao->addBufferVec4(*positionVBO, false);
    vao->addBufferVec4(*colorVBO, true);
    vao->addBufferMat4(*instanceVBO);

    camera = new Camera(glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), verticalWalls, horizontalWalls);
    projectionMatrix = glm::infinitePerspective(PI / 2.0f, GLfloat(800.0f) / GLfloat(600.0f), 1.0f);
}

// render the program
void Scene::render(void)
{  
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));
    viewMatrix = camera->getViewMatrix();
    // glEnable(GL_BLEND);  // enable blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int codCol = 0;
    shader->setInt("codCol", codCol);
    shader->setMat4("viewMatrix", viewMatrix);
    shader->setMat4("projectionMatrix", projectionMatrix);
	GLCall(glDrawElementsInstancedARB(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instanceCount));

    GLCall(glutSwapBuffers());
    GLCall(glFlush());

}



// process normal keys
void Scene::processNormalKeys(unsigned char key, int x, int y)
{
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
    delete vao;
    delete positionVBO;
    delete colorVBO;
    delete instanceVBO;
    delete shader;
    delete ebo;
    delete camera;
}