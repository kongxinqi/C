#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define WIDTH 80
#define HEIGHT 25

void initScreen() {
    system("clear");
}

void setCursor(int x, int y) {
    printf("\x1B[%d;%dH", y, x);
}

void printChar(char c, int x, int y, int color) {
    setCursor(x, y);
    if (color != -1) {
        printf("\x1B[%dm%c\x1B[0m", color, c);
    } else {
        printf("%c", c);
    }
}

void printMatrix() {
    char chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{}[];()<>.";
    int colors[] = {31, 32, 33, 34, 35, 36, 37}; // ANSI colors
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int charIndex = rand() % (sizeof(chars) - 1);
            int colorIndex = rand() % (sizeof(colors) / sizeof(int));
            int color = colors[colorIndex];
            char c = chars[charIndex];
            if (x % 10 == 0) c = 'X';
            if (y == HEIGHT - 1) c = '_';
            printChar(c, x, y, color);
        }
    }
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    initScreen();
    while (1) {
        printMatrix();
        usleep(20000); // �ȴ�20����
        if (kbhit()) {
            char ch = getchar();
            if (ch == 27) { // ��ESC�˳�
                break;
            }
        }
    }
    return 0;
}
