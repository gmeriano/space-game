#include "player.h"
#include "asteroid.h"
#include <raylib.h>
#include <vector>

enum GameState { PLAYING, GAME_OVER, PAUSE, HOME };

class Game
{
    public:
        Game(int screenWidth, int screenHeight);
        void update();
        void resetGame();
        GameState gameState;
    private:
        Player* player;
        Asteroid* asteroid;
        void handleGameInput();
        void keepScore();
        Vector2 calculateThrustVector();
        bool checkCollision(Player p, Asteroid a);
        void draw();
        int screenWidth;
        int screenHeight;
        std::vector<Asteroid> asteroids;
        int collisionFrames;
        int score;
};