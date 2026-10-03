#include "raylib.h"
#include <iostream>
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
              KIRBY_FLY_FRAME_LIMIT = 600.0f;
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { static_cast<float>(SIZE), static_cast<float>(SIZE) };
constexpr Vector2 BOTTOM_LEFT = {0.0f, SCREEN_HEIGHT };
constexpr Vector2 TOP_LEFT = { 0.0f, 0.0f };


// images found from spriters-resource.com - all characters from Kirby games (don't sue me please)
constexpr char KIRBY_STANDING[] = "assets/kirby_standing.png";
constexpr char KRACKO[] = "assets/kracko.png";
constexpr char KIRBY_FLY[] = "assets/kirby_fly.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gScaleFactor;
float gAngle = 0.0f;
Vector2 gPositionKirby = BOTTOM_LEFT;
Vector2 gPositionOrigin = ORIGIN;
Vector2 gScale = BASE_SIZE;
Vector2 gPositionKracko = TOP_LEFT;
Vector2 gPositionKirbyFly = ORIGIN;

float gPreviousTicks = 0.0f;
int krackoFrames = 0;
int kirbyFlyFrames = 0;

bool isKirbyWalk = true;
bool isKirbyFly = false;
bool isKrackoCircular = false;


Texture2D gKirbyStandingTexture;
Texture2D gKrackoTexture;
Texture2D gKirbyFlyTexture;

KrackoTypes kracko;
KirbyFly kirbyFly;

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
    gKirbyFlyTexture = LoadTexture(KIRBY_FLY);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (gPositionKracko.y < SCREEN_HEIGHT / 2.0f - 100.0f) {
        gPositionKracko.y += 0.05f;
    }else {
        if (gPositionKracko.x < SCREEN_WIDTH / 2.0f - 100.0f) gPositionKracko.x += 0.05f;
        else {
            if (!isKrackoCircular) {
                isKrackoCircular = true;
            }
        }
        if (gPositionKirby.x < SCREEN_WIDTH / 2.0f) { //once kirby gets to the middle of the screen, he starts flying
            gPositionKirby.x += 0.09f;
        }else {
            if (isKirbyWalk == true) isKirbyWalk = false;
            if (!isKirbyFly) {
             //   cout << "should only occur one" << endl;
                isKirbyFly = true;
                gPositionKirbyFly.x = gPositionKirby.x;
                gPositionKirbyFly.y = gPositionKirby.y;
            }

        }
    }

    if (isKirbyFly) {
       // cout << "previous y: " << gPositionKirbyFly.y << endl;
        if (gPositionKirbyFly.y > OFFSET_FROM_BORDER) gPositionKirbyFly.y -= 0.05f;
        if (gPositionKirbyFly.x < SCREEN_WIDTH - OFFSET_FROM_BORDER) gPositionKirbyFly.x += 0.05f;

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
    }

    if (isKrackoCircular) {

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

    if (isKirbyWalk){
        Rectangle textureAreaKirby = {
            0.0f, 0.0f,
            static_cast<float>(gKirbyStandingTexture.width),
            static_cast<float>(gKirbyStandingTexture.height)
        };

        Rectangle destinationAreaKirby = {
            gPositionKirby.x + OFFSET_FROM_BORDER, gPositionKirby.y - OFFSET_FROM_BORDER,
            gScale.x, gScale.y
        };

        DrawTexturePro(gKirbyStandingTexture, textureAreaKirby,
            destinationAreaKirby, objectOrigin, gAngle, WHITE);

    }

    if (isKirbyFly) {
        float kirbyX = 0.0f, kirbyY = 0.0f;

        if (kirbyFly == UP) kirbyX = 0.0f;
        if (kirbyFly == DOWN) kirbyX = static_cast<float>(gKirbyFlyTexture.width) / 2.0f;
        Rectangle textureAreaKirbyFly = {
            kirbyX, kirbyY,
            static_cast<float>(gKirbyFlyTexture.width / 2.0f),
            static_cast<float>(gKirbyFlyTexture.height)
        };
        Rectangle destinationAreaKirbyFly = {
            gPositionKirbyFly.x, gPositionKirbyFly.y - OFFSET_FROM_BOTTOM,
            gScale.x, gScale.y
        };

        DrawTexturePro(gKirbyFlyTexture, textureAreaKirbyFly,
            destinationAreaKirbyFly, objectOrigin, gAngle, WHITE);
    }
    float krackoX = 0.0f, krackoY = 0.0f,
            krackoWidth = static_cast<float>(gKrackoTexture.width) / 5.0f;

    if(kracko == ONE){
        krackoX = 0.0f;
    }else if(kracko == TWO){
        krackoX = krackoWidth;
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
        gPositionKracko.x, gPositionKracko.y,
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