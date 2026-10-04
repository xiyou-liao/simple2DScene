/**

* Author: Ren Liao

* Assignment: Pong Clone

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
              OFFSET_FROM_BORDER= 100,
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
//constexpr char KIRBY_STANDING[] = "assets/kirby_standing.png";
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
//Vector2 gPositionKirbyFly = ORIGIN;
Vector2 gPositionKrackoCircular = TOP_RIGHT;
Vector2 gPositionStar = ORIGIN;
Vector2 gPositionExplosion = ORIGIN;
Vector2 gScaleExplosion = BASE_SIZE;
Vector2 gPositionBackgroundLight = TOP_LEFT;
Vector2 gPositionBackgroundDark = TOP_RIGHT;
//Direction gDirection = BOTTOM;

float gPreviousTicks = 0.0f;
int krackoFrames = 0;
int kirbyFlyFrames = 0;
int colorFrames = 0;
//bool isKirbyWalk = true;
//bool isKirbyFly = false;
bool isKrackoCircular = false;
bool isFightTime = false;
bool isKillTime = false;

float gOrbitAngle = 0.0f;


//Texture2D gKirbyStandingTexture;
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "simpleKirbyAnimation");

 //   gKirbyStandingTexture = LoadTexture(KIRBY_STANDING);
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
    float ticks = GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    colorFrames++;
    if (colorFrames > COLOR_FRAME_LIMIT) {
        backgroundTint = DARKGRAY;
        if (colorFrames > COLOR_FRAME_LIMIT * 2.0f) colorFrames = 0;
    }else {
        backgroundTint = LIGHTGRAY;
    }

    if (gPositionKracko.y < SCREEN_HEIGHT / 2.0f - 150.0f) {
        gPositionKracko.y += 50.0f*deltaTime;
    }
    if (gPositionKracko.x > SCREEN_WIDTH / 2.0f + 100.0f) gPositionKracko.x -= 80.0f*deltaTime;
       else {
           //gOrbitAngle = fmod((gOrbitAngle + 0.8f), 360.0f);
             if (!isKrackoCircular) {
                 isKrackoCircular = true;
                 gPositionKrackoCircular = gPositionKracko;
             }

       }
        // if (gPositionKirby.x < 150.0f) { //start flying after walking for a bit
        //     gPositionKirby.x += 90.0f * deltaTime;
        //    // float oppositeKirby = SCREEN_WIDTH - gPositionKirby.x;
        //    // gPositionKracko.x = oppositeKirby -= 0.05f ; //kracko follows kirby
        // }else {
        //     if (isKirbyWalk == true) isKirbyWalk = false;
        //     if (!isKirbyFly) {
        //      //   cout << "should only occur one" << endl;
        //         isKirbyFly = true;
        //         gPositionKirbyFly.x = gPositionKirby.x;
        //         gPositionKirbyFly.y = gPositionKirby.y;
        //     }
        //
        // }


  //  if (isKirbyFly) {
       // cout << "previous y: " << gPositionKirbyFly.y << endl;
        if (gPositionKirbyFly.y > 2 * OFFSET_FROM_BORDER) gPositionKirbyFly.y -= 80.0f * deltaTime;
        if (gPositionKirbyFly.x < SCREEN_WIDTH / 2.0f - 150.0) gPositionKirbyFly.x += 80.0f * deltaTime;
        else {
            if (!isFightTime) {
                isFightTime = true;
            }
        }

       // cout << gPositionKirbyFly.y << endl;
        kirbyFlyFrames ++;
        if (kirbyFlyFrames == KIRBY_FLY_FRAME_LIMIT) {
            if (kirbyFly == UP) {
                kirbyFly = DOWN;
              //  cout << "changing kirby fly up to down" << endl;
            }
            else if (kirbyFly == DOWN) {
                kirbyFly = UP;
               // cout << "changing kirby fly down to up" << endl;
            }
            kirbyFlyFrames = 0;
        }
  //  }
    //gAngleStar = fmod((gAngleStar + 50.0f)*deltaTime, 360.0f);
    gAngleStar = fmod(gAngleStar + 0.05f, 360.0f);

    if (!isFightTime) {
        gPositionStar = gPositionKirbyFly;
    }
    else if (isFightTime) {
        if (!isKillTime) {
            gPositionStar.x+=50.0f*deltaTime;
           // gPositionStar.y-=5.0f*deltaTime;

            if (gPositionStar.x >= gPositionKracko.x + 100.0f) {
                isKillTime = true;
                gPositionExplosion.x = gPositionStar.x;
                gPositionExplosion.y = gPositionStar.y;
            }
        }
    }
    if (isKillTime) {
        if (gScaleExplosion.x < 500.0f) {
            gScaleExplosion.x += 0.10f;
            gPositionExplosion.x -= 60.0f * deltaTime;
        }


        if (gScaleExplosion.y < 500.0f) {
            gScaleExplosion.y += 0.10f;
            gPositionExplosion.y -= 45.0f *deltaTime;
        }
    }

    if (isKrackoCircular) {
        gOrbitAngle = fmod((gOrbitAngle + 0.001f), 360.0f);
        float radius = 25.0f;
        gPositionKracko = {
            cosf(gOrbitAngle)*radius + gPositionKrackoCircular.x,
            sinf(gOrbitAngle)*radius + gPositionKrackoCircular.y
        };

    }

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

    Vector2 objectOrigin = {gScale.x / 2.0f, gScale.y / 2.0f};
 //   Vector2 backgroundOrigin = {gScale.x * 100.0f, gScale.y * 100.0f};
    Rectangle textureAreaBackground = {
        0.0f, 0.0f,
        static_cast<float>(gBackgroundTexture.width),
        static_cast<float>(gBackgroundTexture.height),
    };
    Rectangle destinationAreaBackground = {
        0.0f, 0.0f,
        gScale.x*15.0f, gScale.y*7.0f
    };

    DrawTexturePro(gBackgroundTexture, textureAreaBackground, destinationAreaBackground, objectOrigin, gAngle, backgroundTint);
   // DrawTexturePro(gBackgroundTexture, textureAreaBackground, destinationAreaBackgroundDark, objectOrigin, gAngle, DARKGRAY);
    // if (isKirbyWalk){
    //     Rectangle textureAreaKirby = {
    //         0.0f, 0.0f,
    //         static_cast<float>(gKirbyStandingTexture.width),
    //         static_cast<float>(gKirbyStandingTexture.height)
    //     };
    //
    // Rectangle destinationAreaKirby = {
    //     gPositionKirby.x + OFFSET_FROM_BORDER, gPositionKirby.y - OFFSET_FROM_BORDER,
    //     gScale.x, gScale.y
    // };
    //
    // DrawTexturePro(gKirbyStandingTexture, textureAreaKirby,
    //     destinationAreaKirby, objectOrigin, gAngle, WHITE);
    //
    // }

    float krackoX = 0.0f, krackoY = 0.0f,
            krackoWidth = static_cast<float>(gKrackoTexture.width) / 5.0f,
            krackoOffset = 13.0f;
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


  //  if (isKirbyFly) {
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

        Rectangle textureAreaStar = {
            0.0f, 0.0f,
             static_cast<float>(gStarTexture.width),
             static_cast<float>(gStarTexture.height)
         };

        Rectangle destinationAreaStar = {
           gPositionStar.x, gPositionStar.y,
            gScale.x, gScale.y
        };
        DrawTexturePro(gStarTexture, textureAreaStar, destinationAreaStar, objectOrigin, gAngleStar, WHITE);
   // }
    if (isKillTime) {
        Rectangle textureAreaExplosion = {
            0.0f, 0.0f,
            static_cast<float>(gExplosionTexture.width), static_cast<float>(gExplosionTexture.height)
        };
        Rectangle destinationAreaExplosion = {
            gPositionExplosion.x, gPositionExplosion.y,
            gScaleExplosion.x, gScaleExplosion.y
        };
        DrawTexturePro(gExplosionTexture, textureAreaExplosion, destinationAreaExplosion, objectOrigin, gAngle, WHITE);
    }


    EndDrawing();
}

void shutdown() {
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