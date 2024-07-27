#include "raylib.h"
#include "menu.h"

bool IsButtonClicked(const char *text, int x, int y)
{
    Vector2 mousePoint = GetMousePosition();
    Rectangle button = { x, y, MeasureText(text, 20) + 20, 30 };

    if (CheckCollisionPointRec(mousePoint, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }

    return false;
}

void DrawButton(const char *text, int x, int y)
{
    DrawRectangle(x, y, MeasureText(text, 20) + 20, 30, LIGHTGRAY);
    DrawText(text, x + 10, y + 5, 20, DARKGRAY);
}
