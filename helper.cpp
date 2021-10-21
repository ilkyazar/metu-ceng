#include "helper.h"

// This function will read the texture and initialize it according to the boolean value
// Ex: If it is the first argument i.e the height map, it will read the height map

void initTexture(char *filename, int *w, int *h, bool isHeight) {
    int width, height;

    unsigned char *raw_image = NULL;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_decompress( &cinfo );
    jpeg_stdio_src( &cinfo, infile );
    jpeg_read_header( &cinfo, TRUE );
    jpeg_start_decompress( &cinfo );

    raw_image = (unsigned char*) malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);
    row_pointer[0] = (unsigned char*) malloc(cinfo.output_width*cinfo.num_components);

    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);

        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++) {
            raw_image[location++] = row_pointer[0][i];
        }
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glewExperimental = GL_TRUE; 
    
    if (GLEW_OK != glewInit()) {
        exit(-1);
    }

    if(!isHeight){
        // READ TEXTURE MAP

        // Generate texture names
        // First parameter is for the count of sampler objects
        glGenTextures(1, &textureId);
        
        // Activate the desired unit and bind the texture to the target of that unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Select modulate to mix texture with color for shading
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

        // When texture area is small, bilinear filter the closest mipmap
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // When texture area is large, bilinear filter the first mipmap
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Texture should tile
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Set pixel storage modes
        // GL_UNPACK_ALIGNMENT specifies the alignment requirements for the start of each pixel row in memory
        // 1 is the value that GL_UNPACK_ALIGNMENT is set to
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Upload the image to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else {
        // READ HEIGHT MAP
        glGenTextures(1, &heightTextureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightTextureId);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Upload the image to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    
    *w = width;
    *h = height;

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}

GLuint initShader(const std::string& filename, bool isVertexShader){
    std::string shaderSource;

    if (!readDataFromFile(filename, shaderSource)){
        std::cout << "Cannot find file name: " + filename << std::endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint sh;
    if (isVertexShader) {
        sh = glCreateShader(GL_VERTEX_SHADER);
    }
    else {
        sh = glCreateShader(GL_FRAGMENT_SHADER);
    }

    glShaderSource(sh, 1, &shader, &length);
    glCompileShader(sh);

    char output[1024] = {0};
    glGetShaderInfoLog(sh, 1024, &length, output);

    if (isVertexShader) {
        printf("Vertex shader compile log: %s\n", output);
    }
    else {
        printf("Fragment shader compile log: %s\n", output);
    }

    return sh;
}

bool readDataFromFile(const std::string& fileName, std::string &data) {
    std::fstream myfile;

    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open()) {
        std::string curLine;

        while (getline(myfile, curLine)){
            data += curLine;
            if (!myfile.eof())
                data += "\n";
        }

        myfile.close();
    }
    else
        return false;

    return true;
}

