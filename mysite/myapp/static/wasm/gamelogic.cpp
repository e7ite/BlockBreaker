#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

class Paddle
{
    int x, y, w, h, speed;
    int score;

public:
    Paddle(int x, int y, int w, int h) 
        : x(x), y(y), w(w), h(h), speed(0), score(0) {}

    int getX() const { return x; }
    void setX(int x) { this->x = x; }
    int getY() const { return y; }
    void setY(int y) { this->y = y; }
    int getW() const { return w; }
    int getH() const { return h; }
    int getSpeed() const { return speed; }
    void setSpeed(int speed) { this->speed = speed; }
    int getScore() const { return score; }
    void setScore(int score) { this->score = score; }

    void move(emscripten::val canvas, int movementSpeed,
              bool leftKeyPressed, bool rightKeyPressed)
    {
        speed = rightKeyPressed ? movementSpeed 
                : leftKeyPressed ? -movementSpeed : 0;

        int maxX = canvas["width"].as<int>() - w;
        int minX = 0;
        x += speed;
        x = std::min(x, maxX);
        x = std::max(x, minX);
    }
};

class Ball
{
    int x, y, r;
    int dx, dy;

public:
    Ball(int x, int y, int r, int dx, int dy) 
        : x(x), y(y), r(r), dx(dx), dy(dy) {}

    int getX() const { return x; }
    void setX(int x) { this->x = x; }
    int getY() const { return y; }
    void setY(int y) { this->y = y; }
    int getR() const { return r; }
    int getDX() const { return dx; }
    void setDX(int dx) { this->dx = dx; }
    int getDY() const { return dy; }
    void setDY(int dy) { this->dy = dy; }

    bool move(Paddle paddle, emscripten::val canvas)
    {
        if (hitSideWall(canvas))
            dx = -dx;
        
        if (hitTop() || hitPaddle(paddle, canvas))
            dy = -dy;

        if (hitBottom(canvas) && !hitPaddle(paddle, canvas))
            return false;

        x += dx;
        y += dy;
        return true;
    }

    bool hitBottom(emscripten::val canvas)
    {
        return y + dy > canvas["height"].as<int>() - r;
    }

    bool hitTop()
    {
        return y + dy < r;
    }

    bool hitSideWall(emscripten::val canvas)
    {
        return x + dx > canvas["width"].as<int>() - r || x + dx < r;
    }

    bool hitPaddle(Paddle paddle, emscripten::val canvas)
    {
        return hitBottom(canvas) &&
            x > paddle.getX() && x < paddle.getX() + paddle.getW(); 
    }
};

class Brick
{
    int x, y, w, h;
    bool alive;
    int score;

public:
    Brick(int x, int y, int w, int h, int score) 
        : x(x), y(y), w(w), h(h), alive(true), score(score) {}

    int getX() const { return x; }
    void setX(int x) { this->x = x; }
    int getY() const { return y; }
    void setY(int y) { this->y = y; }
    int getW() const { return w; }
    int getH() const { return h; }
    int getScore() const { return score; }
    bool isAlive() const { return alive; }
    void markDead() { alive = false; }
};

class BrickLayout
{
    std::vector<Brick> bricks;
    int rows, cols;
    int direction;

public:

    BrickLayout(int rows, int cols, int width, int height, int padding,
                int offsetFromTop, int offsetFromLeft) 
        : rows(rows), cols(cols), direction(1)
    {
        int numRandoms = static_cast<int>(emscripten_random() * 6);

        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++) 
            {
                int score = 10;
                int randomSpecial = static_cast<int>(emscripten_random() * 10);
                if (numRandoms && randomSpecial < 1)
                {
                    score = 50;
                    numRandoms--;
                }
                bricks.push_back(Brick(i * (width + padding) + offsetFromLeft,
                                       j * (height + padding) + offsetFromTop,
                                       width, height, score));
            }
        }
    }

    int size() { return bricks.size(); }
    Brick get(int i) { return bricks[i]; }

    bool collisionDetection(emscripten::val canvas, Paddle &paddle, Ball &ball)
    {
        bool blocksLeft = false;
        int minX = 0;

        for (auto &brick : bricks)
        {
            if (!brick.isAlive())
                continue; 

            int maxX = canvas["width"].as<int>() - brick.getW();
            blocksLeft = true;

            brick.setX(brick.getX() + direction);
            if (brick.getX() > maxX || brick.getX() <= minX) {
                direction = -direction;
                for (auto &brick : bricks)
                    brick.setY(brick.getY() + 1);
            }

            if (ball.getX() <= brick.getX() ||
                ball.getX() >= brick.getX() + brick.getW() ||
                ball.getY() <= brick.getY() ||
                ball.getY() >= brick.getY() + brick.getH())
                continue;

            if (brick.getY() + brick.getH() > canvas["height"].as<int>())
                return false;

            paddle.setScore(paddle.getScore() + brick.getScore());
            ball.setDY(-ball.getDY());
            brick.markDead();
        }

        return blocksLeft;
    }
};

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::class_<Ball>("Ball")
    .constructor<int, int, int, int, int>()
    .property("x", &Ball::getX, &Ball::setX)
    .property("y", &Ball::getY, &Ball::setY)
    .property("r", &Ball::getR)
    .property("dx", &Ball::getDX, &Ball::setDX)
    .property("dy", &Ball::getDY, &Ball::setDY)
    .function("move", &Ball::move);

    emscripten::class_<Paddle>("Paddle")
    .constructor<int, int, int, int>()
    .property("x", &Paddle::getX, &Paddle::setX)
    .property("y", &Paddle::getY, &Paddle::setY)
    .property("w", &Paddle::getW)
    .property("h", &Paddle::getH)
    .property("score", &Paddle::getScore)
    .function("move", &Paddle::move);

    emscripten::class_<Brick>("Brick")
    .constructor<int, int, int, int, int>()
    .property("x", &Brick::getX)
    .property("y", &Brick::getY)
    .property("w", &Brick::getW)
    .property("h", &Brick::getH)
    .property("alive", &Brick::isAlive)
    .property("score", &Brick::getScore)
    .function("markDead", &Brick::markDead);

    emscripten::class_<BrickLayout>("BrickLayout")
    .constructor<int, int, int, int, int, int, int>()
    .function("get", &BrickLayout::get)
    .function("size", &BrickLayout::size)
    .function("collisionDetection", &BrickLayout::collisionDetection);
}