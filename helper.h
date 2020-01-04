#ifndef __HELPER__H__
#define __HELPER__H__

#include <iostream>
#include <string>
#include <fstream>
#include <jpeglib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//using namespace std;

extern GLuint idJpegTexture;

void initTexture(char *filename, int *w, int *h);

#endif