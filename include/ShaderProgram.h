#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "headers.h"

class ShaderProgram{
    private:
        GLuint id;
        
        const int versionMaj;
        const int versionMin;

        const char* glsl_version;

        std::string loadShaderSource(const char* filename);
        GLuint loadShader(GLenum type, const char* filename);
        bool linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);

    public:
        ShaderProgram(const char* glsl_version, const int versionMaj, const int versionMin, const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
        ~ShaderProgram();
        void use();
        void stopUsing();

        // uniform setter
        void setVec2f(glm::vec2 value, const GLchar* name);
        void setVec3f(glm::vec3 value, const GLchar* name);
        void setVec4f(glm::vec4 value, const GLchar* name);
        void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose);
        void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose);
        void set1f(GLfloat value, const GLchar* name);
        void set1i(GLint value, const GLchar* name);

        // getters
        GLuint getID();
};

#endif