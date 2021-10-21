#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"


static GLFWwindow* window = NULL;

int widthTexture, heightTexture;
GLfloat heightFactor = 10.0f;

// For reading textures
GLuint textureId;
GLuint heightTextureId;

// For shaders
GLuint programId;
GLuint vertexShaderId;
GLuint fragmentShaderId;

// Vertices of triangles
glm::vec3* vertices;

// For camera
glm::vec3 camera_position;
GLfloat camera_speed = 0.0f;
glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 camera_gaze = glm::vec3(0.0, 0.0, 1.0);

// light source
glm::vec3 light_position;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
glm::mat4 normalMatrix;


int widthDisplay = 1000;
int heightDisplay = 1000;

bool isFullScreen = false;
GLFWmonitor* monitor;
const GLFWvidmode* videoMode;

static void errorCallback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

static void windowsizeCallback(GLFWwindow* window, int widthDisplay, int heightDisplay) {
    glViewport(0, 0, widthDisplay, heightDisplay);
}

// Keyboard events will be called here
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);   
    }
    else if( key == GLFW_KEY_P && action == GLFW_PRESS ){
        if(isFullScreen){
            heightDisplay = 1000;
            widthDisplay = 1000;
            isFullScreen = false;
            glfwSetWindowMonitor(window, nullptr, 0, 0, widthDisplay, heightDisplay, 0);
        }
        else{
            heightDisplay = videoMode->height;
            widthDisplay = videoMode->width;
            isFullScreen = true;
            glfwSetWindowMonitor(window, monitor,0,0, widthDisplay, heightDisplay, videoMode->refreshRate);
        }
    }

    else if (key == GLFW_KEY_R) {
        heightFactor += 0.5;
    }

    else if (key == GLFW_KEY_F) {
        heightFactor -= 0.5;
    }

    else if (key == GLFW_KEY_A) {
        // rotates the gaze around the up vector
        camera_gaze = glm::rotate(camera_gaze, 0.05f, camera_up);

    }

    else if (key == GLFW_KEY_D) {
        // rotates the gaze around the up vector
        camera_gaze = glm::rotate(camera_gaze, -0.05f, camera_up);
    }

    else if (key == GLFW_KEY_W) {
        // rotates gaze around the left vector
        glm::vec3 camera_left = glm::vec3(1.0, 0.0, 0.0);
        camera_gaze = glm::rotate(camera_gaze, 0.05f, camera_left);
        //camera_up = glm::rotate(camera_up, 0.05f, camera_left);
        
    }

    else if (key == GLFW_KEY_S) {
        // rotates gaze around the left vector
        glm::vec3 camera_left = glm::cross(camera_up, camera_gaze);
        camera_gaze = glm::rotate(camera_gaze, -0.05f, camera_left);
        camera_up = glm::rotate(camera_up, -0.05f,camera_left);
    }

    else if (key == GLFW_KEY_I) {
        // the plane will be placed to the initial position with initial configurations of the camera and speed of 0
        // The camera will be positioned initially at (w/2, w/10, -w/4)
        camera_position = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
        camera_up = glm::vec3(0.0, 1.0, 0.0);
        camera_gaze = glm::vec3(0.0, 0.0, 1.0);
        camera_speed = 0.0f;
    }

    //move the light source
    else if(key == GLFW_KEY_T) {
        light_position.y += 5;
    }

    else if(key == GLFW_KEY_G) {
        light_position.y -= 5;
    }

    else if(key == GLFW_KEY_RIGHT) {
        light_position.x -= 5;
    }

    else if(key == GLFW_KEY_LEFT) {
        light_position.x += 5;
    }

    else if(key == GLFW_KEY_UP) {
        light_position.z += 5;
    }

    else if(key == GLFW_KEY_DOWN) {
        light_position.z -= 5;
    }

    /*
    Q and E keys will move the texture and height map along the plane left and right respectively
    by subtracting and adding 1
    */
    else if (key == GLFW_KEY_Q) {
    }

    else if (key == GLFW_KEY_E) {
    }

    else if (key == GLFW_KEY_Y) {
        camera_speed += 0.01;
    }
    else if (key == GLFW_KEY_H) {
        camera_speed -= 0.01;
    }
    else if (key == GLFW_KEY_X) {
        camera_speed = 0.0f;
    }

}

// Sample triangle rendering, not used
void customizedRenderFunction() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // color state is red
    glVertex3f(0, 0.5, 0); // first vertex
    glColor3f(0.0, 1.0, 0.0); // color state is green
    glVertex3f(-0.5, -0.5, 0); // second vertex
    glColor3f(0.0, 0.0, 1.0); //color state is blue
    glVertex3f(0.5, -0.5, 0); // third vertex
    glEnd();
}

