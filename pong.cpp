/***********************************************************************
 * Program:
 *    Project
 * Author:
 *    Drey Platt
 *    Thomas Nichols
 *    Kyle Stewart
 * Summary:
 *    OK, I couldn't help myself.  The first video game is "Tennis for Two"
 *    developed in 1958 (you read that correctly) by William Higinbotham
 *    as a demo for the computer systems in the Brookhaven National Laboratory.
 *    The display was a normal oscilloscope.  Ours will use slighly more
 *    advanced graphics: OpenGL
 ***********************************************************************/

#include "point.h"       // the ball has a position
#include "uiInteract.h"  // interface with OpenGL
#include "uiDraw.h"      // all the draw primitives exist here
#include <math.h>  //for the sine and cosine values
#include "bullets.h"
#include <vector>
#include <stdlib.h>
#include <math.h>



#define PADDLE 25
#define PI 3.14159265 //to convert to radians from degrees
float currentTime = 0.00;
// set the bounds of the game
float Point::xMin = -128.0;
float Point::xMax =  128.0;
float Point::yMin = -128.0;
float Point::yMax =  128.0;
bool isfinishedStrike = true;
bool isfinishedAdvance = true;
bool anotherBird = true;
float randBirdTime;
int bulletDiameter = 8;
int birdDiameter = 20;
float angle = 135;
float timeTilNextBird;
float ballVelocity = 10;
int score = 0;
bool leftIsPressed;
bool rightIsPressed;
int timePressedRight = 0;
int timePressedLeft = 0;


struct Bird
{
    Point birdPoint;
    float dx;     // horizontal velocity of the ball
    float dy;     // vertical velocity of the ball
    bool alive = true;
};

struct Bullet
{
    Point bulletPoint;
    float dx;     // horizontal velocity of the ball
    float dy;     // vertical velocity of the ball
    bool alive = true;
};

std::vector <Bullet> theBullets;
std::vector <Bird> theBirds;

/*****************************************
 * Skeet
 * The main skeet structure
 ****************************************/
class Skeet
{
public:
    Skeet();                          // set up the game
    void advanceBall();              // advance the game by one move
    void timeCheck();                // check to see if bird can go
    void newBirds();                 // create a new bird
    void draw();                     // draw everything
    void move(int up, int down);     // move everything
    void shoot(int shoot);           // Shoot a ball out
    void strike();                   // did we hit something?
private:
    bool dead;    // is the game over?
    int score;    // current score.. how many times did we hit the ball?
    Point paddle; // location of the paddle
    
};

/***************************************************
 * Skeet :: CONSTRUCTOR
 * Give the bullet a set velocity and set
 * the positions of everything else.
 ***************************************************/
Skeet::Skeet() :  dead(false), score(0), paddle()
{
    // put the paddle where it belongs
    paddle.setX(paddle.getXMax());
    paddle.setY(paddle.getYMin());
    //create a vector of bullets
   
};

/********************************************
 * Skeet :: ADVANCE
 * Move the ball forward by some amount
 *******************************************/
void Skeet::advanceBall()
{;
    // gravity.. just for fun!
    //dy -= .2;
    int theBirdSize = (int)theBirds.size();
    int theBulletSize = (int)theBullets.size();
    for (int i = 0; i < theBirdSize; i++){
        //std::cout << theBirds[i].alive;
        theBirds[i].birdPoint.setX(theBirds[i].birdPoint.getX() + theBirds[i].dx);
        theBirds[i].birdPoint.setY(theBirds[i].birdPoint.getY() + theBirds[i].dy);
        //if the birds is past the yMax or yMin or xMax or if not alive then delete it
        if  ((theBirds[i].birdPoint.getX() - birdDiameter) > theBirds[i].birdPoint.getXMax() || (theBirds[i].birdPoint.getY() - birdDiameter)> theBirds[i].birdPoint.getYMax() || (theBirds[i].birdPoint.getY() + birdDiameter)< theBirds[i].birdPoint.getYMin())
        {
            theBirds[i] = theBirds.back(); theBirds.pop_back(); //i--;
        }
    }
    //std::cout << std::endl;
//Draw all the bullets
    for (int i = 0; i < theBulletSize; i++)
    {
        theBullets[i].bulletPoint.setX(theBullets[i].bulletPoint.getX() - theBullets[i].dx);
        theBullets[i].bulletPoint.setY(theBullets[i].bulletPoint.getY() + theBullets[i].dy);
        //if bullet is outside of the screen then delete it
        //do this by moving the object to the back of the array and then use pop back
        //check if it passes the left most wall or the ceiling
        if (theBullets[i].bulletPoint.getX() < theBullets[i].bulletPoint.getXMin() || theBullets[i].bulletPoint.getY() > theBullets[i].bulletPoint.getYMax()){
               theBullets[i] = theBullets.back(); theBullets.pop_back(); //i--;
        }
    }
    isfinishedAdvance = true;
}

