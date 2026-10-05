/**

* Author: Ren Liao
* Assignment: simple 2D Animation
* Date due: [10/05/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.

**/

#include "raylib.h"
#include <iostream>
#include <math.h>

using namespace std;
// Enums
enum AppStatus { TERMINATED, RUNNING };
enum KrackoTypes {ONE, TWO, THREE, FOUR, FIVE};
enum KirbyFly {UP, DOWN};

// Global Constants
constexpr int SCREEN_WIDTH  = 800 * 1.5f,
              SCREEN_HEIGHT = 450 * 1.5f,
              FPS           = 60,
              SIZE          = 100,
              OFFSET_FROM_BORDER = 100,
              KRACKO_FRAME_LIMIT = 500.0f,
              KIRBY_FLY_FRAME_LIMIT = 400.0f,
              COLOR_FRAME_LIMIT = 10000.0f,
              LIMIT_KRACKO_ANGLE = 20.0f;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { static_cast<float>(SIZE), static_cast<float>(SIZE) };
constexpr Vector2 BOTTOM_LEFT = {0.0f, SCREEN_HEIGHT };
constexpr Vector2 TOP_RIGHT = { SCREEN_WIDTH, 0.0f };
constexpr Vector2 TOP_LEFT = {0.0f, 0.0f};

// images found from spriters-resource.com - all characters from Kirby games (don't sue me please)
constexpr char KRACKO[] = "assets/kracko.png";
constexpr char KIRBY_FLY[] = "assets/kirby_fly.png";
constexpr char SKY_BACKGROUND[] = "assets/kirby_sky.png";
constexpr char STAR[] = "assets/kirby_star.png";
constexpr char EXPLOSION[] = "assets/kirby_explosion.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gScaleFactor;
float gAngle = 0.0f;
float gAngleStar = 0.0f;
Vector2 gPositionKirbyFly = BOTTOM_LEFT;
Vector2 gPositionOrigin = ORIGIN;
Vector2 gScale = BASE_SIZE;
Vector2 gPositionKracko = TOP_RIGHT;
Vector2 gPositionKrackoCircular = TOP_RIGHT;
Vector2 gPositionStar = ORIGIN;
Vector2 gPositionExplosion = ORIGIN;
Vector2 gScaleExplosion = BASE_SIZE;
Vector2 gPositionBackgroundLight = TOP_LEFT;
Vector2 gPositionBackgroundDark = TOP_RIGHT;

float gPreviousTicks = 0.0f;
float gOrbitAngle = 0.0f;
float gSpeed = 1000.0f;
float gScaleSpeed = 500.0f;
float gOrbitSpeed = 1000.0f;
int krackoFrames = 0;
int kirbyFlyFrames = 0;
int colorFrames = 0;

bool isKrackoCircular = false;
bool isFightTime = false;
bool isKillTime = false;

Texture2D gKrackoTexture;
Texture2D gKirbyFlyTexture;
Texture2D gBackgroundTexture;
Texture2D gStarTexture;
Texture2D gExplosionTexture;

KrackoTypes kracko;
KirbyFly kirbyFly;
Color backgroundTint = WHITE;

// Function Declarations
void initialize();
void processInpit();
void update();
void render();
void shutdown();

