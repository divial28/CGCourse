#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

class Camera
{
public:

    enum Mode 
    {
        FLY = 0,
        FPS
    };

    Camera(sf::Window* window
           , glm::vec3 position = glm::vec3(0.0f)
           , Mode mode = FLY);

    void handle();

    void rotate();

    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();

    void moveDown();

    void moveUp();

    void update();

    void changeCursorCapchure();

    const glm::mat4& View();

    const glm::mat4& Projection();

    const glm::vec3& Position();

    const glm::vec3& Direction();

    void setMode(Mode newMode);

    void setFOV(GLfloat degrees);

    void setKeys(bool* array);

private:
    glm::mat4   view;
    glm::mat4   projection;

    glm::vec3   position;
    glm::vec3   front;
    glm::vec3   up;
    glm::vec3   direction;

    GLfloat     speed;

    GLfloat     yaw;
    GLfloat     pitch;
    GLfloat     rotation;
    GLfloat     sensitivity;

    GLfloat     FOV;

    Mode        mode;

    bool        * keys;
    bool        cursorCapchure;

    sf::Window  * window;
};

#endif