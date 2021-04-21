#include "Camera.h"

Camera::Camera(sf::Window* window, glm::vec3 position, Mode mode)
    : window(window)
    , view(1.0f)
    , projection(1.0f)
    , position(position)
    , front(0.0f, 0.0f, -1.0f)
    , up(0.0f, 1.0f, 0.0f)
    , rotation(0.0f)
    , pitch(0.0f)
    , yaw(-90.0f)
    , sensitivity(0.05f)
    , FOV(glm::radians(45.0f))
    , speed(0.2f)
    , mode(mode)
    , cursorCapchure(false)
{
    changeCursorCapchure();
}

void Camera::handle()
{
    if(keys[sf::Keyboard::W])
        moveForward();
    if(keys[sf::Keyboard::S])
        moveBackward();
    if(keys[sf::Keyboard::A])
        moveLeft();
    if(keys[sf::Keyboard::D])
        moveRight();
    if(keys[sf::Keyboard::E])
        moveUp();
    if(keys[sf::Keyboard::Q])
        moveDown();
}

void Camera::rotate()
{
    if(!cursorCapchure)
        return;

    sf::Vector2i shift = sf::Mouse::getPosition(*window);
    sf::Vector2i center(window->getSize().x / 2, window->getSize().y / 2);
    shift -= center;
    sf::Mouse::setPosition(center, *window);

    yaw += shift.x * sensitivity;
    pitch -= shift.y * sensitivity;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw)); 
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    direction = front;

    if(mode == FPS)
        direction.y = 0;

    front = glm::normalize(front);
    direction = glm::normalize(direction);
}

void Camera::moveForward()
{
    position += direction * speed;
}

void Camera::moveBackward()
{
    position -= direction * speed;
}

void Camera::moveLeft()
{
    position -= glm::normalize(glm::cross(direction, up)) * speed;
}

void Camera::moveRight()
{
    position += glm::normalize(glm::cross(direction, up)) * speed;
}

void Camera::moveUp()
{
    position.y += speed;
}

void Camera::moveDown()
{
    position.y -= speed;
}

void Camera::update()
{
    handle();
    rotate();

    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(FOV, (float)window->getSize().x / window->getSize().y, 0.1f, 100.0f);
}

void Camera::changeCursorCapchure()
{
    cursorCapchure = !cursorCapchure;
    window->setMouseCursorVisible(!cursorCapchure);
    window->setMouseCursorGrabbed(cursorCapchure);
}

const glm::mat4& Camera::View()
{
    return view;
}

const glm::mat4& Camera::Projection()
{
    return projection;
}

const glm::vec3& Camera::Position()
{
    return position;
}

const glm::vec3& Camera::Direction()
{
    return direction;
}

void Camera::setMode(Mode new_mode)
{
    mode = new_mode;
}

void Camera::setFOV(GLfloat degrees)
{
    FOV = glm::radians(degrees);
}

void Camera::setKeys(bool* array)
{
    keys = array;
}