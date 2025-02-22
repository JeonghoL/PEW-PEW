#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

#include <irrKlang.h>
using namespace irrklang;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)

const unsigned int WIN_W = 2560;
const unsigned int WIN_H = 1440;
const unsigned int WIN_X = 0;
const unsigned int WIN_Y = 0;

#define MAX_NUM_BONES_PER_VERTEX 4