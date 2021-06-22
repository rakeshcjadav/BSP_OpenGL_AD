#include<iostream>
#include<glew.h>
#include<glfw3.h>

// GLEW OR GLAD

unsigned int CreateMesh();
unsigned int CreateShaders();

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    unsigned int mesh = CreateMesh();
    unsigned int program = CreateShaders();

    while (!glfwWindowShouldClose(window))
    {
        /* Loop until the user closes the window */
        /* Render here */
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindVertexArray(mesh);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
/*
// Vertex Shader

#version 330 core

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}

// Rasterizer will covert the shape into fragments ( RBGA, Depth )

// Fragment Shader
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
*/

unsigned int CreateMesh()
{
    float vertices[] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,     // 0
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     // 1
         0.5f,  0.5f, 1.0f, 0.0f, 0.0f      // 2
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    glBindVertexArray(0);

    return VAO;
}

unsigned int CreateShaders()
{
    const char* vertexShaderSource = 
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec3 color;\n"
        "mat4 scaleMatrix = mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);\n"
        "out vec3 vertexColor;\n"
        "out float fValue;"
        "void main()\n"
        "{\n"
        "   vec4 pos = scaleMatrix * vec4(position.x, position.y, 0.0, 1.0);"
        "   gl_Position = pos;\n"
        "   vertexColor = color;\n"
        "   fValue = 0.3f;\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 FragColor;\n"
        "in float fValue;"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, fValue);\n"
        "}\0";
    
    int success;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    return shaderProgram;
}