// Function Definitions
void initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Kirby and Kracko Fight Animation");

    gKrackoTexture = LoadTexture(KRACKO);
    gKirbyFlyTexture = LoadTexture(KIRBY_FLY);
    gBackgroundTexture = LoadTexture(SKY_BACKGROUND);
    gStarTexture = LoadTexture(STAR);
    gExplosionTexture = LoadTexture(EXPLOSION);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    //changes background tint every 10000 frames
    colorFrames++;
    if (colorFrames > COLOR_FRAME_LIMIT) {
        backgroundTint = DARKGRAY;
        if (colorFrames > COLOR_FRAME_LIMIT * 2.0f) colorFrames = 0;
    }else {
        backgroundTint = LIGHTGRAY;
    }

    //moves kracko diagonally at the beginning of the animation
    if (gPositionKracko.y < SCREEN_HEIGHT / 2.0f - 150.0f) {
        gPositionKracko.y += 0.03f * gSpeed * deltaTime;
    }
    if (gPositionKracko.x > SCREEN_WIDTH / 2.0f + 100.0f) gPositionKracko.x -= 0.08f * gSpeed *deltaTime;
    else {
         if (!isKrackoCircular) { //changes Kracko to move circularly
             isKrackoCircular = true;
             gPositionKrackoCircular = gPositionKracko;
         }
   }

    //make Kirby fly up diagonally
    if (gPositionKirbyFly.y > 2 * OFFSET_FROM_BORDER) gPositionKirbyFly.y -= 0.08f * gSpeed * deltaTime;
    if (gPositionKirbyFly.x < SCREEN_WIDTH / 2.0f - 150.0) gPositionKirbyFly.x += 0.08f * gSpeed * deltaTime;
    else {
        if (!isFightTime) { //once Kirby is in place, commence fight
            isFightTime = true;
        }
    }

    //makes Kirby flap his arms up and down while flying
    kirbyFlyFrames ++;
    if (kirbyFlyFrames == KIRBY_FLY_FRAME_LIMIT) {
        if (kirbyFly == UP) {
            kirbyFly = DOWN;
        }
        else if (kirbyFly == DOWN) {
            kirbyFly = UP;
        }
        kirbyFlyFrames = 0;
    }

    gAngleStar = fmod(gAngleStar + 0.05f * gOrbitSpeed * deltaTime, 360.0f); //changes the star's angle so it rotates

    if (!isFightTime) {
        gPositionStar = gPositionKirbyFly; //star will follow kirby while he flies until it is fight time
    }
    else if (isFightTime) {
        if (!isKillTime) { //while fighting, but not yet kill time, move the star towards Kracko
            gPositionStar.x += 0.05f * gSpeed * deltaTime;

            if (gPositionStar.x >= gPositionKracko.x + 100.0f) {
                isKillTime = true;
                gPositionExplosion.x = gPositionStar.x;
                gPositionExplosion.y = gPositionStar.y;
            }
        }
    }

    if (isKillTime) { //kill time! (yay) make explosion grow and defeat the evil Kracko
        if (gScaleExplosion.x < 600.0f) {
            gScaleExplosion.x += 0.10f * gScaleSpeed * deltaTime;
            gPositionExplosion.x -= 0.06f * gScaleSpeed * deltaTime;
        }

        if (gScaleExplosion.y < 600.0f) {
            gScaleExplosion.y += 0.10f * gScaleSpeed * deltaTime;
            gPositionExplosion.y -= 0.045f * gScaleSpeed * deltaTime;
        }
    }

    if (isKrackoCircular) { //kracko moves in orbit motion
        gOrbitAngle = fmod((gOrbitAngle + 0.001f * gOrbitSpeed * deltaTime), 360.0f);
        float radius = 25.0f;
        gPositionKracko = {
            cosf(gOrbitAngle) * radius + gPositionKrackoCircular.x,
            sinf(gOrbitAngle) * radius + gPositionKrackoCircular.y
        };
    }

    //changes kracko frames so that its lightning moves in a cool way
    krackoFrames++;
    if (krackoFrames == KRACKO_FRAME_LIMIT) {
        if (kracko == ONE) kracko = TWO;
        else if (kracko == TWO) kracko = THREE;
        else if (kracko == THREE) kracko = FOUR;
        else if (kracko == FOUR) kracko = FIVE;
        else if (kracko == FIVE) kracko = ONE;
        krackoFrames = 0;
    }
}

