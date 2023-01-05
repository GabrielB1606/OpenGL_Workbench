#define N_SHADER_TYPES 6

enum ShaderTypeEnum{
    VERTEX_SHADER = 0,
    TESSELLATION_CONTROL_SHADER,
    TESSELLATION_EVALUATION_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER,
    COMPUTE_SHADER
};

const std::string ShaderNames[N_SHADER_TYPES] = {
    "Vertex Shader",
    "Tessellation Control Shader",
    "Tessellation Evaluation Shader",
    "Geometry Shader",
    "Fragment Shader",
    "Compute Shader"
};