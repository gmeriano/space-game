#include <raylib.h>

class Asteroid
{
    public:
        Asteroid(float x, float y, float size, float speed);
        float getX();
        float getY();
        float getVelocityX();
        float getVelocityY();
        int getSize();
        void updatePosition();
        void rotateCircle(float theta, float originX, float originY);
        Vector2 calculateVelocityFromSpeed(float speed);
        void draw();
        Vector2 getCenterPoint();
        bool destroy;
        float getRadius();

    private:
        float x; 
        float y;
        float velocityX;
        float velocityY;
        int size;
        int rotation;
        bool debugMode = false;
        float circleX;
        float circleY;
};