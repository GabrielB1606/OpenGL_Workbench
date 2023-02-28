// #ifndef PROGRAM_H
// #define PROGRAM_H

// class Program{
//     private:
//         GLuint id;  // Program ID
        
//         // GLSL version
//         const int
//             versionMaj,
//             versionMin;

//         const char* glsl_version;

//         GLuint shaders[N_SHADER_TYPES] = {0, 0, 0, 0, 0, 0};

//         std::string loadShaderSource(char* filename);
//         GLuint loadShader(GLenum type, char* filename);
//         bool linkProgram();

//     public:

//         // Constructor/Destructor
//         Program(const char* glsl_version, const int versionMaj, const int versionMin, const char* vertexFile, const char* fragmentFile, char* geometryFile);
//         Program(const char* glsl_version, const int versionMaj, const int versionMin);
//         ~Program();

//         // Bind/Unbind Program
//         void use();
//         void stopUsing();

//         // Set Uniforms
        
//             // Vectors
//         void setVec2f(glm::vec2 value, const GLchar* name);
//         void setVec3f(glm::vec3 value, const GLchar* name);
//         void setVec4f(glm::vec4 value, const GLchar* name);
        
//             // Matrices 
//         void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose);
//         void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose);
        
//             // Scalars
//         void set1f(GLfloat value, const GLchar* name);
//         void set1i(GLint value, const GLchar* name);
// };

// #endif