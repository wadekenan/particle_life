// in terminal build with:
// cmake -B build && cmake --build build && ./build/game
// if project has been moved: 
// CTRL + SHIFT + P -> CMake: Delete cache and reconfig

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// particle structure
struct Particle {
    int type;
    float posX;
    float posY;
    float velX;
    float velY;
};

const int screenWidth = 1000;
const int screenHeight = 1000;
const int maxVel = 0;
const int maxR = 100;
const int numParticles = 2000;
const int particleTypes = 4;
const char pColorChars[4] = {'b', 'r', 'g', 'p'};
const int particleSize = 2;
const int forceFact = 5;
const int minRepulF = 2;
float dt = .0167;
float frictFact = .7;
struct Particle particles[numParticles];
const int randomOn = 1;

// attraction matrix
float aMatrix[4][4] = {
    //b. r.  g. p. 
    {.8, .2, 0, .6}, // b
    {0, .8, .2, .3}, // r
    {.2, 0, .8, 0}, // g
    {.9, .2, .3, -.3} // p
};

//float aMatrix[particleTypes][particleTypes];

void generateAttractMatrix()
{
    for (int i=0; i<particleTypes; i++)
    {
        for (int j=0; j<particleTypes; j++)
        {
            aMatrix[i][j] = (float)(rand()%100) / 100 * 2 - 1;
        }
    }
}

void drawAttractMatrix()
{
    DrawRectangle(0, 0, particleTypes*40, particleTypes*40, BLACK);
    for (int i=0; i<particleTypes; i++)
    {
        DrawText(TextFormat("%c", pColorChars[i]), 10, 30*(i+1), 10, WHITE);
        DrawText(TextFormat("%c", pColorChars[i]), 30*(i+1), 10, 10, WHITE);
        for (int j=0; j<particleTypes; j++)
        {
            DrawText(TextFormat("%.2f", aMatrix[i][j]), 30*(j+1), 30*(i+1), 10, WHITE);
        }
    }
}

void generateParticles()
{
    // each particle randomly placed
    for (int i=0; i<numParticles; i++)
    {
        struct Particle p;
        p.posX = rand() % screenWidth;
        p.posY = rand() % screenHeight;
        p.velX = (float)rand() / RAND_MAX * maxVel*2 - maxVel;
        p.velY = (float)rand() / RAND_MAX * maxVel*2 - maxVel;
        p.type = rand() % particleTypes;
        particles[i] = p;
    }
}

void updateParticles()
{
    // each particle checks all other particles to calculate a net force
    for (int i=0; i<numParticles; i++)
    {
        struct Particle p = particles[i];
        // force treated as accel since mass assumed to be 1
        float totFx = 0;
        float totFy = 0;

        for (int j=0; j<numParticles; j++)
        {
            if (j == i) continue;
            struct Particle b = particles[j];
            const float dX = b.posX-p.posX;
            const float dY = b.posY-p.posY;
            const float dis = hypot(dX, dY);
            float f = 0;
            // if in range act on particle
            if (dis<maxR && dis > 0)
            {
                float d_n = dis / maxR;
                float beta = .3;
                float a = aMatrix[p.type][b.type];

                // internal repulsive force
                if (d_n < beta) f = (minRepulF*d_n/beta) - minRepulF;

                // external matrix force applied
                else f = (1 - (fabsf(2*d_n-1-beta) / (1-beta))) * a;
                
                totFx += dX/dis * f;
                totFy += dY/dis * f;
            }
        }
        // scaling total forces
        totFx *= maxR * forceFact;
        totFy *= maxR * forceFact;

        // friction force applied
        p.velX *= frictFact;
        p.velY *= frictFact;

        // applied change in vel from force 
        p.velX += (totFx * dt);
        p.velY += (totFy * dt);

        // applied change in position from vel
        p.posX += p.velX * dt;
        p.posY += p.velY * dt;

        // wrap positions
        if (p.posX >= screenWidth) p.posX -= screenWidth;
        if (p.posX <= 0) p.posX += screenWidth;
        if (p.posY >= screenWidth) p.posY -= screenHeight;
        if (p.posY <= 0) p.posY += screenHeight;

        /*if (p.posX >= screenWidth || p.posX < 0) p.velX *= -1;
        if (p.posY >= screenHeight || p.posY < 0) p.velY *= -1;*/

        particles[i] = p;

    }
}

int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Particle Life");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    srand(time(NULL));
    if (randomOn == 1) generateAttractMatrix();
    generateParticles();

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //dt = GetFrameTime();
        //frictFact = pow(.5, dt / .04);

        updateParticles();

        // Draw
        BeginDrawing();

        ClearBackground(BLACK);

        // draw each particle
        for (int i=0; i<numParticles; i++)
        {
            struct Color color;
            if (particles[i].type == 0) color = BLUE;
            else if (particles[i].type == 1) color = RED;
            else if (particles[i].type == 2) color = GREEN;
            else color = PURPLE;
            DrawCircle(particles[i].posX, 
                particles[i].posY, particleSize, color);
        }

        drawAttractMatrix();


        EndDrawing();
        
    }
    
    CloseWindow(); // Close window and OpenGL context
    

    return 0;
}