#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource =    "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\0";

const char* fragmentShaderSource1 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(0.2f, 0.2f, 1.0f, 0.4f);\n"
                                    "}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(1.0f, 1.0f, 0.0f, 0.4f);\n"
                                    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1100, 750, "Questao 1", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Erro ao inicializar GLAD\n";
        return -1;
    }

    // Definição dos vértices do retângulo (2 triângulos)
    float triangle1[] = {
         0.5f,  1.0f, 0.0f,  // topo direito
         1.0f, -0.5f, 0.0f,  // base direita
         0.0f, -0.5f, 0.0f,  // base esquerda
    }; 

    float triangle2[] = {
         0.0f, -0.5f, 0.0f,  // base direita
        -0.5f,  1.0f, 0.0f,  // topo esquerdo
        -1.0f, -0.5f, 0.0f   // base esquerda
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compilação dos shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

	unsigned int fragmentShader[2];
    fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader[0], 1, &fragmentShaderSource1, nullptr);
    glCompileShader(fragmentShader[0]);

    fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader[1], 1, &fragmentShaderSource2, nullptr);
    glCompileShader(fragmentShader[1]);

	unsigned int shaderProgram[2];
    shaderProgram[0] = glCreateProgram();
    glAttachShader(shaderProgram[0], vertexShader);
    glAttachShader(shaderProgram[0], fragmentShader[0]);
    glLinkProgram(shaderProgram[0]);

    shaderProgram[1] = glCreateProgram();
    glAttachShader(shaderProgram[1], vertexShader);
    glAttachShader(shaderProgram[1], fragmentShader[1]);
    glLinkProgram(shaderProgram[1]);

    // Verificação de erros na compilação do shader
    int success;
    char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "Erro de compilação do shader (vertex): " << infoLog << std::endl;
	}

	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[0], 512, nullptr, infoLog);
		std::cout << "Erro de compilação do shader (fragment): " << infoLog << std::endl;
	}

    glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader[1], 512, nullptr, infoLog);
        std::cout << "Erro de compilação do shader (fragment): " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader[0]);
    glDeleteShader(fragmentShader[1]);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram[0]);
        // draw first triangle using the data from the first VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram[1]);
        // then we draw the second triangle using the data from the second VAO
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram[0]);
    glDeleteProgram(shaderProgram[1]);

    glfwTerminate();
    return 0;
}