void createTriangles() {
    int vertex_count = 3 * 2 * widthTexture * heightTexture;
    vertices = new glm::vec3[vertex_count];
    int index = 0;
    glm::vec3 vertex;

    // Each pixel will be represented by two triangles
    for(int i = 0; i < widthTexture; i++){
        for(int j = 0; j < heightTexture; j++, index+=6){
            // 1st triangle
            vertices[index] = glm::vec3(i, 0, j);
            vertices[index+1] = glm::vec3(i+1, 0, j+1);
            vertices[index+2] = glm::vec3(i, 0, j+1);

            // 2nd triangle
            vertices[index+3] = glm::vec3(i, 0, j);
            vertices[index+4] = glm::vec3(i+1, 0, j);
            vertices[index+5] = glm::vec3(i+1, 0, j+1);
        }
    }
}

void setTextures(char * argv[]) {
    initTexture(argv[2], &widthTexture, &heightTexture, false);
    initTexture(argv[1], &widthTexture, &heightTexture, true);

    glUniform1i(glGetUniformLocation(programId, "texture1"), 0);
    glUniform1i(glGetUniformLocation(programId, "texture2"), 1);

    glUniform1i(glGetUniformLocation(programId, "widthTexture"), widthTexture);
    glUniform1i(glGetUniformLocation(programId , "heightTexture"), heightTexture);
}

void setUniformVariables() {

    // Query their location
    GLint modelingMatrixLoc = (GLint)glGetUniformLocation(programId, "modelingMatrix");
    GLint viewingMatrixLoc = (GLint)glGetUniformLocation(programId, "viewingMatrix");
    GLint projectionMatrixLoc = (GLint)glGetUniformLocation(programId, "projectionMatrix");
    GLint cameraPositionLoc = (GLint)glGetUniformLocation(programId, "cameraPos");
    GLint lightPositionLoc = (GLint)glGetUniformLocation(programId, "lightPos");
    GLint normalLoc = (GLint)glGetUniformLocation(programId, "normalMatrix");
    GLint heightFacLoc = (GLint)glGetUniformLocation(programId, "heightFactor");

    // Set the variables 
    glUniformMatrix4fv(modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(cameraPositionLoc, 1, glm::value_ptr(camera_position));
    glUniform3fv(lightPositionLoc, 1, glm::value_ptr(light_position));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniform1f(heightFacLoc, heightFactor);
}

void cameraTransformation(){
    // There will be perspective projection with an angle of 45 degrees. The aspect ratio will be 1,
    // near and far plane will be 0.1 and 1000 respectively.
    camera_position = camera_position + (camera_gaze * camera_speed);
    projection =  glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
    view = glm::lookAt(camera_position, camera_position + camera_gaze * 0.1f, camera_up);
    model = glm::mat4(1.0f);
    normalMatrix = glm::inverseTranspose(view);
}


void activateShaders() {
    // Create a shader program
    programId = glCreateProgram();

    // Create vertex and fragment shaders
    vertexShaderId   = initShader("shader.vert", true);
    fragmentShaderId = initShader("shader.frag", false);

    // Attach shaders to the shader programs
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);    
    // Activate the program
    glUseProgram(programId);    
}

void setWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(1000, 1000, "CENG477 - HW3", NULL, NULL);

    if (!window){
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowsizeCallback);
}


void clearBuffers() {
    glClearStencil(0);
    glClearDepth(1.0f);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void drawVertexArray() {
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 6 * widthTexture * heightTexture);
    glDisableClientState(GL_VERTEX_ARRAY);

    glfwSwapBuffers(window);
}

void setScreen(){
    monitor = glfwGetPrimaryMonitor();
    videoMode = glfwGetVideoMode(monitor);
    glfwGetWindowSize(window, &widthDisplay, &heightDisplay);
}

int main(int argc, char * argv[]){
    std::cout << "height map image: " << argv[1] << std::endl;
    std::cout << "texture map image: " << argv[2] << std::endl;

    glfwSetErrorCallback(errorCallback);

    glewExperimental = GL_TRUE; 

    if(!glfwInit()){
        exit(-1);
    }

    setWindow();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        glfwTerminate();
        exit(-1);
    }

    activateShaders();
    
    setTextures(argv);

    // The camera will be positioned initially at (w/2, w/10, -w/4)
    camera_position = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
    //There will be one light source in the scene at the initial position of (w/2, 100, h/2).
    light_position = glm::vec3(widthTexture/2, 100,heightTexture/2 );
    cameraTransformation();

    // Uniform variables must be set by the main program
    setUniformVariables();

    createTriangles();

    glEnable(GL_DEPTH_TEST);

    setScreen();
    
    while(!glfwWindowShouldClose(window)) {
        // All buffers should be cleared before rendering each frame
        clearBuffers();
        
        glViewport(0, 0, widthDisplay, heightDisplay);
        cameraTransformation();

        // Uniform variables must be set by the main program
        setUniformVariables();

        drawVertexArray();
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();
}