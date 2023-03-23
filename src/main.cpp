#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

typedef unsigned int Tag;
#define TAG_MODEL    (2<<0)
#define TAG_PLAYER   (2<<1)
#define TAG_SOLID    (2<<2)
#define TAG_LIGHT    (2<<3)

#define MAX_MODELS      100 

struct GameObject
{
    Tag tags;

    // TAG_MODEL
    Model model;
    Vector3 scale           = Vector3One();
    Vector3 position        = Vector3Zero();
    Vector3 rotationAxis    = Vector3Zero();
    float rotationAngle     = 0.f;

    Color tint         = WHITE;

    GameObject(Vector3 position, Tag tags)
    {
        this->tags = tags;
        this->position = position;
    }

    inline bool HasTag(Tag tag)
    {
        return (tags & tag) == tag;
    }

    void UpdateAndDraw(float delta)
    {
        if (HasTag(TAG_MODEL))
        {
            DrawModelEx(model, position, rotationAxis, rotationAngle, scale, tint);
        }
    }

    void Unload(){
        if (HasTag(TAG_MODEL))
        {
            UnloadModel(model);
        }
    }
};

static std::vector<GameObject> GameObjects;

static void AddObject(GameObject object)
{
    GameObjects.push_back(object);
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - models loading");
    SetWindowState(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE);
    // DisableCursor();

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 50.0f, 50.0f, 50.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };     // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 80.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Place test model
    GameObject model = GameObject(Vector3Zero(),TAG_MODEL);
    model.model = LoadModel("assets/scene.gltf");
    model.rotationAxis = { 1.f, 0.f, 0.f };
    model.rotationAngle = 270;
    model.scale = { 0.3f, 0.3f, 0.3f };
    AddObject(model);

    TraceLog(LOG_INFO, "meshcount %d", model.model.meshCount);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Select model on mouse click
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode3D(camera);

                // draw all models
                float delta = GetFrameTime();
                for (auto& obj : GameObjects)
                {
                    obj.UpdateAndDraw(delta);
                }

                DrawGrid(20, 1.f);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (auto& obj : GameObjects)
    {
        obj.Unload();
    }

    CloseWindow();              // Close window and OpenGL context

    return 0;
}
