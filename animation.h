#ifndef ANIMATION_H
#define ANIMATION_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

/**
 * Move the console cursor to the specified column and row.
 * Coordinates use the console character grid with (0,0) at the top-left.
 * @param x Column index (horizontal position), zero-based.
 * @param y Row index (vertical position), zero-based.
 */

/**
 * Set the console text foreground and background colors.
 * @param fg Foreground color from the `Color` enum.
 * @param bg Background color from the `Color` enum (default: `Color::BLACK`).
 */
enum class Color {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4,
    MAGENTA = 5, YELLOW = 6, WHITE = 7, GRAY = 8,
    BRIGHT_BLUE = 9, BRIGHT_GREEN = 10, BRIGHT_CYAN = 11,
    BRIGHT_RED = 12, BRIGHT_MAGENTA = 13, BRIGHT_YELLOW = 14, BRIGHT_WHITE = 15
};

/* ---------- CONSOLE HELPERS ---------- */
inline void gotoxy(short x, short y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

inline void setColor(Color fg, Color bg = Color::BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)fg | ((int)bg << 4));
}

/**
 * Maximizes the current process console window.
 *
 * If no console window exists, the function has no effect.
 */
inline void maximizeConsole() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_MAXIMIZE);
}

/**
 * Hide the console cursor for the current standard output console.
 *
 * Sets the console cursor to an invisible state so subsequent console output
 * does not display a blinking cursor.
 */
inline void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

/**
 * Make the console cursor visible and set its size to 100.
 *
 * This updates the current process console's cursor to be shown and sets the cursor
 * size to 100 (percent of character cell). No value is returned.
 */
inline void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}





/* ---------- CENTERED LOADING BAR ---------- */
inline void loadingBar(int duration = 1500) {
    system("cls");
    hideCursor();
    
    int centerX = 40;
    int centerY = 14;
    int barWidth = 50;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(centerX + 17, centerY - 2);
    cout << "LOADING SYSTEM...";
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(centerX, centerY);
    cout << char(201);
    for (int i = 0; i < barWidth; i++) cout << char(205);
    cout << char(187);
    
    gotoxy(centerX, centerY + 2);
    cout << char(200);
    for (int i = 0; i < barWidth; i++) cout << char(205);
    cout << char(188);
    
    Color rainbow[] = {Color::BRIGHT_RED, Color::BRIGHT_YELLOW, Color::BRIGHT_GREEN, 
                       Color::BRIGHT_CYAN, Color::BRIGHT_BLUE, Color::BRIGHT_MAGENTA};
    
    int steps = 20;
    int delayPerStep = duration / steps;
    
    for (int p = 0; p <= steps; p++) {
        int len = (barWidth * p) / steps;
        for (int i = 0; i < len; i++) {
            setColor(rainbow[(i * 6) / barWidth]);
            gotoxy(centerX + 1 + i, centerY + 1);
            cout << char(219);
        }
        
        int percent = (p * 100) / steps;
        gotoxy(centerX + barWidth/2 - 2, centerY + 4);
        setColor(percent < 30 ? Color::BRIGHT_RED : percent < 70 ? Color::BRIGHT_YELLOW : Color::BRIGHT_GREEN);
        cout << "[ " << setw(3) << percent << "% ]";
        Sleep(delayPerStep);
    }
    
    gotoxy(centerX + 18, centerY + 6);
    setColor(Color::BRIGHT_GREEN);
    cout << "COMPLETE!";
    Sleep(400);
    showCursor();
    setColor(Color::BRIGHT_WHITE);
}

/**
 * Draws a decorative, colored header bar across the console and centers the provided title within it.
 *
 * This clears the console, renders a multi-line header with color accents and a gradient-like border,
 * places the `title` centered horizontally within the header area, and then positions the cursor
 * below the header ready for subsequent output.
 *
 * @param title Title text to center and display within the header.
 */
inline void drawHeader(const string &title) {
    system("cls");
    
    int centerX = 20;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(centerX, 2);
    for (int i = 0; i < 90; i++) cout << char(220);
    
    setColor(Color::BRIGHT_CYAN, Color::BLUE);
    gotoxy(centerX, 3); cout << char(201) << string(88, char(205)) << char(187);
    
    setColor(Color::BRIGHT_YELLOW, Color::BLUE);
    gotoxy(centerX, 4); cout << char(186) << string(88, ' ') << char(186);
    gotoxy(centerX, 5); cout << char(186) << string(88, ' ') << char(186);
    
    setColor(Color::BRIGHT_CYAN, Color::BLUE);
    gotoxy(centerX, 6); cout << char(200) << string(88, char(205)) << char(188);
    
    int titleX = centerX + 44 - title.length()/2;
    setColor(Color::BRIGHT_YELLOW, Color::BLUE);
    gotoxy(titleX, 5);
    cout << title;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(centerX, 7);
    for (int i = 0; i < 90; i++) cout << char(223);
    
    setColor(Color::BRIGHT_WHITE, Color::BLACK);
    gotoxy(0, 9);
}

