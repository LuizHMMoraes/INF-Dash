#ifndef MOUSE_H
#define MOUSE_H

#include "raylib.h"

// Function to check if a button is clicked
bool IsButtonClicked(const char *text, int x, int y);

// Function to draw a button with hover effect
void DrawButton(const char *text, int x, int y);

#endif // MOUSE_H
