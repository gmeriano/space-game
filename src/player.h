#include <raylib.h>

enum PlayerState { IDLE, MOVE, BOOST };

class Player
{
    public:
        Player(float x, float y, float thrust);
        float getX();
        float getY();
        void resetPlayer();
        float getWidth();
        float getHeight();
        float getVelocityX();
        float getVelocityY();
        int getSize();
        Rectangle getCollisionRect();
        void setRotation(Vector2 angle);
        void updateVelocities(float deltaX, float deltaY);
        void updatePosition();
        void draw();
        void drawFuel();
        void move();
        void boost();
        void idle();
        bool canBoost();

    private:
        float x; 
        float y;
        float width;
        float height;
        float thrust;
        float velocityX;
        float velocityY;
        int size;
        bool debugMode;
        float rotation;
        PlayerState playerState;
        Rectangle collisionRect;
        float fuel;
        int boostRecharging;
};