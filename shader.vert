#version 430

layout(location = 0) in vec3 position;

// Uniform variables that must be set by OpenGL program;
uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform vec4 cameraPos;

uniform sampler2D texture1; //texture map
uniform sampler2D texture2; //height map
uniform int widthTexture;
uniform int heightTexture;
uniform float heightFactor;

out vec2 textureCoord;
out vec3 vertexNormal;
out vec3 toCameraVec;
out vec3 toLightVec;

// Varying variables that will be resterized for each fragment
//varying vec4 fragWorldPos;
//varying vec3 fragWorldNor;

float computeHeight(vec3 pos) {
    //Change sign in case it doesnt work
    vec3 texCoord = vec2(pos.x / float(widthTexture), pos.z / float(heightTexture));
    vec4 texColor = texture(texture2, texCoord);
    float height = heightFactor * textureColor.x;
    return height;
}

void main() {
    float currentVertexHeight = computeHeight(position);
    vec3 textureCoord = vec2(position.x / float(widthTexture), position.z / float(heightTexture));

    //Vector from current position to camera
    toCameraVec = vec3(cameraPos.x - position.x, cameraPos.y - currentVertexHeight, cameraPos.z - position.z );
    toCameraVec = normalize(toCameraVec);

    //There will be one light source in the scene at the initial position of (w/2, 100, h/2)
    toLightVec = vec3(widthTexture / 2 - position.x, 100 - currentVertexHeight, heightTexture / 2 - position.z);
    toLightVec = normalize(toLightVec);

    vec3 totalNormal = vec3(0.0, 0.0, 0.0);

    //height and positions of the vertices of adj triangles
    float heightLeft, heightRight, heightBottom, heightTop, heightTRight, heightBLeft;
    vec3 posLeft, posRight, posBottom, posTop, posTRight, posBLeft;
    
    //compute vertex normal
    if(position.x < widthTexture && position.x > 0 && position.z < heightTexture && position.z > 0){
        posLeft = position - vec3(1,0,0);
        posRight = position + vec3(1,0,0);
        posBottom = position - vec3(0,0,1);
        posTop = position + vec3(0,0,1);
        posTRight = position + vec3(1,0,1);
        posBLeft = position - vec3(1,0,1);

        heightLeft = computeHeight(posLeft);
        heightRight = computeHeight(posRight);
        heightBottom = computeHeight(posBottom);
        heightTop = computeHeight(posTop);
        heightTRight = computeHeight(posTRight);
        heightBLeft = computeHeight(posBLeft);

        //A vertex has 6 adj triangles
        vec3 normal1 = cross((posTop - position),(posLeft - position));
        vec3 normal2 = cross((posTRight - position),(posTop - position));
        vec3 normal3 = cross((posRight - position),(posTRight - position));
        vec3 normal4 = cross((posBottom - position),(posRight - position));
        vec3 normal5 = cross((posBLeft - position),(posBottom - position));
        vec3 normal6 = cross((posLeft - position),(posBLeft - position));

        totalNormal = normal1 + normal2 + normal3 + normal4 + normal5 + normal6;
        vertexNormal = totalNormal / vec3(6.0, 6.0, 6.0);
        vertexNormal = normalize(vertexNormal); 
    }


    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * vec4(position.x, currentVertexHeight, position.z);

}