void render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    //makes the object's origin at its origin
    Vector2 objectOrigin = {gScale.x / 2.0f, gScale.y / 2.0f};

    //background drawing
    Rectangle textureAreaBackground = {
        0.0f, 0.0f,
        static_cast<float>(gBackgroundTexture.width),
        static_cast<float>(gBackgroundTexture.height),
    };
    Rectangle destinationAreaBackground = {
        0.0f, 0.0f,
        gScale.x*15.0f, gScale.y*7.0f
    };

    DrawTexturePro(gBackgroundTexture, textureAreaBackground,
        destinationAreaBackground, objectOrigin, gAngle, backgroundTint);

    //kracko drawing
    float krackoX = 0.0f, krackoY = 0.0f,
            krackoWidth = static_cast<float>(gKrackoTexture.width) / 5.0f,
            krackoOffset = 10.0f;
    if(kracko == ONE){
        krackoX = 0.0f + krackoOffset;
    }else if(kracko == TWO){
        krackoX = krackoWidth + krackoOffset;
    }else if (kracko == THREE){
        krackoX = krackoWidth * 2 + krackoOffset;

    }else if (kracko == FOUR){
        krackoX= krackoWidth * 3 + krackoOffset;
    }
    else if (kracko == FIVE) {
        krackoX = krackoWidth * 4 + krackoOffset;
    }

    Rectangle textureAreaKracko = {
        krackoX, krackoY,

        krackoWidth,
        static_cast<float>(gKrackoTexture.height)
    };

    Rectangle destinationAreaKracko = {
        gPositionKracko.x, gPositionKracko.y,
        gScale.x + 200, gScale.y + 200
    };

    DrawTexturePro(gKrackoTexture, textureAreaKracko,
        destinationAreaKracko, objectOrigin, gAngle, WHITE);

    //kirby drawing
    float kirbyX = 0.0f, kirbyY = 0.0f;

    if (kirbyFly == UP) kirbyX = 0.0f;
    if (kirbyFly == DOWN) kirbyX = static_cast<float>(gKirbyFlyTexture.width) / 2.0f;
    Rectangle textureAreaKirbyFly = {
        kirbyX, kirbyY,
        static_cast<float>(gKirbyFlyTexture.width) / 2.0f,
        static_cast<float>(gKirbyFlyTexture.height)
    };
    Rectangle destinationAreaKirbyFly = {
        gPositionKirbyFly.x, gPositionKirbyFly.y - OFFSET_FROM_BORDER,
        gScale.x, gScale.y
    };

    DrawTexturePro(gKirbyFlyTexture, textureAreaKirbyFly,
        destinationAreaKirbyFly, objectOrigin, gAngle, WHITE);

    //draw star
    Rectangle textureAreaStar = {
        0.0f, 0.0f,
         static_cast<float>(gStarTexture.width),
         static_cast<float>(gStarTexture.height)
     };

    Rectangle destinationAreaStar = {
       gPositionStar.x, gPositionStar.y,
        gScale.x, gScale.y
    };
    DrawTexturePro(gStarTexture, textureAreaStar,
        destinationAreaStar, objectOrigin, gAngleStar, WHITE);

    if (isKillTime) {
        //draw explosion
        Rectangle textureAreaExplosion = {
            0.0f, 0.0f,
            static_cast<float>(gExplosionTexture.width),
            static_cast<float>(gExplosionTexture.height)
        };
        Rectangle destinationAreaExplosion = {
            gPositionExplosion.x, gPositionExplosion.y,
            gScaleExplosion.x, gScaleExplosion.y
        };
        DrawTexturePro(gExplosionTexture, textureAreaExplosion,
            destinationAreaExplosion, objectOrigin, gAngle, WHITE);
    }

    EndDrawing();
}

void shutdown() {
    UnloadTexture(gKirbyFlyTexture);
    UnloadTexture(gStarTexture);
    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gKrackoTexture);
    UnloadTexture(gExplosionTexture);
    CloseWindow();
}

int main() {
    initialize();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}