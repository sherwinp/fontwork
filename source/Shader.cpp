// Std. Includes
#include <iostream>
#include <sstream>
#include <fstream>

#include <map>
#include <string>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


void Shader::Load(std::string filepathvs, std::string filepathfs, std::string filepathgs, std::string name)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    bool hasGeometryShaderCode;
    try
    {
        // Open files
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        std::ifstream vertexShaderFile(filepathvs);
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();

        std::ifstream fragmentShaderFile(filepathfs);
        fShaderStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fShaderStream.str();

        // If geometry shader path is present, also load a geometry shader
        std::ifstream geometryShaderFile(filepathgs);
        hasGeometryShaderCode = !geometryShaderFile.fail();
        if (hasGeometryShaderCode)
        {
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    // 2. Now create shader object from source code
    Compile(vertexCode, fragmentCode, hasGeometryShaderCode ? geometryCode : "");
    if( !success )
    {
        std::cerr << filepathvs << " shader ";
        throw std::bad_exception();
    }
}

Shader &Shader::Use()
{
    glUseProgram(this->ID);
    return *this;
}
void preprocess_shadersource(GLuint shader, GLenum type, std::string source){

    const GLchar* sources[] = {
      // Define GLSL version
  #ifdef GL_ES_VERSION_2_0
      "#version 300 es\n"
  #else
      "#version 300 es\n"
  #endif
      ,
      // GLES2 precision specifiers
  #ifdef GL_ES_VERSION_2_0
      // Define default float precision for fragment shaders:
      (type == GL_FRAGMENT_SHADER) ?
      "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
      "precision highp float;           \n"
      "#else                            \n"
      "precision mediump float;         \n"
      "#endif                           \n"
      : ""
      // Note: OpenGL ES automatically defines this:
      // #define GL_ES
  #else
      // Ignore GLES 2 precision specifiers:
      "precision mediump float;         \n"
  #endif
      ,
       source.c_str()};

    glShaderSource(shader, 3, sources, NULL);

}
void Shader::Compile(std::string vertexSource, std::string fragmentSource, std::string geometrySource)
{
    GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    preprocess_shadersource(sVertex, GL_VERTEX_SHADER, vertexSource);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    preprocess_shadersource(sFragment, GL_FRAGMENT_SHADER, fragmentSource);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // If geometry shader source code is given, also compile geometry shader
    if (!geometrySource.empty())
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        preprocess_shadersource(gShader, GL_GEOMETRY_SHADER, geometrySource);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (!geometrySource.empty())
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (!geometrySource.empty())
        glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
