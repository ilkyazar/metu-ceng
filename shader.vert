#version 330

layout(location = 0) in vec3 position;

// Uniform variables that must be set by OpenGL program;
uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D texture1; //texture map
uniform sampler2D texture2; //height map
uniform int widthTexture;
uniform int heightTexture;
uniform float heightFactor;

out vec2 textureCoord;

// Varying variables that will be resterized for each fragment
varying vec4 fragWorldPos;
varying vec3 fragWorldNor;

void main() {
    textureCoord = vec2(-position.x / float(widthTexture), -position.z / float(heightTexture));
    vec4 textureColor = texture(texture2, textureCoord);
    float height = heightFactor * textureColor.x;

    // gl_Vertex and gl_Normal are attribute variables fetched from buffers

    //fragWorldPos = modelingMatrix * gl_Vertex;
    //fragWorldNor = inverse(transpose(mat3x3(modelingMatrix))) * gl_Normal;

    vec3 totalNormal = vec3(0.0, 0.0, 0.0);
    int count = 0;
    if()

    
    
    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * gl_Vertex;


}