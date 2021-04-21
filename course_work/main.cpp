#include <SFML/Graphics.hpp>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <math.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "generator.h"

//TODO: blend glasses
//TODO: sort glasses
//TODO: cull glass back faces
//TODO: torches

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);

    glewInit();

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec4 pointLightPositions[] = {
        glm::vec4( 0.7f,  0.2f,  2.0f, 1.0f),
        glm::vec4( 2.3f, -3.3f, -4.0f, 1.0f),
        glm::vec4(-4.0f,  2.0f, -12.0f, 1.0f),
        glm::vec4( 0.0f,  0.0f, -3.0f, 1.0f)
    };  

    // Setting up light VAO
    GLuint lampVAO, lampVBO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);

    glBindVertexArray(lampVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT, GL_TRIANGLES);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
     //тест глубины для нормального отображения полигонов
    glEnable(GL_DEPTH_TEST);

    bool keys[sf::Keyboard::KeyCount] = {false};

    Camera cam(&window, glm::vec3(0, 5, 0));
    cam.setKeys(keys);

    Shader shader(&cam, "../combined_light.vert", "../combined_light.frag");
    Shader glassShader(&cam, "../combined_light.vert", "../glass.frag");
    Shader lampShader(&cam, "../lamp.vert", "../lamp.frag");

    std::map<char, Model*> models;
    models['c'] = new Model("../models/cobbleblock/cobbleblock.obj");
    models['d'] = new Model("../models/doorblock/doorblock.obj");
    models['@'] = new Model("../models/logblock/logblock.obj");
    models['r'] = new Model("../models/stairsblock/stairsblock.obj");
    models['g'] = new Model("../models/grassblock/grassblock.obj");
    models['#'] = new Model("../models/plankblock/plankblock.obj");
    models['!'] = new Model("../models/torchblock/torch.obj");
    models['w'] = new Model("../models/glassblock/glassblock.obj");
    //TODO: add fence

    Generator generator(100, 100, 10);
    Scene scene = generator.getScene();

    sf::Clock clock;
    sf::Clock fpsClock;
    int fps = 0;

    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                keys[event.key.code] = true;
                if(event.key.code == sf::Keyboard::F)
                    cam.changeCursorCapchure();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                keys[event.key.code] = false;
            }
        }

        if(clock.getElapsedTime().asMilliseconds() < 20)
            continue;

        clock.restart();
        
        cam.update();

        glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

        //glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

        shader.Use();

        shader.uniform("material.shininess", 64.0f);
        shader.uniform("dirLight.direction", cam.View()*glm::vec4(-0.1f, -0.4f, -1.4f, 0.0f));  
        shader.uniform("dirLight.ambient", ambientColor);
        shader.uniform("dirLight.diffuse", diffuseColor);
        shader.uniform("dirLight.specular", glm::vec3(1.0f));

        /*for (int i = 0; i < 4; i++)
        {
            std::string s("pointLights[");
            s += std::to_string(i) + "]";
            shader.uniform((s+".ambient").c_str(), ambientColor);
            shader.uniform((s+".diffuse").c_str(), diffuseColor); // darken the light a bit to fit the scene
            shader.uniform((s+".specular").c_str(), glm::vec3(1.0f));
            shader.uniform((s+".constant").c_str(), 1.0f);
            shader.uniform((s+".linear").c_str(), 0.09f);
            shader.uniform((s+".quadratic").c_str(), 0.032f); 
            shader.uniform((s+".position").c_str(), cam.View()*pointLightPositions[i]);
        }*/

        /*glm::vec3 camPos = cam.View()*glm::vec4(cam.Position(), 1.0f);
        glm::vec3 camDir = glm::normalize(cam.View()*glm::vec4(cam.Direction(), 0.0f));
        shader.uniform("projectedLight.position", camPos);
        shader.uniform("projectedLight.direction", camDir);
        shader.uniform("projectedLight.ambient", ambientColor);
        shader.uniform("projectedLight.diffuse", diffuseColor);
        shader.uniform("projectedLight.specular",glm::vec3(1.0f));
        shader.uniform("projectedLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.uniform("projectedLight.cutOffOut", glm::cos(glm::radians(17.5f)));
        shader.uniform("projectedLight.constant", 1.0f);
        shader.uniform("projectedLight.linear", 0.09f);
        shader.uniform("projectedLight.quadratic", 0.032f);*/
        
        shader.uniform("viewPos", cam.Position());   

        for(auto block : scene)
        {
            glm::mat4 model(1.0f);
            int x, y, z;
            Generator::getCoords(block.first, &x, &y, &z);
            model = glm::translate(model, glm::vec3(x, y, z));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            shader.uniform("model", model);
            if(block.second != '0')
                models.at(block.second)->Draw(shader);
        }

        /*glassShader.Use();
        {
            /*glassShader.uniform("material.shininess", 64.0f);
            glassShader.uniform("dirLight.direction", cam.View()*glm::vec4(-0.1f, -0.4f, -1.4f, 0.0f));  
            glassShader.uniform("dirLight.ambient", ambientColor);
            glassShader.uniform("dirLight.diffuse", diffuseColor);
            glassShader.uniform("dirLight.specular", glm::vec3(1.0f));
            glassShader.uniform("viewPos", cam.Position());
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            glassShader.uniform("model", model);
            glass.Draw(glassShader);
        }*/

        // Rendering lamp
        lampShader.Use();
        glBindVertexArray(lampVAO);  

        /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 modell(1.0f);
        modell = glm::translate(modell, glm::vec3(0.0f));
        lampShader.uniform("model", modell);        
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        /*for(int i = 0; i < 4; i++)
        {
            glm::mat4 model(1.0f);
            glm::vec3 vec(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            model = glm::translate(model, vec);
            model = glm::scale(model, glm::vec3(0.2f));
            lampShader.uniform("model", model);        
            glDrawArrays(GL_TRIANGLES, 0, 36);        
        }*/


        glBindVertexArray(0);
        


        //swap buffers
        window.display();
    }

    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);

    return 0;
}