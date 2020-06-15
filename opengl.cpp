#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>

#include "Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "tests/TestClearColor.h"

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(800, 800, "shtcd game", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Err" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float bgPositions[] = {               // texture start (0,0) bottom left
             -400.0f, -400.0f,  0.0f, 0.0f, // 0
              400.0f, -400.0f,  1.0f, 0.0f, // 1
              400.0f,  400.0f,  1.0f, 1.0f, // 2
             -400.0f,  400.0f,  0.0f, 1.0f  // 3
        };

        float shapePositions[] = {             // texture start (0,0) bottom left
                -100.0f, -100.0f,  0.0f, 0.0f, // 0
                 100.0f, -100.0f,  1.0f, 0.0f, // 1
                 100.0f,  100.0f,  1.0f, 1.0f, // 2
                -100.0f,  100.0f,  0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {    // 2 triangles = square
                0, 1, 2,
                2, 3, 0
        };

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        VertexArray va;
        VertexBuffer vb(bgPositions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        VertexArray shapesVA;
        VertexBuffer shapesVB(shapePositions, 4 * 4 * sizeof(float));
        shapesVA.AddBuffer(shapesVB, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f); // inner window boundaries
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0 ,0));
        glm::mat4 viewProj = proj * view;


        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(400, 400, 0));
        glm::mat4 backgroundMVP = viewProj * model;

        glm::mat4 shapeModel0 = glm::translate(glm::mat4(1.0f), glm::vec3(120, 680, 0));
        glm::mat4 shapeMVP0 = viewProj * shapeModel0;

        glm::mat4 shapeModel1 = glm::translate(glm::mat4(1.0f), glm::vec3(393, 680, 0));
        glm::mat4 shapeMVP1 = viewProj * shapeModel1;

        glm::mat4 shapeModel2 = glm::translate(glm::mat4(1.0f), glm::vec3(666, 680, 0));
        glm::mat4 shapeMVP2 = viewProj * shapeModel2;

        glm::mat4 shapeModel3 = glm::translate(glm::mat4(1.0f), glm::vec3(120, 407, 0));
        glm::mat4 shapeMVP3 = viewProj * shapeModel3;

        glm::mat4 shapeModel4 = glm::translate(glm::mat4(1.0f), glm::vec3(393, 407, 0));
        glm::mat4 shapeMVP4 = viewProj * shapeModel4;

        glm::mat4 shapeModel5 = glm::translate(glm::mat4(1.0f), glm::vec3(666, 407, 0));
        glm::mat4 shapeMVP5 = viewProj * shapeModel5;

        glm::mat4 shapeModel6 = glm::translate(glm::mat4(1.0f), glm::vec3(120, 134, 0));
        glm::mat4 shapeMVP6 = viewProj * shapeModel6;

        glm::mat4 shapeModel7 = glm::translate(glm::mat4(1.0f), glm::vec3(393, 134, 0));
        glm::mat4 shapeMVP7 = viewProj * shapeModel7;

        glm::mat4 shapeModel8 = glm::translate(glm::mat4(1.0f), glm::vec3(666, 134, 0));
        glm::mat4 shapeMVP8 = viewProj * shapeModel8;

        glm::mat4 circles[] = {
                shapeMVP0, shapeMVP1, shapeMVP7, shapeMVP4, shapeMVP8
        };

        glm::mat4 crosses[] = {
                shapeMVP2, shapeMVP3, shapeMVP5, shapeMVP6
        };

        Shader shader("res/shaders/test.shader");

        Texture bg("res/textures/background.jpg");
        Texture circle("res/textures/circle.png");
        Texture cross("res/textures/cross.png");

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            renderer.Clear();

            shader.SetUniform1i("u_Texture", 0);
            shader.SetUniformMat4f("u_MVP", backgroundMVP);
            bg.Bind();
            renderer.Draw(va, ib, shader);

            cross.Bind();
            for (auto const &shapeMVP : crosses)
            {
                shader.SetUniformMat4f("u_MVP", shapeMVP);
                renderer.Draw(shapesVA, ib, shader);
            }

            circle.Bind();
            for (auto const &shapeMVP : circles)
            {
                shader.SetUniformMat4f("u_MVP", shapeMVP);
                renderer.Draw(shapesVA, ib, shader);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

