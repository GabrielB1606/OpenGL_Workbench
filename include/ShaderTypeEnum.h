#define N_SHADER_TYPES 6

enum ShaderTypeEnum{
    ARR_VERTEX_SHADER = 0,
    ARR_TESSELLATION_CONTROL_SHADER,
    ARR_TESSELLATION_EVALUATION_SHADER,
    ARR_GEOMETRY_SHADER,
    ARR_FRAGMENT_SHADER,
    ARR_COMPUTE_SHADER
};

const std::string ShaderNames[N_SHADER_TYPES] = {
    "Vertex Shader",
    "Tessellation Control Shader",
    "Tessellation Evaluation Shader",
    "Geometry Shader",
    "Fragment Shader",
    "Compute Shader"
};