#include <headers.h>
#include "Program.h"

std::string Program::loadShaderSource(char* filename){
    
    std::string temp = "";
    std::string src = "";
    std::ifstream in_file;

    in_file.open(filename);

    if(in_file.is_open()){

        while( std::getline(in_file, temp) ){
            src += temp + "\n";
        }

    }else{
        std::cout << "ERROR -> Can't open Shader Source\n\tShader src path: " << filename << "\n";
        return "";
    }

    in_file.close();

    std::string version = glsl_version;
    src.replace( src.find("#version"), 12, "#version "+version );

    return src;
}

GLuint Program::loadShader(GLenum type, char* filename){

    // status of the shader
    char infoLog[512];
    GLint success;

    // create the shader
    GLuint shader = glCreateShader( type );

    // load source of the shader
    std::string str_src =  this->loadShaderSource(filename);
    const GLchar* src = str_src.c_str();

    // compile shader source
    glShaderSource( shader, 1, &src, NULL );
    glCompileShader(shader);

    // get status of the compilation
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    // assign shader to program shaders array
    switch (shader){
        case GL_VERTEX_ARRAY:
            this->shaders[ ARR_VERTEX_SHADER ] = shader;
            break;
        
        case GL_TESS_CONTROL_SHADER:
            this->shaders[ ARR_TESSELLATION_CONTROL_SHADER ] = shader;
            break;
        
        case GL_TESS_EVALUATION_SHADER:
            this->shaders[ ARR_TESSELLATION_EVALUATION_SHADER ] = shader;
            break;
        
        case GL_GEOMETRY_SHADER:
            this->shaders[ ARR_GEOMETRY_SHADER ] = shader;
            break;

        case GL_FRAGMENT_SHADER:
            this->shaders[ ARR_FRAGMENT_SHADER ] = shader;
            break;
        
        case GL_COMPUTE_SHADER:
            this->shaders[ ARR_COMPUTE_SHADER ] = shader;
            break;

        default:
            break;
    }

    if( !success ){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR -> Can't compile shader:\n\tShader src path: " << filename <<"\n\t" << infoLog << "\n";
        return 0;
    }

    return shader;
}

bool Program::linkProgram(){

    // status of the program
    char infoLog[512];
    GLint success;

    // create program
    this->id = glCreateProgram();
    
    // attach all available shaders to the program
    for (size_t i = 0; i < N_SHADER_TYPES; i++){
        if( shaders[i] )
            glAttachShader( this->id, shaders[i] );
    }

    // link program
    glLinkProgram(this->id);

    // get status of the linking
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);

    if( !success ){
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR -> Program couldn't link:\n\t" << infoLog << "\n";
        return false;
    }

    // unbind program
    glUseProgram(0);

    // delete all compiled shaders (we will not need them anymore)
    for (size_t i = 0; i < N_SHADER_TYPES; i++){
        if( shaders[i] )
            glDeleteShader( this->shaders[i] );
    }

    return true;
}

// constructor/destructor

Program::Program(const char* glsl_version, const int versionMaj, const int versionMin,char* vertexFile, char* fragmentFile, char* geometryFile = nullptr) : versionMaj(versionMaj), versionMin(versionMin){

    // specify glsl version of the shaders
    this->glsl_version = glsl_version;

    //  load shaders
    this->shaders[ ARR_VERTEX_SHADER ] = loadShader(GL_VERTEX_SHADER, vertexFile);
    
    if( geometryFile != nullptr)
        this->shaders[ ARR_GEOMETRY_SHADER ] = loadShader(GL_GEOMETRY_SHADER, geometryFile);

    loadShader(GL_FRAGMENT_SHADER, fragmentFile);

    // link program
    this->linkProgram();

}

Program::Program(const char* glsl_version, const int versionMaj, const int versionMin) : versionMaj(versionMaj), versionMin(versionMin){
    this->glsl_version = glsl_version;
}

Program::~Program(){
    glDeleteProgram(this->id);
}

// bind/unbind

void Program::use(){
    glUseProgram(this->id);
}

void Program::stopUsing(){
    glUseProgram(0);
}

// uniform setters

void Program::setVec3f(glm::vec3 value, const GLchar* name){
    this->use();
    glUniform3fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );
    this->stopUsing();
}

void Program::setVec2f(glm::vec2 value, const GLchar* name){
    this->use();    
    glUniform2fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );
    this->stopUsing();
}

void Program::setVec4f(glm::vec4 value, const GLchar* name){
    this->use();
    glUniform4fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );
    this->stopUsing();
}

void Program::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE){
    this->use();
    glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    this->stopUsing();
}

void Program::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE){
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    this->stopUsing();
}

void Program::set1f(GLfloat value, const GLchar* name){
    this->use();
    glUniform1f( glGetUniformLocation(this->id, name), value );
    this->stopUsing();
}
void Program::set1i(GLint value, const GLchar *name)
{
    this->use();
    glUniform1i( glGetUniformLocation(this->id, name), value );
    this->stopUsing();
}