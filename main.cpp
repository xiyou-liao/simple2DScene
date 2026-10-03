#include "raylib.h"
#include <iostream>
using namespace std;
// Enums
enum AppStatus { TERMINATED, RUNNING };
enum KrackoTypes {ONE, TWO, THREE, FOUR, FIVE};
// Global Constants
constexpr int SCREEN_WIDTH  = 800 * 1.5f,
              SCREEN_HEIGHT = 450 * 1.5f,
              FPS           = 60,
              SIZE          = 100,
              OFFSET_FROM_BOTTOM = 100;
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { static_cast<float>(SIZE), static_cast<float>(SIZE) };
constexpr Vector2 BOTTOM_LEFT = {0.0f, SCREEN_HEIGHT };
constexpr Vector2 TOP_LEFT = { 0.0f, 0.0f };
// images found from spriters-resource.com - all characters from Kirby (don't sue me please)
constexpr char KIRBY_STANDING[] = "assets/kirby_standing.png";
constexpr char KRACKO[] = "assets/kracko.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gScaleFactor;
float gAngle = 0.0f;
Vector2 gPositionBottomLeft = BOTTOM_LEFT;
Vector2 gPositionOrigin = ORIGIN;
Vector2 gScale = BASE_SIZE;
Vector2 gPositionTopLeft = TOP_LEFT;







Texture2D gKirbyStandingTexture;
Texture2D gKrackoTexture;
KrackoTypes kracko;

// Function Declarations
void initialize();
void processInpit();
void update();
void render();
void shutdown();

// Function Definitions
void initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "simpleKirbyAnimation");

    gKirbyStandingTexture = LoadTexture(KIRBY_STANDING);
    gKrackoTexture = LoadTexture(KRACKO);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {

}

void render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    Rectangle textureAreaKirby = {
        0.0f, 0.0f,
        static_cast<float>(gKirbyStandingTexture.width),
        static_cast<float>(gKirbyStandingTexture.height)
    };

    Rectangle destinationAreaKirby = {
        gPositionBottomLeft.x + OFFSET_FROM_BOTTOM, gPositionBottomLeft.y - OFFSET_FROM_BOTTOM,
        gScale.x, gScale.y
    };

    Vector2 objectOrigin = {gScale.x / 2.0f, gScale.y / 2.0f};

    DrawTexturePro(gKirbyStandingTexture, textureAreaKirby,
        destinationAreaKirby, objectOrigin, gAngle, WHITE);

    float krackoX = 0.0f, krackoY = 0.0f,
            krackoWidth = gKrackoTexture.width/5;

    if(kracko == ONE){
        krackoX = 0.0f;
    }else if(kracko == TWO){
        krackoX =   krackoWidth;
    }else if (kracko == THREE){
        krackoX = krackoWidth * 2;

    }else if (kracko == FOUR){
        krackoX= krackoWidth * 3;
    }
    else if (kracko == FIVE) {
        krackoX = krackoWidth * 4;
    }

    Rectangle textureAreaKracko = {
        krackoX, krackoY,

        krackoWidth,
        static_cast<float>(gKrackoTexture.height)
    };

    Rectangle destinationAreaKracko = {
        gPositionTopLeft.x, gPositionTopLeft.y,
        gScale.x + 200, gScale.y + 200
    };

    DrawTexturePro(gKrackoTexture, textureAreaKracko,
        destinationAreaKracko, objectOrigin, gAngle, WHITE);


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