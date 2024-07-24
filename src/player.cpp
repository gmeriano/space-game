#include "player.h"
#include <raylib.h>
#include "constants.h"
#include <iostream>
#include <math.h>

Texture2D IDLE_TEXTURE;
Texture2D MOVE_TEXTURE;
Texture2D BOOST_TEXTURE;
Player::Player(float x, float y, float thrust)
{
    this -> x = x;
    this -> y = y;
    this -> thrust = thrust;
    velocityX = 0.0f;
    velocityY = 1.0f;
    size = 1;
    rotation = 0;
    IDLE_TEXTURE = LoadTexture("./res/player/Idle.png");
    MOVE_TEXTURE = LoadTexture("./res/player/Move.png");
    BOOST_TEXTURE = LoadTexture("./res/player/Boost.png");
    playerState = IDLE;
    width = IDLE_TEXTURE.width;
    height = IDLE_TEXTURE.height;
    fuel = 1;
    debugMode = true;
    boostRecharging = 0;
    collisionRect = {x - (float)IDLE_TEXTURE.width * size / 8, y-(float)IDLE_TEXTURE.height * size * 0.9f / 2, (float)IDLE_TEXTURE.width * size / 4, (float)IDLE_TEXTURE.height * size * 0.9f};
}

float Player::getX() {
    return x;
}

float Player::getY() {
    return y;
}

float Player::getWidth() {
    return width;
}

float Player::getHeight() {
    return height;
}

float Player::getVelocityX() {
    return velocityX;
}

float Player::getVelocityY() {
    return velocityY;
}

int Player::getSize() {
    return size;
}

Rectangle Player::getCollisionRect()
{
    return collisionRect;
}

void Player::setRotation(Vector2 velocity)
{
    rotation = 90 - atan2(velocity.x, velocity.y) * 180 / PI;
}

void Player::updateVelocities(float deltaX, float deltaY) {
    velocityX += deltaX * thrust;
    velocityY += deltaY * thrust;
}

void Player::updatePosition() {
    x += velocityX;
    y += velocityY;
    collisionRect.x += velocityX;
    collisionRect.y += velocityY;

    if (x > SCREEN_WIDTH + size/2.0) {
        x = 0 - size/2;
        collisionRect.x = x - (float)IDLE_TEXTURE.width * size / 8;
    }
    if (x < 0 - size/2) {
        x = SCREEN_WIDTH + size/2.0;
        collisionRect.x = x - (float)IDLE_TEXTURE.width * size / 8;
    }
    if (y < 0) {
        y = size;
        collisionRect.y = y - (float)IDLE_TEXTURE.height * size * 0.9f / 2;
        velocityY = 0.5;
    }
}

void Player::resetPlayer() {
    x = (float)SCREEN_WIDTH / 2.0;
    y = (float)SCREEN_HEIGHT * 3.0 / 4.0;
    thrust = 0.05;
    velocityX = 0.0f;
    velocityY = 1.0f;
    rotation = 0;
    playerState = IDLE;
    fuel = 1;
    boostRecharging = 0;
    collisionRect = {x - (float)IDLE_TEXTURE.width * size / 8, y-(float)IDLE_TEXTURE.height * size * 0.9f / 2, (float)IDLE_TEXTURE.width * size / 4, (float)IDLE_TEXTURE.height * size * 0.9f};
}

void Player::move() {
    playerState = MOVE;
    if (fuel < 1) {
        fuel += 0.001;
    }
    if (fuel > 1) {
        fuel = 1;
    }
}

void Player::boost() {
    playerState = BOOST;
    if (fuel > 0) {
        fuel -= 0.03;
    }
    if (fuel <= 0) {
        playerState = IDLE;
        boostRecharging = 60;
        fuel = 0;
    }
}

void Player::idle() {
    playerState = IDLE;
    if (fuel < 1) {
        fuel += 0.005;
    }
    if (fuel > 1) {
        fuel = 1;
    }
}
    
void Player::draw() {  

    drawFuel();

    float originalTextureWidth = IDLE_TEXTURE.width;
    float originalTextureHeight = IDLE_TEXTURE.height;
    float textureWidth = IDLE_TEXTURE.width * size;
    float textureHeight = IDLE_TEXTURE.height * size;
    switch (playerState) {
        case MOVE:
            DrawTexturePro(MOVE_TEXTURE, {0, 0, originalTextureWidth, originalTextureHeight}, 
                {x, y, textureWidth, textureHeight}, 
                {textureWidth / 2.0f, textureHeight / 2.0f}, 
                rotation, RAYWHITE);
            break;
        case BOOST:
            DrawTexturePro(BOOST_TEXTURE, {0, 0, originalTextureWidth, originalTextureHeight}, 
                {x, y, textureWidth, textureHeight}, 
                {textureWidth / 2.0f, textureHeight / 2.0f}, 
                rotation, RAYWHITE);
            break;
        case IDLE:
            DrawTexturePro(IDLE_TEXTURE, {0, 0, originalTextureWidth, originalTextureHeight}, 
                {x, y, textureWidth, textureHeight}, 
                {textureWidth / 2.0f, textureHeight / 2.0f}, 
                rotation, RAYWHITE);
            break;
        default:
            DrawTexturePro(IDLE_TEXTURE, {0, 0, originalTextureWidth, originalTextureHeight}, 
                {x, y, textureWidth, textureHeight}, 
                {textureWidth / 2.0f, textureHeight / 2.0f}, 
                rotation, RAYWHITE);
    }

    // Draw collision rect.
    if (debugMode) {
         DrawRectangleLines(collisionRect.x, collisionRect.y, collisionRect.width, collisionRect.height, RED);
    }
}

bool Player::canBoost()
{
    if (fuel > 0 && boostRecharging == 0) {
        return true;
    }
    return false;
}

void Player::drawFuel()
{
    if (boostRecharging > 0) {
        boostRecharging--;
    }
    DrawRectangle(SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - IDLE_TEXTURE.height, IDLE_TEXTURE.width/2, IDLE_TEXTURE.height*2, LIGHTGRAY);
    DrawRectangle(SCREEN_WIDTH - 200 + 20, SCREEN_HEIGHT / 2 - IDLE_TEXTURE.height * fuel, IDLE_TEXTURE.width/2 - 40, 2 * IDLE_TEXTURE.height * fuel, ORANGE);
}