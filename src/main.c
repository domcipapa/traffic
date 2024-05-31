#include <stdlib.h>
#include <raylib.h>
#include "../assets/plc.h"

typedef struct {
    int x;
    int y;
    bool enabled;
} Light;

typedef struct {
    Light red;
    Light yellow;
    Light green;
    char side;
} Lamp;

void init(Texture2D *background, Lamp **lamps, int lamps_size);
void render_lamps(Lamp *lamps, int lamps_size, float light_radius);
void render_button(Rectangle button, float disabled_x, float enabled_x, float y, int font_size, Color color, bool status, char *disabled, char *enabled);
void on_button(Vector2 mouse_position, Rectangle button, Color *color, bool *status, Lamp *lamps, int lamps_size, float *timer);
void disabled_pulse(bool status, Lamp *lamps, int lamps_size);
void enabled_logic(char *previous_side, Lamp **lamps, int lamps_size, bool status);

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

    float disabled_timer = 0.0f;
    float enabled_timer = 0.0f;
    char previous_side = 'd';

    float disabled_tick = 0.7f;
    float enabled_tick = 5.0f;

    bool status = false;
    while (!WindowShouldClose()) {
        Vector2 mouse_position = GetMousePosition();

        if (disabled_timer >= disabled_tick) {
            disabled_pulse(status, lamps, lamps_size);
            disabled_timer = 0.0f;
        }
        disabled_timer += GetFrameTime();

        if (status && enabled_tick - enabled_timer < 1.0f) {
            for (int i = 0; i < lamps_size; i++) {
                if (lamps[i].side == 'a' && previous_side == 'd') {
                    lamps[i].yellow.enabled = true;
                    lamps[i - 2].yellow.enabled = true;
                    lamps[i - 2].green.enabled = false;
                }
                if (lamps[i].side == 'b' && previous_side == 'a') {
                    lamps[i].yellow.enabled = true;
                    lamps[i - 2].yellow.enabled = true;
                    lamps[i - 2].green.enabled = false;
                }
                if (lamps[i].side == 'c' && previous_side == 'b') {
                    lamps[i].yellow.enabled = true;
                    lamps[i - 2].yellow.enabled = true;
                    lamps[i - 2].green.enabled = false;
                }
                if (lamps[i].side == 'd' && previous_side == 'c') {
                    lamps[i].yellow.enabled = true;
                    lamps[i - 2].yellow.enabled = true;
                    lamps[i - 2].green.enabled = false;
                }
            }
        }

        if (enabled_timer >= enabled_tick) {
            enabled_logic(&previous_side, &lamps, lamps_size, status);
            enabled_timer = 0.0f;
        }
        enabled_timer += GetFrameTime();

        on_button(mouse_position, button, &button_color, &status, lamps, lamps_size, &enabled_timer);
        BeginDrawing(); {
            DrawTexture(background, 0, 0, WHITE);
            render_lamps(lamps, lamps_size, light_radius);
            render_button(button, disabled_x, enabled_x, y, font_size, button_color, status, disabled, enabled);
            DrawText(TextFormat("Disabled remaining: %.2f\nEnabled remaining: %.2f", disabled_tick - disabled_timer, enabled_tick - enabled_timer), 120, 20, 13, YELLOW);
        } EndDrawing();
    }
    return 0;
}

char a_turn(Lamp *lamps, int lamps_size) {
    for (int i = 0; i < lamps_size; i++) {
        if (lamps[i].side == 'a') {
            lamps[i].red.enabled = false;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = true;
        } else {
            lamps[i].red.enabled = true;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = false;
        }
    }
    return 'a';
}

char b_turn(Lamp *lamps, int lamps_size) {
    for (int i = 0; i < lamps_size; i++) {
        if (lamps[i].side == 'b') {
            lamps[i].red.enabled = false;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = true;
        } else {
            lamps[i].red.enabled = true;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = false;
        }
    }
    return 'b';
}

char c_turn(Lamp *lamps, int lamps_size) {
    for (int i = 0; i < lamps_size; i++) {
        if (lamps[i].side == 'c') {
            lamps[i].red.enabled = false;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = true;
        } else {
            lamps[i].red.enabled = true;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = false;
        }
    }
    return 'c';
}

char d_turn(Lamp *lamps, int lamps_size) {
    for (int i = 0; i < lamps_size; i++) {
        if (lamps[i].side == 'd') {
            lamps[i].red.enabled = false;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = true;
        } else {
            lamps[i].red.enabled = true;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = false;
        }
    }
    return 'd';
}

void enabled_logic(char *previous_side, Lamp **lamps, int lamps_size, bool status) {
    if (!status) return;

    if (*previous_side == 'd') {
        *previous_side = a_turn(*lamps, lamps_size);
        return;
    }

    if (*previous_side == 'a') {
        *previous_side = b_turn(*lamps, lamps_size);
        return;
    }

    if (*previous_side == 'b') {
        *previous_side = c_turn(*lamps, lamps_size);
        return;
    }

    if (*previous_side == 'c') {
        *previous_side = d_turn(*lamps, lamps_size);
        return;
    }
}

void disabled_pulse(bool status, Lamp *lamps, int lamps_size) {
    if (status) return;
    for (int i = 0; i < lamps_size; i++) {
        lamps[i].yellow.enabled = !lamps[i].yellow.enabled;
    }
}

void on_button(Vector2 mouse_position, Rectangle button, Color *color, bool *status, Lamp *lamps, int lamps_size, float *timer) {
    if (CheckCollisionPointRec(mouse_position, button)) {
        *color = BLACK;
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

        // set timer to skip some time
        *timer = 5.0f;

        // reset all lights
        for (int i = 0; i < lamps_size; i++) {
            lamps[i].red.enabled = false;
            lamps[i].yellow.enabled = false;
            lamps[i].green.enabled = false;
        }

        // negate status
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
    InitWindow(800, 450, "SIMATIC WinCC Runtime Advanced");
    SetTargetFPS(144);

    // load background texture
    Image image = LoadImageFromMemory(".png", ASSETS_PLC_PNG, (int) ASSETS_PLC_PNG_LEN);
    *background = LoadTextureFromImage(image);

    // allocate and fill lamps pointer
    *lamps = malloc(sizeof(Lamp) * lamps_size);
    (*lamps)[0] = (Lamp) { {462, 322, false}, {462, 340, false}, {462, 358, false}, 'a' };
    (*lamps)[1] = (Lamp) { {377, 324, false}, {377, 342, false}, {377, 360, false}, 'a' };
    (*lamps)[2] = (Lamp) { {305, 289, false}, {305, 307, false}, {305, 325, false}, 'b' };
    (*lamps)[3] = (Lamp) { {305, 199, false}, {305, 217, false}, {305, 235, false}, 'b' };
    (*lamps)[4] = (Lamp) { {306, 97, false}, {306, 115, false}, {306, 133, false}, 'c' };
    (*lamps)[5] = (Lamp) { {373, 97, false}, {373, 115, false}, {373, 133, false}, 'c' };
    (*lamps)[6] = (Lamp) { {499, 116, false}, {499, 134, false}, {499, 152, false}, 'd' };
    (*lamps)[7] = (Lamp) { {499, 209, false}, {499, 227, false}, {499, 245, false}, 'd' };
}
