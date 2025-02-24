#pragma once

// 각종 include
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// 각종 lib
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)

// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 싱글톤 매크로
#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLE(type)	type::GetInstance()

#define CAMERAPOS()			GET_SINGLE(Camera)->getPosition()

// 공용 변수들
const unsigned int WIN_W = 800;
const unsigned int WIN_H = 600;
const unsigned int WIN_X = 100;
const unsigned int WIN_Y = 100;

const float PI = 3.141592f;

extern double cur_x;
extern double cur_y;

extern unique_ptr<class Engine> GEngine;

// 공용 함수들
string LoadFile(const string& filename);
GLuint LoadTexture(const char* path);
void SetUpShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);