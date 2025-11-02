#ifndef ANIMATION_H
#define ANIMATION_H

// Define Windows version for modern console functions
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

/* ---------- COLOR CONSTANTS ---------- */
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

// Maximize console window
inline void maximizeConsole() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_MAXIMIZE);
}

// Set console to full screen mode (alternative method)
inline void setFullScreen() {
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

/* ---------- BIG TEXT PRINT ---------- */
inline void printBigText(const string &s, int x, int y, Color color = Color::BRIGHT_CYAN) {
    setColor(color);
    gotoxy(x, y);
    cout << s;
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- SIMPLE FADE IN ---------- */
inline void fadeIn() {
    system("cls");
}

/* ---------- LOADING BAR (FIXED - NO OVERLAP) ---------- */
inline void loadingBar(int x = 30, int y = 20, int w = 50) {
    system("cls");
    const int left = x, top = y, right = x + w, bottom = y + 2;
    
    // Draw loading text above the bar
    setColor(Color::BRIGHT_CYAN);
    gotoxy(left + w/2 - 5, top - 2);
    cout << "LOADING...";
    
    // Draw border
    setColor(Color::BRIGHT_CYAN);
    for (int i = left; i <= right; ++i) {
        gotoxy(i, top);     cout << (char)205;
        gotoxy(i, bottom);  cout << (char)205;
    }
    
    gotoxy(left, top);     cout << (char)201;
    gotoxy(right, top);    cout << (char)187;
    gotoxy(left, bottom);  cout << (char)200;
    gotoxy(right, bottom); cout << (char)188;

    Color grad[] = { Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW };

    // Progress bar animation
    for (int p = 0; p <= 100; p += 5) {
        int len = ((w - 1) * p) / 100;
        for (int i = 0; i < len; ++i) {
            setColor(grad[i % 3]);
            gotoxy(left + 1 + i, top + 1);
            cout << (char)219;
        }
        gotoxy(left + w/2 - 4, bottom + 2);
        setColor(Color::BRIGHT_YELLOW);
        cout << "[ " << setw(3) << p << "% ]";
        Sleep(20);
    }
    
    gotoxy(left + w/2 - 5, bottom + 4);
    setColor(Color::BRIGHT_GREEN);
    cout << "COMPLETE!";
    Sleep(500);
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- MODERN HEADER (CENTERED) ---------- */
inline void drawHeader(const string &title) {
    system("cls");
    
    int centerX = 25;  // Adjusted for centering
    
    // Top line
    setColor(Color::BRIGHT_CYAN);
    gotoxy(centerX, 2); 
    for (int i = 0; i < 80; ++i) cout << (char)220;
    
    // Header box with gradient effect
    setColor(Color::BRIGHT_CYAN, Color::BLUE);
    gotoxy(centerX, 3); cout << (char)201 << string(78, (char)205) << (char)187;
    gotoxy(centerX, 4); cout << (char)186 << string(78, ' ') << (char)186;
    gotoxy(centerX, 5); cout << (char)186 << string(78, ' ') << (char)186;
    gotoxy(centerX, 6); cout << (char)200 << string(78, (char)205) << (char)188;
    
    // Title centered with larger appearance
    int titleX = centerX + 39 - title.length()/2;
    gotoxy(titleX, 4);
    setColor(Color::BRIGHT_YELLOW, Color::BLUE);
    cout << "  ";
    gotoxy(titleX, 5);
    cout << title;
    
    // Bottom line
    setColor(Color::BRIGHT_CYAN, Color::BLACK);
    gotoxy(centerX, 7); 
    for (int i = 0; i < 80; ++i) cout << (char)223;
    
    setColor(Color::BRIGHT_WHITE, Color::BLACK);
    gotoxy(0, 9);
}

/* ---------- FAST PRINT (NO ANIMATION) ---------- */
inline void slowPrint(const string &s, Color fg = Color::BRIGHT_CYAN, int ms = 0) {
    setColor(fg);
    cout << s;
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- CENTERED BOX ---------- */
inline void drawAnimatedBox(int x, int y, int w, int h, Color borderColor = Color::BRIGHT_CYAN) {
    setColor(borderColor);
    
    // Draw all at once (no animation)
    gotoxy(x, y); 
    cout << (char)201;
    for (int i = 1; i < w - 1; ++i) cout << (char)205;
    cout << (char)187;
    
    for (int i = 1; i < h - 1; ++i) {
        gotoxy(x, y + i); cout << (char)186;
        gotoxy(x + w - 1, y + i); cout << (char)186;
    }
    
    gotoxy(x, y + h - 1); 
    cout << (char)200;
    for (int i = 1; i < w - 1; ++i) cout << (char)205;
    cout << (char)188;
    
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- NO PARTICLE EFFECT ---------- */
inline void particleEffect(int duration = 0) {
    // Empty - no particles
}

/* ---------- INSTANT TYPEWRITER ---------- */
inline void typewriter(const string &s, int x, int y, int delay = 0) {
    gotoxy(x, y);
    setColor(Color::BRIGHT_YELLOW);
    cout << s;
    setColor(Color::BRIGHT_WHITE);
}

/* ---------- NO WAVE PRINT ---------- */
inline void wavePrint(const string &s, int delay = 0) {
    setColor(Color::BRIGHT_CYAN);
    cout << s;
    setColor(Color::BRIGHT_WHITE);
}

#endif // ANIMATION_H