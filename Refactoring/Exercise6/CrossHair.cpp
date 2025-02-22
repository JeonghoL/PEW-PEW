#include "pch.h"
#include "CrossHair.h"
#include "stb_image.h"

const char* CrossHairvs = "CrosshairVert.glsl";
const char* CrossHairfs = "CrosshairFrag.glsl";

void setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);

CrossHair::CrossHair(int Viewtype, float size, GLFWwindow* win)
    :m_Viewtype(Viewtype), window(win), customCursor(nullptr)
{
    if (Viewtype == 1)
    {
        float centerX = WIN_W / 2.0f;
        float centerY = WIN_H / 2.0f;

        vertices = {
            centerX - size, centerY,
            centerX + size, centerY,

            centerX, centerY - size,
            centerX, centerY + size
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        setupCrosshairShaders(CrossHairvs, CrossHairfs);
    }
    else if (Viewtype == 3)
    {
        GLFWimage cursorImage;
        int channels;
        const char* cursorPath = "texture/aim.png"; 

        cursorImage.pixels = stbi_load(cursorPath,
            &cursorImage.width, &cursorImage.height,
            &channels, 4);

        if (cursorImage.pixels) {
            customCursor = glfwCreateCursor(&cursorImage,
                cursorImage.width / 2,  
                cursorImage.height / 2); 

            stbi_image_free(cursorImage.pixels);
        }
    }
}

CrossHair::~CrossHair()
{
    if (m_Viewtype == 1)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
    else if (m_Viewtype == 3 && customCursor)
    {
        glfwDestroyCursor(customCursor);
    }
}

void CrossHair::setupCrosshairShaders(const char* vertexName, const char* fragmentName)
{
    if (m_Viewtype == 1)
        setupShader(vertexName, fragmentName, shaderProgram);
}

void CrossHair::draw()
{
    if (m_Viewtype == 1)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glUseProgram(shaderProgram);

        glUniform2f(glGetUniformLocation(shaderProgram, "screenSize"), WIN_W, WIN_H);
        glUniform3f(glGetUniformLocation(shaderProgram, "crosshairColor"), 0.0f, 1.0f, 0.0f); // ���

        glBindVertexArray(VAO);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINES, 0, 4);
    }
    else if (m_Viewtype == 3)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (customCursor) {
            glfwSetCursor(window, customCursor);
        }
    }
}