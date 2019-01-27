#include "GLShader.h"

GLShader::GLShader()
{
    totalShaders=0;
    shaders = std::make_unique<GLuint[]>(2);
    shaders[(int)ShaderType::VERTEX_SHADER]=0;
    shaders[(int)ShaderType::FRAGMENT_SHADER]=0;
    attributeList.clear();
    uniformLocationList.clear();
}

GLShader::~GLShader()
{
    attributeList.clear();    
    uniformLocationList.clear();
}
                                                                       
void
GLShader::LoadFromString
(GLenum type, const std::string &source)
{    
    #ifndef __APPLE__  
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cerr << "glewInit() failed returning " << err << std::endl;
        exit(1);
    }
    #endif
    
    GLuint shader = glCreateShader(type);

    const char * ptmp = source.c_str();
    glShaderSource (shader, 1, &ptmp, NULL);
 
    GLint status;
    glCompileShader (shader);
    glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;        
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
        std::cerr << "Compile log: " << infoLog << std::endl;
        delete [] infoLog;
    }
    shaders[totalShaders++] = shader;
}

void
GLShader::CreateProgram()
{
    program = glCreateProgram();
} 

void
GLShader::LinkProgram()
{    
    GLint status;
    glLinkProgram (program);
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        
        glGetProgramiv (program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glGetProgramInfoLog (program, infoLogLength, NULL, infoLog);
        std::cerr << "Link log: " << infoLog << std::endl;
        delete [] infoLog;
    }

    glDeleteShader(shaders[(int) ShaderType::VERTEX_SHADER]);
    glDeleteShader(shaders[(int) ShaderType::FRAGMENT_SHADER]);
}

void
GLShader::AttachShaders()
{
    if (shaders[(int) ShaderType::VERTEX_SHADER] != 0)
    {
        glAttachShader(program, shaders[(int) ShaderType::VERTEX_SHADER]);
    }
    if (shaders[(int) ShaderType::FRAGMENT_SHADER] != 0)
    {
        glAttachShader(program, shaders[(int) ShaderType::FRAGMENT_SHADER]);
    }    
}

void
GLShader::Use()
{
    glUseProgram(program);
}

void
GLShader::UnUse()
{
    glUseProgram(0);
}

void
GLShader::DetachShaders()
{
    if (shaders[(int) ShaderType::VERTEX_SHADER] != 0)
    {
        glDetachShader(program, shaders[(int) ShaderType::VERTEX_SHADER]);
    }
    if (shaders[(int) ShaderType::FRAGMENT_SHADER] != 0)
    {
        glDetachShader(program, shaders[(int) ShaderType::FRAGMENT_SHADER]);
    }    
}

void
GLShader::DeleteShaderProgram()
{    
    glDeleteProgram(program);
} 

void
GLShader::AddAttribute
(const std::string &attribute)
{
    attributeList[attribute] = 
        glGetAttribLocation(program, attribute.c_str());    
}

GLuint
GLShader::operator 
[](const std::string &attribute)
{
    return attributeList[attribute];
}

void
GLShader::AddUniform
(const std::string &uniform)
{
    uniformLocationList[uniform] = 
        glGetUniformLocation(program, uniform.c_str());
}

GLuint
GLShader::ReturnShaderProgram()
{
    return program;
} 

GLuint
GLShader::operator
()(const std::string &uniform)
{
    return uniformLocationList[uniform];
}

GLuint
GLShader::ReturnShaderAttribute
(std::string attribute)
{
    return attributeList[attribute];    
}

void
GLShader::LoadFromFile
(GLenum whichShader, const std::string &filename)
{
    int Mslv, mslv; // major and minor versions of the shading language
    sscanf((char*)glGetString
        (GL_SHADING_LANGUAGE_VERSION), "%d.%d", &Mslv, &mslv);
    std::string strMajor = std::to_string(Mslv);
    std::string strMinor = std::to_string(mslv);
    
    std::ifstream ifs {filename, std::ifstream::in};
    if(ifs)
    {         
        std::string line, buffer;
        buffer = "#version " + strMajor + strMinor + "\r\n";
        buffer.append(std::istreambuf_iterator<char>(ifs), 
                      std::istreambuf_iterator<char>());
        LoadFromString(whichShader, buffer);  
    }
    else
    {
        std::cerr << "Error loading shader: " << filename << std::endl;
    }
}
