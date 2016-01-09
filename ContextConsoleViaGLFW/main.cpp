#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>

// Checking if a shader compiled successfully
void checkStatus(GLuint &shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("%d\n", status == GL_TRUE);
    // If status is equal to GL_TRUE, then your shader was compiled successfully. 
}

int main() {
    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
                                                                                 //GLFWwindow* fullScreen = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

                                                                                 // set context
    glfwMakeContextCurrent(window);

    // glew
    // =======================================
    glewExperimental = GL_TRUE;
    glewInit();
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    printf("%u\n", vertexBuffer);
    // =======================================

    // open gl code
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);

    // Vertex Array Objects: stored information of vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object: stored information of vertices
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // load memory into video card
    // use element memory could reuse data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(elements), elements, GL_STATIC_DRAW);

    // code to be compiled in video card to be the shader
    const char *vertexSource =
        "#version 150\n     \
        in vec2 position;   \
        in vec3 color;      \
                            \
        out vec3 Color;     \
        void main() {       \
            Color = color;  \
            gl_Position = vec4(position, 0.0, 1.0); \
        }";
    const char *fragmentSource =
        "#version 150\n     \
        in vec3 Color;      \
                            \
        out vec4 outColor;  \
                            \
        void main() {       \
            outColor = vec4(Color, 1.0);    \
        }";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    checkStatus(vertexShader);

    // Retrieving the compile log
    // char buffer[512];
    // glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    checkStatus(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // explicity spceify which buffer is written to which buffer, cause there could have mutible buffers
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)(2 * sizeof(float)));

    // use uniform to change the arrtibute of the triganle
    // GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    // glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);

    // let the color change with time
    // auto t_start = std::chrono::high_resolution_clock::now();

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /*auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        glUniform3f(uniColor, 0.0f, 0.0f, (sin(time * 5.0f) + 1.0f) / 2.0f);*/

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    // end
    glfwTerminate();
    return 0;
}