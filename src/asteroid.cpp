#include "asteroid.h"
#include <math.h>
#include "constants.h"
#include <iostream>
#include <ctime>     // For the time function


Texture2D texture;
Asteroid::Asteroid(float x, float y, float size, float speed)
{
    this -> x = x;
    this -> y = y;
    Vector2 velocity = calculateVelocityFromSpeed(speed);
    this -> velocityX = velocity.x;
    this -> velocityY = velocity.y;
    texture = LoadTexture("./res/Asteroid2.png");
    this -> size = size;
    rotation = 0;
    circleX = x;
    circleY = y;
    destroy = false;
    debugMode = true;
}

float RandomFloat(float min, float max) {
    return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

float Asteroid::getVelocityY() {
    return velocityY;
}

float Asteroid::getY() {
    return y;
}

Vector2 Asteroid::calculateVelocityFromSpeed(float speed)
{
    // create  velocity vector from spawn point to center
    float deltaX = SCREEN_WIDTH / 2.0 - x;
    float deltaY = SCREEN_HEIGHT / 2.0 - y;

    float norm = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

    float directionX = deltaX / norm;
    float directionY = deltaY / norm;

    // randomly adjust vector by PI/6 degrees
    float angleAdjustment = RandomFloat(-PI/6, PI/6);

    directionX = cos(angleAdjustment)*directionX - sin(angleAdjustment)*directionY;
    directionY = sin(angleAdjustment)*directionX + cos(angleAdjustment)*directionY;

    // add speed magnitude to vector
    return {speed*directionX, speed*directionY};
}

void Asteroid::updatePosition() {
    if (y < SCREEN_HEIGHT * 2) {
        x += velocityX;
        y += velocityY;
    }
    else {
        destroy = true;
    }
}

void Asteroid::rotateCircle(float theta, float originX, float originY) {
        circleX = (cos(theta) * (x - originX)) - (sin(theta) * (y-originY)) + originX;
        circleY = (sin(theta) * (x - originX)) + (cos(theta) * (y-originY)) + originY;
    }

void Asteroid::draw() {
    // Draw Asteroid
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, 
                {x, y, (float)texture.width * size, (float)texture.height * size}, 
                {(float)texture.width / 2.0f * size, (float)texture.height / 2.0f * size}, 
                rotation, RAYWHITE);
    if (debugMode) {
        DrawCircleLines(circleX, circleY, texture.width * size / 2, BLUE);   
    }
    rotation += 1;
}

Vector2 Asteroid::getCenterPoint()
{
    return {circleX, circleY};
}

float Asteroid::getRadius()
{
    return texture.width * size / 2;
}