/* ---------- FAST PRINT ---------- */
inline void slowPrint(const string &s, Color fg = Color::BRIGHT_CYAN, int ms = 0) {
    setColor(fg);
    cout << s;
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- FAST ANIMATED BOX ---------- */
inline void drawAnimatedBox(int x, int y, int w, int h, Color borderColor = Color::BRIGHT_CYAN) {
    setColor(borderColor);
    
    gotoxy(x, y); 
    cout << char(201);
    for (int i = 1; i < w - 1; ++i) cout << char(205);
    cout << char(187);
    
    for (int i = 1; i < h - 1; ++i) {
        gotoxy(x, y + i); cout << char(186);
        gotoxy(x + w - 1, y + i); cout << char(186);
    }
    
    gotoxy(x, y + h - 1); 
    cout << char(200);
    for (int i = 1; i < w - 1; ++i) cout << char(205);
    cout << char(188);
    
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- CENTERED SPINNER ---------- */
inline void showSpinner(int duration = 500, int centerX = 60, int centerY = 15) {
    char spinner[] = {'|', '/', '-', '\\'};
    setColor(Color::BRIGHT_CYAN);
    int loops = duration / 80;
    for (int i = 0; i < loops; i++) {
        gotoxy(centerX, centerY);
        cout << "  " << spinner[i % 4] << " Processing...";
        Sleep(80);
    }
    gotoxy(centerX, centerY);
    cout << "                    ";
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- CENTERED TOAST MESSAGES ---------- */
inline void showSuccessToast(const string &message, int centerX = 50, int centerY = 24) {
    gotoxy(centerX - message.length()/2, centerY);
    setColor(Color::BRIGHT_GREEN, Color::BLACK);
    cout << " " << message << " ";
    Sleep(800);
    
    gotoxy(centerX - message.length()/2, centerY);
    cout << string(message.length() + 2, ' ');
    setColor(Color::BRIGHT_WHITE);
}

inline void showErrorToast(const string &message, int centerX = 50, int centerY = 24) {
    gotoxy(centerX - message.length()/2, centerY);
    setColor(Color::BRIGHT_RED, Color::BLACK);
    cout << " " << message << " ";
    Sleep(800);
    
    gotoxy(centerX - message.length()/2, centerY);
    cout << string(message.length() + 2, ' ');
    setColor(Color::BRIGHT_WHITE);
}

inline void showInfoToast(const string &message, int centerX = 50, int centerY = 24) {
    gotoxy(centerX - message.length()/2, centerY);
    setColor(Color::BRIGHT_CYAN, Color::BLACK);
    cout << " " << message << " ";
    Sleep(600);
    
    gotoxy(centerX - message.length()/2, centerY);
    cout << string(message.length() + 2, ' ');
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- PULSE TEXT ---------- */
inline void pulseText(const string &text, int x, int y, int cycles = 2) {
    for (int c = 0; c < cycles; c++) {
        gotoxy(x, y);
        setColor(Color::BRIGHT_YELLOW);
        cout << text;
        Sleep(150);
        
        gotoxy(x, y);
        setColor(Color::YELLOW);
        cout << text;
        Sleep(150);
    }
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- TYPEWRITER EFFECT ---------- */
inline void typewriter(const string &s, int x, int y, int delay = 30) {
    gotoxy(x, y);
    setColor(Color::BRIGHT_YELLOW);
    for (char c : s) {
        cout << c << flush;
        if (delay > 0) Sleep(delay);
    }
    setColor(Color::BRIGHT_WHITE);
}


inline void drawDynamicBox(int x, int y, int w, int h, Color borderColor = Color::BRIGHT_CYAN) {
    setColor(borderColor);
    
    // Top border
    gotoxy(x, y); 
    cout << char(201);
    for (int i = 1; i < w - 1; ++i) cout << char(205);
    cout << char(187);
    
    // Side borders
    for (int i = 1; i < h - 1; ++i) {
        gotoxy(x, y + i); cout << char(186);
        gotoxy(x + w - 1, y + i); cout << char(186);
    }
    
    // Bottom border
    gotoxy(x, y + h - 1); 
    cout << char(200);
    for (int i = 1; i < w - 1; ++i) cout << char(205);
    cout << char(188);
    
    setColor(Color::BRIGHT_WHITE);
}

#endif // ANIMATION_H