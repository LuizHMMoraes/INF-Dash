#include "raylib.h"
#include "Mouse.h"
//the name of this file should be 'MouseHandler'
bool IsButtonClicked(const char *text, int x, int y)
{
    int width = MeasureText(text, 30);
    int height = 30;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        GetMouseX() >= x && GetMouseX() <= (x + width) &&
        GetMouseY() >= y && GetMouseY() <= (y + height))
    {
        return true;
    }
    return false;
}

void DrawButton(const char *text, int x, int y)
{
    int width = MeasureText(text, 30);
    int height = 30;
    if (GetMouseX() >= x && GetMouseX() <= (x + width) &&
        GetMouseY() >= y && GetMouseY() <= (y + height))
    {
        DrawText(text, x, y, 33, ORANGE);
    }
    else
    {
        DrawText(text, x, y, 30, BLACK);
    }
}
