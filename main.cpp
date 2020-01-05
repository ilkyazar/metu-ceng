#include "helper.h"
#include "glm/glm.hpp"

static GLFWwindow* window = NULL;

GLuint idJpegTexture;
int widthTexture, heightTexture;

//vertices of triangles
glm::vec3* vertices;


static void errorCallback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

//keyboard events will be called here
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);    
}

void customizedRenderFunction(){
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // color state is red
    glVertex3f(0, 0.5, 0); // first vertex
    glColor3f(0.0, 1.0, 0.0); // color state is green
    glVertex3f(-0.5, -0.5, 0); // second vertex
    glColor3f(0.0, 0.0, 1.0); //color state is blue
    glVertex3f(0.5, -0.5, 0); // third vertex
    glEnd();
}

void createTriangles(){
    glm::vec3 vertex;
    //int vertex_count = 3 * 2 * widthTexture * heightTexture;
    std::vector<glm::vec3> vertices;
    //TODO: create vertices of 2 triangles for each pixel
    
    for(int i=0; i <  widthTexture; i++){
        for(int j=0; j < heightTexture; j++){
            vertex = glm::vec3(i,0,j);
            vertices.push_back(vertex);
            vertex = glm::vec3(i+1,0,j+1);
            

        }
    }
}

int main(int argc, char * argv[]){
    std::cout << "height map image: " << argv[1] << std::endl;
    std::cout << "texture map image: " << argv[2] << std::endl;

    glfwSetErrorCallback(errorCallback);

    if(!glfwInit()){
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    //glfwCreateWindow(width, height,...)
    window = glfwCreateWindow(1000, 1000, "CENG477 - HW3", NULL, NULL); 
    if (!window){
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    initTexture(argv[2], &widthTexture, &heightTexture);
    std::cout << "texture initialized" << std::endl;

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(-1);
    }

    while( !glfwWindowShouldClose(window) ){
        glfwWaitEvents();
        customizedRenderFunction();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}