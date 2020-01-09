#include "helper.h"

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

    raw_image = (unsigned char*) malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    row_pointer[0] = (unsigned char *) malloc( cinfo.output_width*cinfo.num_components );
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glewExperimental = GL_TRUE; 
    
    if ( GLEW_OK != glewInit()) {
        exit(-1);
    }

    if(!isHeight){
        //READ TEXTURE MAP
        // First parameter is for the count of sampler objects
        glGenTextures(1, &textureId);
        
        // Activate the desired unit and bind the texture to the target of that unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // select modulate to mix texture with color for shading
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
        // when texture area is small, bilinear filter the closest mipmap
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // when texture area is large, bilinear filter the first mipmap
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // texture should tile
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Upload the img to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else{
        //READ HEIGHT MAP
        glGenTextures(1, &heightTextureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightTextureId);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Upload the img to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    
    *w = width;
    *h = height;

    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}

void initShaders() {

  programId = glCreateProgram();

  //vertexShaderId   = initVertexShader("shader.vert");
  //fragmentShaderId = initFragmentShader("shader.frag");

  //glAttachShader(programId, vertexShaderId);
  //glAttachShader(programId, fragmentShaderId);

  glLinkProgram(programId);

}