/****************************************
 * STRIKE
 * Strike the paddle.  The further from the center,
 * the more it hits
 ****************************************/
void Skeet::strike()
{
    
    // ball not at the paddle yet
    //loop through all the bullets in vector
    int theSize = (int)theBullets.size();
    int theBirdSize =(int)theBirds.size();
for (int i = 0; i < theSize; i++){
    float bulX = theBullets[i].bulletPoint.getX();
    float bulY = theBullets[i].bulletPoint.getY();
    for (int j = 0; j < theBirdSize; j++){
        float birdX = theBirds[j].birdPoint.getX();
        float birdY = theBirds[j].birdPoint.getY();
       //if the distance is closer than the tolerance level
        if((bulX-birdX < 30) || (bulY-birdY <30)){
        float disBetween = sqrtf((fabs(birdX - bulX)* fabs(birdX - bulX)) +(fabs(birdY - bulY)* fabs(birdY - bulY)));
      //  std::cout << "Dist: " << disBetween << << std::endl;
        if (disBetween < 24){
            //remove the bird and bullet
            theBullets[i] = theBullets.back(); theBullets.pop_back(); //i--;
            theBirds[j] = theBirds.back(); theBirds.pop_back(); //j--;
            score++;
            //theBirds[i].alive = false;
            //theBullets[j].alive = false;
        }
        }
    }
    
}
    //
    // yeah, we hit the ball
    //
    
    // hit.  Change the dx direction.
    //   dx *= -1.0;
    //   ball.setX(ball.getXMax() - 1.0);
    //
    //   // get some score
    //   score++;
    //
    //   // speed up every 5 points
    //   if (score % 5 == 0)
    //      dx += -1.0;
    //
    //   // the dy is changed by the angle.
    //   dy += distance / 5;
    isfinishedStrike=true;
}


/**************************************************
 * SKEET : SHOOT
 * Shoot the ball with the spacebar
 *************************************************/
void Pong::shoot(int space)
{
    //Should a new bullet ball be shot out?
    if(space && theBullets.size() < 5){

        //create a new bullet
        Bullet newBullet;
        //set that bullets velocity
        newBullet.dx = ballVelocity*(sin((angle-90)*PI/180));
        newBullet.dy = ballVelocity*(cos((angle-90)*PI/180));
        //give that new bullet its start location
        newBullet.bulletPoint.setX(paddle.getXMax());
        newBullet.bulletPoint.setY(paddle.getYMin());
        //add that bullet to the vector 'theBullets'
        theBullets.push_back(newBullet);
        //create a new bullet ball
        //give that bulet ball a start location

        //give it a velocity x and y based on current angle

//        Point uppoint(ball.getXMin() + 5, ball.getYMax() - 50);
//        drawNumber(uppoint, dy);
//        // draw the down
       //Point downpoint(5,  - 100);
       // drawNumber(downpoint, (int)theBullets.size());
    }
    
}

/**************************************************
 * PONG : TIMECHECK
 * Move the paddle
 *************************************************/
void Pong::timeCheck()
{
    if (timeTilNextBird<=currentTime){
        //set the random bird time
        randBirdTime = ((float) rand() / (RAND_MAX));
        //set current time as the last time a bird was released
        timeTilNextBird = currentTime + randBirdTime;
        anotherBird = true;
       // std::cout << "Bird";
    } else {
        anotherBird = false;
    }
}
/**************************************************
 * PONG : NEWBIRDS
 * create a new bird object
 *************************************************/
