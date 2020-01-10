#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


static GLFWwindow* window = NULL;

GLuint textureId;
GLuint heightTextureId;

GLuint samplerId;
GLuint heightSamplerId;


int widthTexture, heightTexture;

GLfloat heightFactor = 10.0f;

GLuint programId;
GLuint vertexShaderId;
GLuint fragmentShaderId;

//vertices of triangles
std::vector<glm::vec3> vertices;

//camera stuff
glm::vec3 camera_position;
glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 camera_gaze = glm::vec3(0.0, 0.0, 1.0);


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
    
    for(int i=0; i <  widthTexture; i++){
        for(int j=0; j < heightTexture; j++){
            //1st triangle
            vertex = glm::vec3(i,0,j);
            vertices.push_back(vertex);
            vertex = glm::vec3(i+1,0,j+1);
            vertices.push_back(vertex);
            vertex = glm::vec3(i,0,j+1);
            vertices.push_back(vertex);

            //2nd triangle
            vertex = glm::vec3(i,0,j);
            vertices.push_back(vertex);
            vertex = glm::vec3(i+1,0,j);
            vertices.push_back(vertex);
            vertex = glm::vec3(i+1,0,j+1);
            vertices.push_back(vertex);

        }
    }
}

void setTextures(){

    glUniform1i(glGetUniformLocation(programId, "texture1"), 0);
    glUniform1i(glGetUniformLocation(programId, "texture2"), 1);

    glUniform1i(glGetUniformLocation(programId, "widthTexture"), widthTexture);
    glUniform1i(glGetUniformLocation(programId , "heightTexture"), heightTexture);
    glUniform1f(glGetUniformLocation(programId, "heightFactor"), heightFactor);


}

void cameraTransformation(){
    // There will be perspective projection with an angle of 45 degrees. The aspect ratio will be 1,
    // near and far plane will be 0.1 and 1000 respectively.
    glm::mat4 projection =  glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
    glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_gaze * 0.1f, camera_up);
    glm::mat4 model = glm::mat4(1.0f);

    // Uniform variables must be set by the main program

    // Query their location
    GLint modelingMatrixLoc = (GLint)glGetUniformLocation(programId, "modelingMatrix");
    GLint viewingMatrixLoc = (GLint)glGetUniformLocation(programId, "viewingMatrix");
    GLint projectionMatrixLoc = (GLint)glGetUniformLocation(programId, "projectionMatrix");
    GLint cameraPositionLoc = (GLint)glGetUniformLocation(programId, "cameraPos");
    // Set the variables 
    glUniformMatrix4fv(modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(cameraPositionLoc, 1, GL_FALSE, glm::value_ptr(camera_position));
}

int main(int argc, char * argv[]){
    std::cout << "height map image: " << argv[1] << std::endl;
    std::cout << "texture map image: " << argv[2] << std::endl;

    glfwSetErrorCallback(errorCallback);

    glewExperimental = GL_TRUE; 

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

    glewExperimental = GL_TRUE; 

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    //init shaders
    //Make the program current
    //glUseProgram(programId);
    initTexture(argv[2], &widthTexture, &heightTexture, false);
    
    //set camera_position using widthTexture
    //The camera will be positioned initially at (w/2, w/10, -w/4)
    camera_position = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
    
    //cameraTransformation()

    

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