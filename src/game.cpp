#include "game.h"
#include <raylib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <ctime>     // For the time function


float lastUpdateTime = 0;
float difficulty = 3;
Texture explosion;
bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

float GetRandomFloat(float min, float max) {
    return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

Game::Game(int screenWidth, int screenHeight)
{
    this -> screenHeight = screenHeight;
    this -> screenWidth = screenWidth;
    player = new Player((float)screenWidth / 2.0, (float)screenHeight * 3.0 / 4.0, 0.05);
    asteroid = new Asteroid(screenWidth, 0, 1, 3);
    asteroids.push_back(*asteroid);
    explosion = LoadTexture("./res/Explosion_6.png");
    explosion.width = player->getWidth();
    explosion.height = player->getHeight();
    collisionFrames = 5;
    score = 0;
    srand((unsigned int)time(NULL));
}
float scoreCounter = 0;
void Game::update()
{
    bool colliding = false;
    switch (gameState) {
    case PLAYING: 
        handleGameInput();
        draw();

        keepScore();

        if(EventTriggered(difficulty)) {
            rand();
            float randX = GetRandomFloat(0, screenWidth-50);
            rand();
            float randSize = GetRandomFloat(1, 3);
            rand();
            float randSpeed = GetRandomFloat(5,30);
            asteroids.push_back(Asteroid(randX,0,randSize,randSpeed/(randSize)));
        }
        if (player->getY() > screenHeight) {
            gameState = GAME_OVER;
        }
        for (Asteroid& currAsteroid : asteroids) {
            if(checkCollision(*player, currAsteroid)) {
                colliding = true;
                collisionFrames--; 
            }
            if (collisionFrames <= 0) {
                gameState = GAME_OVER;
            }
        }
        if (!colliding) {
            collisionFrames = 4;
        }
        break;
    case PAUSE:
        // TODO Resume button
        // TODO make a UI class for these cases below maybe
        break;
    case GAME_OVER:
        // TODO display score, button for new game or home screen
        DrawTexture(explosion, player->getX() - player->getWidth() / 2, player->getY() - player->getHeight() / 2, WHITE);
        char scoreText[10];
        sprintf(scoreText, "%d", score);
        DrawText("GAME OVER", screenWidth / 2 - 200, screenHeight / 2, 50, WHITE);
        DrawText("Your Score Is:", screenWidth / 2 - 200, screenHeight / 2 + 55, 50, WHITE);
        DrawText(scoreText, screenWidth / 2 - 200, screenHeight / 2 + 110, 50, WHITE);
        DrawText("Press ENTER restart Game", screenWidth / 2 - 200, screenHeight / 2 + 165, 50, WHITE);
        if (IsKeyDown(KEY_ENTER)) {
            resetGame();
        }
        break;
    case HOME:
        // TODO button for new game and display high score
        DrawText("SPACE GAME", screenWidth / 2 - 200, screenHeight / 2, 50, WHITE);
        DrawText("Press ENTER to begin Game!", screenWidth / 2 - 200, screenHeight / 2 + 55, 50, WHITE);
        if (IsKeyDown(KEY_ENTER)) {
            gameState = PLAYING;
        }
        break;
    }
}

void Game::resetGame()
{
    player->resetPlayer();
    asteroids.clear();
    asteroid = new Asteroid(screenWidth, 0, 1, 3);
    asteroids.push_back(*asteroid);
    collisionFrames = 5;
    score = 0;
    scoreCounter = 0;
    gameState = PLAYING;
}

void Game::handleGameInput()
{
    if (IsMouseButtonDown(1)) {
        if (player->canBoost()) {
            Vector2 newVelocity = calculateThrustVector();
            player->updateVelocities(5 * newVelocity.x, 5 * newVelocity.y);
            player->boost();
        }
        else {
            player->idle();
        }
    }
    else if (IsMouseButtonDown(0)) {
        Vector2 newVelocity = calculateThrustVector();
        player->updateVelocities(newVelocity.x, newVelocity.y);
        player->move();
    }
    else {
        player->idle();
    }
}

void Game::keepScore()
{
    if (scoreCounter > 30) {
        score++;
        std::cout<<"SCORE: " << score<<"\n";
        scoreCounter = 0;
    }
    scoreCounter++;
    if (score > 50) {
        difficulty = 2;
    }
    if (score > 100) {
        difficulty = 1;
    }
    if (score > 300) {
        difficulty = 0.5;
    }
    if (score > 1000) {
        difficulty = 0.2;
    }
}

Vector2 Game::calculateThrustVector() {
    Vector2 mousePos = GetMousePosition();
    float deltaX = mousePos.x - player->getX();
    float deltaY = mousePos.y - player->getY();
    float norm = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
    float directionX = deltaX / norm;
    float directionY = deltaY / norm;
    return {directionX, directionY};
}

bool Game::checkCollision(Player p, Asteroid a)
{
    Rectangle playerBox = p.getCollisionRect();
    Vector2 asteroidCenter = a.getCenterPoint();

    float closestX;
    if (asteroidCenter.x < playerBox.x) {
        closestX = playerBox.x;
    }
    else if (asteroidCenter.x > playerBox.x + playerBox.width) {
        closestX = playerBox.x + playerBox.width;
    }
    else {
        closestX = asteroidCenter.x;
    }

    float closestY;
    if (asteroidCenter.y < playerBox.y) {
        closestY = playerBox.y;
    }
    else if (asteroidCenter.y > playerBox.y + playerBox.height) {
        closestY = playerBox.y + playerBox.height;
    }
    else {
        closestY = asteroidCenter.y;
    }

    if (sqrt(pow(closestX - asteroidCenter.x, 2) + pow(closestY - asteroidCenter.y, 2)) < a.getRadius()) {
        return true;
    }
    return false;
}

void Game::draw() {
    player->updatePosition();
    player->setRotation(calculateThrustVector());
    player->draw();

    int counter = 0;
    for (Asteroid& asteroid : asteroids) {
        Vector2 velocity = calculateThrustVector();
        asteroid.updatePosition();
        asteroid.rotateCircle(PI + atan2(velocity.x, velocity.y), player->getX(), player->getY());
        asteroid.draw();
        if (asteroid.destroy) {
            asteroids.erase(asteroids.begin() + counter);
        }
        counter++;
    }

    char scoreText[10];
    sprintf(scoreText, "%d", score);
    DrawText(scoreText, screenWidth * 7 / 8, screenHeight / 8, 50, WHITE);

}