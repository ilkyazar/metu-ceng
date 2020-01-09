#ifndef __HELPER__H__
#define __HELPER__H__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <jpeglib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//using namespace std;

extern GLuint textureId;
extern GLuint samplerId;

extern GLuint programId;
extern GLuint vertexShaderId;
extern GLuint fragmentShaderId;

void initTexture(char *filename, int *w, int *h);
void initShaders();

#endif