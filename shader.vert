#version 410

layout(location = 0) in vec3 position;

// Uniform variables that are set by main OpenGL program;
uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform vec4 cameraPos;
uniform mat4 normalMatrix;
uniform vec3 lightPos;

uniform sampler2D texture1; //texture map
uniform sampler2D texture2; //height map
uniform int widthTexture;
uniform int heightTexture;
uniform float heightFactor;

// Outputs of vertex shader are used by fragment shader
out vec2 textureCoord;
out vec3 vertexNormal;
out vec3 toCameraVec;
out vec3 toLightVec;

void setHeight(inout vec3 pos){
    //Compute height from texture2 for given x,z texture coordinates
    vec2 texCoord = vec2(abs(pos.x - widthTexture)/ float(widthTexture), abs(pos.z - heightTexture) / float(heightTexture));
    vec4 texColor = texture(texture2, texCoord);
    float height = heightFactor * texColor.x;
    pos.y = height;
}

void main() {

    textureCoord = vec2(1 - (position.x / (widthTexture + 1)), 1 - (position.z / (heightTexture + 1)));
    vec4 texColor = texture(texture2, textureCoord);
    
    vec3 pos = position;
    pos.y = heightFactor * texColor.x;
    vec3 totalNormal = vec3(0.0, 0.0, 0.0);

    //positions of the vertices of adjecent triangles
    vec3 posLeft, posRight, posBottom, posTop, posTopLeft, posBottomRight;

    //compute vertex normal
    if(position.x < widthTexture && position.x > 0 && position.z < heightTexture && position.z > 0){    
        posRight = position + vec3(1,0,0);
        posTop = position + vec3(0,0,1);
        posTopLeft = position + vec3(-1,0,1);
        posLeft = position - vec3(1,0,0);
        posBottom = position - vec3(0,0,1);
        posBottomRight = position + vec3(1,0,-1);

        //set height (y component) of each adj vertex
        setHeight(posRight);
        setHeight(posTop);
        setHeight(posTopLeft);
        setHeight(posLeft);
        setHeight(posBottom);
        setHeight(posBottomRight);

        //A vertex has 6 adj triangles
        vec3 normal1 = cross((posLeft - pos),(posTopLeft - pos));
        vec3 normal2 = cross((posTopLeft - pos),(posTop - pos));
        vec3 normal3 = cross((posTop - pos),(posRight - pos));
        vec3 normal4 = cross((posRight - pos),(posBottomRight - pos));
        vec3 normal5 = cross((posBottomRight - pos),(posBottom - pos));
        vec3 normal6 = cross((posBottom - pos),(posLeft - pos));

        totalNormal = normal1 + normal2 + normal3 + normal4 + normal5 + normal6;
        vertexNormal = normalize(totalNormal);
        vertexNormal = normalize(vec3(normalMatrix * vec4(vertexNormal, 0.0)));
    }
    //Vector from current position to camera
    toCameraVec = vec3(cameraPos.x - position.x, cameraPos.y - pos.y, cameraPos.z - position.z );
    toCameraVec = normalize(toCameraVec);

    //There will be one light source in the scene at the initial position of (w/2, 100, h/2)
    toLightVec = vec3(lightPos.x - position.x, lightPos.y - pos.y, lightPos.z - position.z);
    toLightVec = normalize(toLightVec);
    
    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * vec4(pos, 1.0);
}