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
#include "scene.h"

//TODO: lighting

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 4;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(1800, 1000), "OpenGL", sf::Style::Default, settings);

    glewInit();
    //glEnable(GL_MULTISAMPLE);  
    
    glPolygonMode(GL_FRONT, GL_TRIANGLES);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
     //тест глубины для нормального отображения полигонов
    glEnable(GL_DEPTH_TEST);

    bool keys[sf::Keyboard::KeyCount] = {false};

    Camera cam(&window, glm::vec3(0, 5, 0), Camera::FPS);
    cam.setKeys(keys);

    Shader shader(&cam, "../combined_light.vert", "../glass.frag");
    Shader lampShader(&cam, "../lamp.vert", "../lamp.frag");

    //TODO: add fence
    Scene scene("../model_list");

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

        if(clock.getElapsedTime().asMilliseconds() < 10)
            continue;

        clock.restart();
        
        cam.update();

        glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        scene.draw(shader);

        //swap buffers
        window.display();
    }

    return 0;
}