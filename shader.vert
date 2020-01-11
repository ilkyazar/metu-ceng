#version 410

layout(location = 0) in vec3 position;

// Uniform variables that must be set by OpenGL program;
uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform vec4 cameraPos;
uniform mat4 M_norm;

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


void computeHeight(inout float height, inout vec3 pos) {
    //Change sign in case it doesnt work
    vec2 texCoord = vec2(abs(pos.x - widthTexture)/ float(widthTexture), abs(pos.z - heightTexture) / float(heightTexture));
    vec4 texColor = texture(texture2, texCoord);
    height = heightFactor * texColor.x;
    pos.y = height;
}


void compute(inout vec3 neighbor, inout vec2 textureCoordinate_neighbor){
    textureCoordinate_neighbor.x = abs(neighbor.x - widthTexture) / widthTexture;
    textureCoordinate_neighbor.y = abs(neighbor.z - heightTexture) / heightTexture;

    vec4 texColor = texture(texture2, textureCoordinate_neighbor);
    neighbor.y = heightFactor * texColor.x;
}

void main() {
    vec3 pos = position;
    mat4 MVP = projectionMatrix * viewingMatrix * modelingMatrix;
    mat4 MV = modelingMatrix;

    float textureCoordinate_x = 1 - (float(position.x) / (widthTexture + 1));
    float textureCoordinate_y = 1 - (float(position.z) / (heightTexture + 1));
    textureCoord = vec2(textureCoordinate_x, textureCoordinate_y);

    vec4 texColor = texture(texture2, textureCoord);
    
    pos.y = heightFactor * texColor.x;

    vec2 textureCoordinate_neighbor;
    vec3 norm;

    vec3 neighbor0, neighbor1, neighbor2, neighbor3, neighbor4, neighbor5;
    vec3 vertex0, vertex1, vertex2, vertex3, vertex4, vertex5;
    /*
    if(position.z == heightTexture-1 && position.x == 0){

        neighbor0 = vec3(position.x + 1, 0, position.z);
        neighbor1 = vec3(position.x + 1, 0, position.z - 1);
        neighbor2 = vec3(position.x, 0,position.z - 1);

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);
        compute(neighbor2, textureCoordinate_neighbor);

        vertex0 = neighbor0 - pos;
        vertex1 = neighbor1 - pos;
        vertex2 = neighbor2 - pos;

        norm = normalize(cross(vertex1, vertex0) + cross(vertex2, vertex1));

    }
    else if(position.z == heightTexture - 1 && position.x == widthTexture - 1){

        neighbor0 = vec3(position.x - 1, 0, position.z);
        neighbor1 = vec3(position.x, 0, position.z - 1);

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);

        vertex0 = pos -  neighbor0;
        vertex1 =  neighbor1 - neighbor0;

        norm = normalize(cross(vertex0, vertex1));

    }
    else if(position.z == 0 && position.x == widthTexture - 1){

        neighbor0 = vec3(position.x - 1, position.y, position.z);
        neighbor1 = vec3(position.x - 1, position.y, position.z + 1);
        neighbor2 = vec3(position.x, 0, position.z + 1);

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);
        compute(neighbor2, textureCoordinate_neighbor);

        vertex0 = neighbor0 - pos;
        vertex1 = neighbor1 - pos;
        vertex2 = neighbor2 - pos;

        norm = normalize(cross(vertex0, vertex1) + cross(vertex1, vertex2));

    }
    else if(position.z == 0 && position.x == 0){

        neighbor0 = vec3(position.x + 1, position.y, position.z);
        neighbor1 = vec3(position.x, position.y, position.z + 1);

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);

        vertex0 = pos - neighbor0;
        vertex1 = neighbor1 - neighbor0;

        norm = normalize(cross(vertex0, vertex1));
    }
    else if((0 < position.x && (position.z == heightTexture - 1 || position.z == 0)) ||
    (0 < position.z && (position.x == widthTexture - 1 || position.x == 0))){

        if(position.z == 0){

            neighbor0 = vec3(position.x + 1, 0, position.z);
            neighbor1 = vec3(position.x, 0, position.z + 1);
            neighbor2 = vec3(position.x - 1, 0, position.z + 1);
            neighbor3 = vec3(position.x - 1, 0, position.z);

        }
        else if(position.z == heightTexture - 1){

            neighbor0 = vec3(position.x - 1, 0, position.z);
            neighbor1 = vec3(position.x, 0, position.z - 1);
            neighbor2 = vec3(position.x + 1, 0, position.z - 1);
            neighbor3 = vec3(position.x + 1, 0, position.z);

        }
        else if(position.x == 0){

            neighbor0 = vec3(position.x, 0, position.z - 1);
            neighbor1 = vec3(position.x + 1, 0, position.z - 1);
            neighbor2 = vec3(position.x + 1, 0, position.z);
            neighbor3 = vec3(position.x, 0, position.z + 1);

        }
        else if(position.x == widthTexture - 1){

            neighbor0 = vec3(position.x, 0, position.z + 1);
            neighbor1 = vec3(position.x - 1, 0, position.z + 1);
            neighbor2 = vec3(position.x - 1, 0, position.z);
            neighbor3 = vec3(position.x, 0, position.z - 1);

        }

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);
        compute(neighbor2, textureCoordinate_neighbor);
        compute(neighbor3, textureCoordinate_neighbor);

        vertex0 = neighbor0 - position;
        vertex1 = neighbor1 - position;
        vertex2 = neighbor2 - position;
        vertex3 = neighbor3 - position;

        norm = normalize(cross(vertex1, vertex0) + cross(vertex2, vertex1) + cross(vertex3, vertex2));

    }
    else{
        */
        neighbor0 = vec3(position.x - 1, 0, position.z);
        neighbor1 = vec3(position.x - 1, 0, position.z + 1);
        neighbor2 = vec3(position.x, 0, position.z + 1);
        neighbor3 = vec3(position.x + 1, 0, position.z);
        neighbor4 = vec3(position.x + 1, 0, position.z - 1);
        neighbor5 = vec3(position.x, 0, position.z - 1);

        compute(neighbor0, textureCoordinate_neighbor);
        compute(neighbor1, textureCoordinate_neighbor);
        compute(neighbor2, textureCoordinate_neighbor);
        compute(neighbor3, textureCoordinate_neighbor);
        compute(neighbor4, textureCoordinate_neighbor);
        compute(neighbor5, textureCoordinate_neighbor);

        vertex0 = neighbor0 - pos;
        vertex1 = neighbor1 - pos;
        vertex2 = neighbor2 - pos;
        vertex3 = neighbor3 - pos;
        vertex4 = neighbor4 - pos;
        vertex5 = neighbor5 - pos;

        norm = normalize(cross(vertex0, vertex1) + cross(vertex1, vertex2) + cross(vertex2, vertex3) + cross(vertex3, vertex4)
                            + cross(vertex4, vertex5) + cross(vertex5, vertex0));

    //}
    
    vertexNormal = normalize(vec3(M_norm * vec4(norm, 0)));
    toCameraVec = normalize(vec3(MV * (vec4(vec3(cameraPos) - pos, 0))));
    toLightVec = normalize(vec3(widthTexture / 2 - position.x, 100 - pos.y, heightTexture / 2 - position.z));

    /*
    
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

        computeHeight(heightLeft, posLeft);
        computeHeight(heightRight, posRight);
        computeHeight(heightBottom, posBottom);
        computeHeight(heightTop, posTop);
        computeHeight(heightTRight, posTRight);
        computeHeight(heightBLeft, posBLeft);

        //A vertex has 6 adj triangles
        vec3 normal1 = cross((posTop - pos),(posLeft - pos));
        vec3 normal2 = cross((posTRight - pos),(posTop - pos));
        vec3 normal3 = cross((posRight - pos),(posTRight - pos));
        vec3 normal4 = cross((posBottom - pos),(posRight - pos));
        vec3 normal5 = cross((posBLeft - pos),(posBottom - pos));
        vec3 normal6 = cross((posLeft - pos),(posBLeft - pos));

        totalNormal = normal1 + normal2 + normal3 + normal4 + normal5 + normal6;
        vertexNormal = normalize(vertexNormal);
        vertexNormal = normalize(vec3(M_norm * vec4(vertexNormal, 0)));
    }
    */
    gl_Position = MVP * vec4(pos, 1.0);
    //gl_Position = vec4(100.0, 100.0, 0.0, 1.0);
}