void Pong::newBirds()
{
float randXVelocity = ((float) rand() / (RAND_MAX));
float randYVelocity = ((float) rand() / (RAND_MAX));
float randYPos = ((float) rand() / (RAND_MAX));
    //create a new bullet
    Bird newBird;
    //give that new bird its start location
    newBird.birdPoint.setX(newBird.birdPoint.getXMin());
    float yPos = (newBird.birdPoint.getYMax() - newBird.birdPoint.getYMin()) * randYPos;
    
    //std::cout << "new Bird\n";
    float newYpos=newBird.birdPoint.getYMin() + yPos;
    newBird.birdPoint.setY(newYpos);
    //set that bullets velocity
    float birdXVel = (randXVelocity*3) + 3;
    float birdYVel = (randYVelocity*4);
    //if the bird is above the half way point makes it come down
    if (newYpos > 0){
        birdYVel = birdYVel * -1;
    }
    newBird.dx = birdXVel;
    newBird.dy = birdYVel;
//std::cout << "new Bird "<< birdXVel << ", " << birdYVel << "ypos: " << newYpos << "\n";
    //add that bird to the vector 'theBirds'
    theBirds.push_back(newBird);
}
/**************************************************
 * PONG : MOVE
 * Move the gun
 *************************************************/
void Pong::move(int up, int down)
{
    //booleans to check if the keys have been released

    
    if (up>0 && angle>100) {
        timePressedRight++;
        int multiplier = (timePressedRight % 5) + 1;
        angle=angle-(2 * multiplier);
    }
    if (down>0 && angle<170){
        timePressedLeft++;
        int multiplier = (timePressedLeft % 5) + 1;
        angle=angle+(2 * multiplier);
    }
    
    if (down==0){
        leftIsPressed = false;
        timePressedLeft = 0;
    }
    
    if (up==0){
        rightIsPressed = false;
        timePressedRight = 0;
    }
    // draw the up
    //    Point uppoint(ball.getXMin() + 5, ball.getYMax() - 50);
    //    drawNumber(uppoint, up);
    //    // draw the down
    //    Point downpoint(ball.getXMin() + 5, ball.getYMax() - 100);
    //    drawNumber(downpoint, down);
}
/*************************************************
 * PONG : DRAW
 * Draw the stuff
 ************************************************/
void Pong::draw()
{
    
    // draw the ball and the paddle
    drawRect(paddle, PADDLE * 2, 10, angle);
    // draw the ball
    //loop through all the bullets in the vector and draw them
    for (int i = 0; i < theBullets.size(); i++){
        if (theBullets[i].alive){
        drawCircle(theBullets[i].bulletPoint, bulletDiameter);
        //drawNumber(theBullets[i].bulletPoint, i);
        }
    }
    // draw the birds
    //loop through all the birds in the vector and draw them
    for (int j = 0; j < theBirds.size(); j++){
        if (theBirds[j].alive){
        drawCircle(theBirds[j].birdPoint, birdDiameter);
       // drawNumber(theBirds[j].birdPoint, j);
        }
      //  std::cout << theBirds[j].birdPoint;
    }
    Point scorePoint(paddle.getXMin() + 5, paddle.getYMax() - 5);
     drawNumber(scorePoint, score);
}

/*********************************************
 * CALLBACK
 * The main interaction loop of the engine.
 * This gets called from OpenGL.  It give us our
 * interface pointer (where we get our events from)
 * as well as a void pointer which we know is our
 * game class.
 *********************************************/
void callBack(const Interface *pUI, void *p)
{
    currentTime = currentTime +.032;
    // we know the void pointer is our game class so
    // cast it into the game class.
    Pong *pPong = (Pong *)p;
    pPong->shoot(pUI->isSpace());
    //only run if previous loop is finished
    if (isfinishedStrike){
        isfinishedStrike = false;
       pPong->strike();
    }
    // advance the ball
    if(isfinishedAdvance){
        isfinishedAdvance = false;
       pPong->advanceBall();
    }
   
    //std::cout << isfinishedAdvance << isfinishedStrike <<std::endl;
    // check the paddle
    pPong->move(pUI->isRight(), pUI->isLeft());
    pPong->draw();
    // did we hit the ball?
    
    //check if another bird can be realeased yet
    //get back boolean value
    pPong->timeCheck();
    
    if(anotherBird){
        //we can now initiate another bird and add it to the bird vector
        pPong->newBirds();
    }
    // draw it
   
}


/*********************************
 * MAIN
 * initialize the drawing window, initialize
 * the game,and run it!
 *********************************/
int main(int argc, char **argv)
{
    
    // Start the drawing
    Interface ui(argc, argv, "Skeet!");
    
    // play the game.  Our function callback will get called periodically
    //called 30 times per second
    Skeet skeet;
    
    ui.run(callBack, (void *)&skeet);
    
    return 0;
}
