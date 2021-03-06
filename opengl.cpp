#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <cstring>

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


bool CheckWin(glm::mat4 *side, glm::mat4 *shapeMVPS)
{
    bool winCases[8] = {
            side[0] == shapeMVPS[0] && side[1] == shapeMVPS[1] && side[2] == shapeMVPS[2],
            side[3] == shapeMVPS[3] && side[4] == shapeMVPS[4] && side[5] == shapeMVPS[5],
            side[6] == shapeMVPS[6] && side[7] == shapeMVPS[7] && side[8] == shapeMVPS[8],
            side[0] == shapeMVPS[0] && side[4] == shapeMVPS[4] && side[8] == shapeMVPS[8],
            side[2] == shapeMVPS[2] && side[4] == shapeMVPS[4] && side[6] == shapeMVPS[6],
            side[0] == shapeMVPS[0] && side[3] == shapeMVPS[3] && side[6] == shapeMVPS[6],
            side[1] == shapeMVPS[1] && side[4] == shapeMVPS[4] && side[7] == shapeMVPS[7],
            side[2] == shapeMVPS[2] && side[5] == shapeMVPS[5] && side[8] == shapeMVPS[8]
    };

    for (bool winCase : winCases)
    {
        if (winCase)
            return true;
    }
    return false;
}


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

        unsigned int shapeScreenPositions[9][3] = {         // bottom left origin
                {120, 680, 0}, {393, 680, 0}, {666, 680, 0},
                {120, 407, 0}, {393, 407, 0}, {666, 407, 0},
                {120, 134, 0}, {393, 134, 0}, {666, 134, 0}
        };

        unsigned int GLFWshapeScreenPositions[9][3] = {     // top left origin
                {120, 120, 0}, {393, 120, 0}, {666, 120, 0},
                {120, 393, 0}, {393, 393, 0}, {666, 393, 0},
                {120, 666, 0}, {393, 666, 0}, {666, 666, 0}
        };

        glm::mat4 shapeMVPS[9];

        for (auto i = 0; i < 9; i++)
        {
            auto& ssp = shapeScreenPositions[i];
            glm::mat4 shapeModel = glm::translate(glm::mat4(1.0f), glm::vec3(ssp[0], ssp[1], ssp[2]));
            shapeMVPS[i] = viewProj * shapeModel;
        }

        glm::mat4 circles[9];
        glm::mat4 crosses[9];

        bool gameOver = false;
        bool grid[9] = { false };

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

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !gameOver)
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                auto& GLssp = GLFWshapeScreenPositions;
                for (auto i = 0; i < 9; i++)
                    if (xpos >= GLssp[i][0]-100 && xpos <= GLssp[i][0]+100 && ypos >= GLssp[i][1]-100  && ypos <= GLssp[i][1]+100 && !grid[i])
                    {
                        grid[i] = true;
                        crosses[i] = shapeMVPS[i];
                        if (CheckWin(crosses, shapeMVPS)) {
                            gameOver = true;
                            std::cout << "Win" << std::endl;
                            break;
                        }
                        std::vector<unsigned short int> freeCells;
                        for (auto j = 0; j < 9; j++)
                            if (!grid[j])
                                freeCells.push_back(j);
                        if (freeCells.empty()) {
                            gameOver = true;
                            std::cout << "Draw" << std::endl;
                            break;
                        }
                        auto index = freeCells[rand() % freeCells.size()];
                        grid[index] = true;
                        circles[index] = shapeMVPS[index];
                        if (CheckWin(circles, shapeMVPS)) {
                            gameOver = true;
                            std::cout << "Lose" << std::endl;
                        }
                        break;
                    }
            }

            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && gameOver)
            {
                gameOver = false;
                glm::mat4 newMat4 [9];
                bool newGrid [9] = { false };
                std::memcpy(crosses, newMat4, sizeof(crosses));
                std::memcpy(circles, newMat4, sizeof(circles));
                std::memcpy(grid, newGrid, sizeof(grid));
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

