#version 330

// Uniform variables that must be set by OpenGL program;
uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;

// Varying variables that will be resterized for each fragment
varying vec4 fragWorldPos;
varying vec3 fragWorldNor;

void main() {

    // gl_Vertex and gl_Normal are attribute variables fetched from buffers

    fragWorldPos = modelingMatrix * gl_Vertex;
    fragWorldNor = inverse(transpose(mat3x3(modelingMatrix))) * gl_Normal;

    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * gl_Vertex;
}