#include<iostream>
#include<glew.h>
#include<glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

// GLEW OR GLAD

unsigned int CreateMesh();
unsigned int CreateShaders();
unsigned int LoadTexture(std::string strImageName, std::string typeImage);

std::string GetMediaPath()
{
    return getenv("MEDIA_PATH");
}

std::string GetTexturePath()
{
    return GetMediaPath().append("textures\\");
}

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
    window = glfwCreateWindow(1920, 1080, "BSP Advance Diploma", NULL, NULL);
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
    unsigned int texture1 = LoadTexture(GetTexturePath().append("minion-transparent-background-9.png"), "png");
    unsigned int texture2 = LoadTexture(GetTexturePath().append("minion.jpg"), "jpg");

    int uniformcolorLocation = glGetUniformLocation(program, "uniformColor");
    int timeLocation = glGetUniformLocation(program, "time");
    int texture1Location = glGetUniformLocation(program, "Texture");
    int texture2Location = glGetUniformLocation(program, "Texture2");

    while (!glfwWindowShouldClose(window))
    {
        /* Loop until the user closes the window */
        /* Render here */
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float timeValue = glfwGetTime();

        glBindVertexArray(mesh);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUseProgram(program);
        glUniform4f(uniformcolorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
        glUniform1f(timeLocation, timeValue);
        glUniform1i(texture1Location, 0);
        glUniform1i(texture2Location, 1);

        //glDrawArrays(GL_TRIANGLES, 0, 9);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // 0
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     // 1
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,     // 2
         -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f     // 3
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        unsigned int IBO;
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
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
        "layout(location = 2) in vec2 uv;\n"

        "uniform float time;\n"
        "out vec3 vertexColor;\n"
        "out float fValue;"
        "out vec2 outUV;\n"
        "void main()\n"
        "{\n"
        "   float scale = 0.2f + abs(sin(time*0.2f))*2.0f;\n"
        "   mat4 scaleMatrix = mat4(scale, 0.0f, 0.0f, 0.0f, 0.0f, scale, 0.0f, 0.0f, 0.0f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);\n"
        "   vec4 pos = scaleMatrix * vec4(position.x, position.y, 0.0, 1.0);\n"
        "   gl_Position = pos;\n"
        "   vertexColor = color;\n"
        "   fValue = 0.3f;\n"
        "   outUV = uv;\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "in float fValue;\n"
        "in vec2 outUV;\n"
        "uniform vec4 uniformColor;\n"
        "uniform float time;\n"
        "uniform sampler2D Texture;\n"
        "uniform sampler2D Texture2;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    //float alpha = abs(sin(time*2.0f));\n"
        "    //FragColor = vec4(vertexColor, 1.0f);\n"
        "    FragColor = texture(Texture, outUV);\n"
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

unsigned int LoadTexture(std::string strImageName, std::string typeImage)
{
    int width, height, nrChannels;
    unsigned char* data = nullptr;
    GLint textureFormat = GL_RGB;
    stbi_set_flip_vertically_on_load(true);
    if(typeImage.compare("jpg") == 0)
        data = stbi_load(strImageName.c_str(), &width, &height, &nrChannels, STBI_rgb);
    else if (typeImage.compare("png") == 0)
    {
        textureFormat = GL_RGBA;
        data = stbi_load(strImageName.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    }

    if (data == nullptr)
    {
        std::cout << "Failed to load image : " << strImageName << std::endl;
        return 0;
    }

    std::cout << "Image Info : " << width << " " << height << " " << nrChannels << std::endl;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texture;
}