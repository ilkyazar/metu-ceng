#version 410

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

in vec2 textureCoord;
in vec3 vertexNormal;
in vec3 toCameraVec;
in vec3 toLightVec;

out vec4 vertexColor;

void main(){

    vec4 textureColor = texture(texture1, textureCoord);

    vec4 ambientCoeff = vec4(0.25 ,0.25 ,0.25 ,1.0);
    vec4 ambientLightColor = vec4(0.3 ,0.3 ,0.3 ,1.0);
    vec4 specularCoeff = vec4(1.0 ,1.0 ,1.0 ,1.0);
    vec4 specularLightColor = vec4(1.0 ,1.0 ,1.0 ,1.0);
    float specularExp = 100.0;
    vec4 diffuseCoeff = vec4(1.0 ,1.0 ,1.0 ,1.0);
    vec4 diffuseLightColor = vec4(1.0 ,1.0 ,1.0 ,1.0);

    float NormalDotLight = clamp(dot(vertexNormal, toLightVec), 0.0, 1.0); //for diffuse component    
    vec3 H = normalize(toLightVec + toCameraVec);
    float NormalDotH = clamp(dot(vertexNormal, H), 0.0, 1.0); //for specular component

    vec4 diffuseColor = diffuseLightColor * diffuseCoeff * NormalDotLight;
    vec4 specularColor = specularLightColor * specularCoeff * pow(NormalDotH, specularExp);
    vec4 ambientColor = ambientLightColor * ambientCoeff;

    /*
    The computed surface color will be combined with the texture color do determine the final
    color of the surface. “clamp“ method can be used to combine these two colors.
    */
    vec4 totalColor = ambientColor + specularColor + diffuseColor;
    vertexColor = vec4(clamp(textureColor.xyz * vec3(totalColor), 0.0, 1.0), 1.0);
    //vertexColor = vec4(1.0,1.0,1.0,1.0);
    

}