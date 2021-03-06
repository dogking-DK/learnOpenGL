#ifndef HEADER_COLLECTION
# define HEADER_COLLECTION
#include <glad/glad.h>									// 封装OpenGL的库
#include <GLFW/glfw3.h>									// 图形界面库
#include <iostream>
#include "window.h"
#include "Shader.h"
#include "camera.h"
#include "texture.h"
#include "Model.h"
#include "Mesh.h"
#include "cube.h"
#include "CubeMap.h"
#include "utility.h"
#include "plane.h"
#include "light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"									// 读取图片的库
// 线性代数相关的数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#endif