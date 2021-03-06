#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// code to be compiled in video card to be the shader
const GLchar *vertexSource =
"#version 150 core\n     \
in vec3 position;   \
in vec3 color;      \
in vec2 texcoord;   \
\
out vec3 Color;     \
out vec2 Texcoord;  \
\
uniform mat4 model; \
uniform mat4 view;  \
uniform mat4 proj;  \
uniform vec3 overrideColor; \
\
void main() {       \
    Color = overrideColor * color;  \
    Texcoord = texcoord;    \
    gl_Position = proj * view * model * vec4(position, 1.0); \
}";

/* The mix function here is a special GLSL function that 
linearly interpolates between two variables based on the third parameter. 
A value of 0.0 will result in the first value, a value of 1.0 will result in the second value 
and a value in between will result in a mixture of both values. */
const GLchar *fragmentSource =
"#version 150 core\n     \
in vec3 Color;      \
in vec2 Texcoord;   \
\
uniform sampler2D texKitten;    \
uniform sampler2D texPuppy;     \
\
out vec4 outColor;  \
\
void main() {       \
    vec4 texColor = mix(texture(texKitten, Texcoord),   \
        texture(texPuppy, Texcoord), 0.5);                  \
outColor = vec4(Color, 1.0);             \
}";

// Checking if a shader compiled successfully
void checkStatus(GLuint &shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("%d\n", status == GL_TRUE);
    // If status is equal to GL_TRUE, then your shader was compiled successfully. 
}

GLFWwindow* initContext() {
    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
    //GLFWwindow* fullScreen = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

    glfwMakeContextCurrent(window); // set context

    // glew
    // =======================================
    glewExperimental = GL_TRUE;
    glewInit();
    // GLuint vertexBuffer;
    // glGenBuffers(1, &vertexBuffer);
    // printf("%u\n", vertexBuffer);
    // =======================================
    return window;
}

void mainLoop(GLFWwindow* _window) {

}

int main() {
    GLFWwindow* window = initContext();

    // Vertex Array Objects: stored information of vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object: stored information of vertices
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer

    // open gl code
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // use element to draw things
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[36] = {
        30, 31, 32,
        30, 34, 32
    };

    for (int i = 1; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            elements[i * 6 + j] = elements[(i - 1) * 6 + j] - 6;
        }
    }    

    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    // load memory into video card
    // use element memory could reuse data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
     sizeof(elements), elements, GL_STATIC_DRAW);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    checkStatus(vertexShader);

    // Retrieving the compile log
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    checkStatus(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // explicity spceify which buffer is written to which buffer, cause there could have mutible buffers
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
      8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    // load texture
    // two texture image here
    GLuint textures[2];
    glGenTextures(2, textures);

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    image = SOIL_load_image("./sample.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    printf("width = %d, height = %d\n", width, height);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    image = SOIL_load_image("./sample2.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    printf("width = %d, height = %d\n", width, height);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    //using this to set color of the border if we use clamp to border or something
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    //the texture could be a 2-D array, like this ( or a file naturally
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);

    // matrix rotate
    //glm::mat4 trans;
    //trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //printf("%f, %f, %f\n", result.x, result.y, result.z);
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    /*The second parameter of the glUniformMatrix4fv function specifies
    how many matrices are to be uploaded,
    because you can have arrays of matrices in GLSL.
    The third parameter specifies whether the specified matrix should be transposed before usage.
    This is related to the way matrices are stored as float arrays in memory;
    you don't have to worry about it.
    The last parameter specifies the matrix to upload,
    where the glm::value_ptr function converts the matrix class into an array of 16 (4x4) floats.*/
    //glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

    glm::mat4 view = glm::lookAt(
        glm::vec3(1.5f, 1.5f, 1.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
        );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    GLint uniColor = glGetUniformLocation(shaderProgram, "overrideColor");

    auto t_start = std::chrono::high_resolution_clock::now();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen to white
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        glm::mat4 model;
        model = glm::rotate(
            model,
            time * glm::radians(180.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
            );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        // Draw a rectangle from the 2 triangles using 6 indices
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glEnable(GL_STENCIL_TEST);

        // Draw floor
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF); // Write to stencil buffer
        glDepthMask(GL_FALSE); // Don't write to depth buffer
        glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

        glDrawArrays(GL_TRIANGLES, 36, 6);

        // Draw cube reflection
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
        glStencilMask(0x00); // Don't write anything to stencil buffer
        glDepthMask(GL_TRUE); // Write to depth buffer

        model = glm::scale(
            glm::translate(model, glm::vec3(0, 0, -1)),
            glm::vec3(1, 1, 1)
            );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        
        glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

        glDisable(GL_STENCIL_TEST);

        //glUniform3f(uniColor, 0.0f, 0.0f, (sin(time * 5.0f) + 1.0f) / 2.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    //exit
    glDeleteTextures(2, textures);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    // glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}