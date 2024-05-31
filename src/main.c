#include <raylib.h>
#include <malloc.h>

typedef struct {
    int x;
    int y;
    bool enabled;
} Light;

typedef struct {
    Light red;
    Light yellow;
    Light green;
} Lamp;

void init(Texture2D *background, Lamp **lamps, int lamps_size);
void render_lamps(Lamp *lamps, int lamps_size, float light_radius);
void render_button(Rectangle button, float disabled_x, float enabled_x, float y, int font_size, Color color, bool status, char *disabled, char *enabled);
void on_button(Vector2 mouse_position, Rectangle button, Color *color, bool *status);
void offline_pulse(bool status, Lamp *lamps, int lamps_size);

int main(void) {
    Texture2D background;
    Lamp *lamps;

    int lamps_size = 8;
    float light_radius = 8;

    init(&background, &lamps, lamps_size);
    Rectangle button = (Rectangle) { 4, 4, 100, 30 };
    Color button_color = DARKGRAY;

    char *disabled = "Disabled";
    char *enabled = "Enabled";
    int font_size = 20;

    int disabled_width = MeasureText(disabled, font_size);
    int enabled_width = MeasureText(enabled, font_size);

    float disabled_x = button.x + (button.width - (float) disabled_width) / 2;
    float enabled_x = button.x + (button.width - (float) enabled_width) / 2;
    float y = button.y + (button.height - (float) font_size) / 2;

    float timer = 0.0f;

    bool status = false;
    while (!WindowShouldClose()) {
        timer += GetFrameTime();
        Vector2 mouse_position = GetMousePosition();

        if (timer >= 0.7f) {
            offline_pulse(status, lamps, lamps_size);
            timer = 0.0f;
        }

        on_button(mouse_position, button, &button_color, &status);
        BeginDrawing(); {
            DrawTexture(background, 0, 0, WHITE);
            render_lamps(lamps, lamps_size, light_radius);
            render_button(button, disabled_x, enabled_x, y, font_size, button_color, status, disabled, enabled);
        } EndDrawing();
    }
    return 0;
}

void offline_pulse(bool status, Lamp *lamps, int lamps_size) {
    if (status) return;
    for (int i = 0; i < lamps_size; i++) {
        lamps[i].yellow.enabled = !lamps[i].yellow.enabled;
    }
}

void on_button(Vector2 mouse_position, Rectangle button, Color *color, bool *status) {
    if (CheckCollisionPointRec(mouse_position, button)) {
        *color = BLACK;
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
        *status = !(*status);
    } else {
        *color = DARKGRAY;
    }
}

void render_button(Rectangle button, float disabled_x, float enabled_x, float y, int font_size, Color color, bool status, char *disabled, char *enabled) {
    DrawRectangleLinesEx(button, 3, color);
    if (status) {
        DrawText(enabled, (int) enabled_x, (int) y, font_size, color);
    } else {
        DrawText(disabled, (int) disabled_x, (int) y, font_size, color);
    }
}

void render_lamps(Lamp *lamps, int lamps_size, float light_radius) {
    for (int i = 0; i < lamps_size; i++) {
        Lamp lamp = lamps[i];
        if (lamp.red.enabled) {
            DrawCircle(lamp.red.x, lamp.red.y, light_radius, RED);
        }
        if (lamp.yellow.enabled) {
            DrawCircle(lamp.yellow.x, lamp.yellow.y, light_radius, GOLD);
        }
        if (lamp.green.enabled) {
            DrawCircle(lamp.green.x, lamp.green.y, light_radius, GREEN);
        }
    }
}

void init(Texture2D *background, Lamp **lamps, int lamps_size) {
    // initialize window
    InitWindow(800, 450, "public static void main(String[] args)");
    SetTargetFPS(144);

    // load background texture
    *background = LoadTexture("../assets/plc.png");

    // allocate and fill lamps pointer
    *lamps = malloc(sizeof(Lamp) * lamps_size);
    (*lamps)[0] = (Lamp) { {462, 322, false}, {462, 340, false}, {462, 358, false} };
    (*lamps)[1] = (Lamp) { {377, 324, false}, {377, 342, false}, {377, 360, false} };
    (*lamps)[2] = (Lamp) { {305, 289, false}, {305, 307, false}, {305, 325, false} };
    (*lamps)[3] = (Lamp) { {305, 199, false}, {305, 217, false}, {305, 235, false} };
    (*lamps)[4] = (Lamp) { {306, 97, false}, {306, 115, false}, {306, 133, false} };
    (*lamps)[5] = (Lamp) { {373, 97, false}, {373, 115, false}, {373, 133, false} };
    (*lamps)[6] = (Lamp) { {499, 116, false}, {499, 134, false}, {499, 152, false} };
    (*lamps)[7] = (Lamp) { {499, 209, false}, {499, 227, false}, {499, 245, false} };
}
