#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <memory>

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#else
#  ifdef WIN32
#    define GLEW_STATIC 1
#  endif
#  include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

enum class ShaderType {VERTEX_SHADER, FRAGMENT_SHADER};

class GLShader
{
    public:
        GLShader();
        ~GLShader();    

        // Don't want to copy or move shaders
        GLShader(const GLShader&) = delete;
        GLShader& operator=(const GLShader&) = delete;

        void LoadFromString(GLenum whichShader, const std::string &source);
        void LoadFromFile(GLenum whichShader, const std::string &filename);
        
        void CreateProgram();
        void AttachShaders();
        void LinkProgram();
        void Use();
        void UnUse();
        void DetachShaders();
        void DeleteShaderProgram();
        
        void AddAttribute(const std::string &attribute);
        void AddUniform(const std::string &uniform);

        GLuint operator[](const std::string &attribute);
        GLuint operator()(const std::string &uniform);
        
        GLuint ReturnShaderAttribute(std::string attribute);
        GLuint ReturnShaderProgram();

    private:
        GLuint program;
        int totalShaders;
        std::unique_ptr<GLuint[]> shaders; // 0->vertexshader, 1->fragmentshader
        std::map<std::string, GLuint> attributeList;
        std::map<std::string, GLuint> uniformLocationList;
};    
