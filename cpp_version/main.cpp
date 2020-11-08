//
//  main.cpp
//  line
//
//  Created by Nicolò Padovan on 07/11/2020.
//

#include <iostream>
#include <ncurses.h>
#include <array>
#include <deque>
#include <unistd.h>
#include "snake.h"

using namespace std;

Snake::Snake() {
    curs_set(0);
    updateDims();

    if (Snake::w%2 != 0) {
        resizeterm(Snake::h, Snake::w-1);
        updateDims();
    }
    
    edges[0][1] = w;
    edges[1][1] = h;

    reset();
    start();
}

void Snake::reset() {
    gameOver = FALSE;
    score = 1;
    timeout(0);
    nodelay(stdscr, TRUE);
    
    while (!history.empty()) {
        history.erase(history.begin());
    }
    currentPosition[0] = w/2; currentPosition[1] = h/2;
    if (currentPosition[0]%2 != 0) {
        currentPosition[0] += 1;
    }
    direction = 0;
    foodPosition[0] = 0; foodPosition[1] = 0;
    generateFood(TRUE);
}

void Snake::updateDims() {
    getmaxyx(stdscr, h, w);
}

void Snake::start() {
    while (TRUE) {
        while (!gameOver) {
            updateWindow();
        }
        
        nodelay(stdscr, FALSE);
        timeout(-1);
        clear();
        wmove(stdscr, 0, 0);
        wprintw(stdscr, "Game-over");
        getch();
        reset();
    }
}

void Snake::updateWindow() {
    int key = getch();

    if (key == KEY_UP) {
        if (direction != 2) {
            direction = 0;
        }
    } else if (key == KEY_RIGHT) {
        if (direction != 3) {
            direction = 1;
        }
    } else if (key == KEY_DOWN) {
        if (direction != 0) {
            direction = 2;
        }
    } else if (key == KEY_LEFT) {
        if (direction != 1) {
            direction = 3;
        }
    }

    history.push_front(currentPosition);
    while (history.size() > score) {
        history.erase(history.end());
    }
    
    switch (direction) {
        case 0:
            currentPosition[1] -= 1;
            break;
        case 1:
            currentPosition[0] += 2;
            break;
        case 2:
            currentPosition[1] += 1;
            break;
        case 3:
            currentPosition[0] -= 2;
            break;
    }
    
    bool find = std::find(history.begin(), history.end(), currentPosition) != history.end();
    if (currentPosition[0] < edges[0][0] || currentPosition[0] > edges[0][1] || currentPosition[1] < edges[1][0] || currentPosition[1] > edges[1][1] || find) {
        gameOver = TRUE;
        return;
    } else {
        clear();
        array<short int, 2> foodPos = generateFood(FALSE);
        addString("OO", foodPos[1], foodPos[0]);
        addString("##", currentPosition[1], currentPosition[0]);
        generateTail();
    }
    
    usleep(180 * 1000);
}

array<short int, 2> Snake::generateFood(bool ignore) {
    if (currentPosition == foodPosition || ignore) {
        score += 1;
        array<short int, 2> newFoodPos;
        while (TRUE) {
            short rand_1 = rand()%edges[0][1];
            short rand_2 = rand()%edges[1][1];

            newFoodPos[0] = rand_1; newFoodPos[1] = rand_2;

            if (newFoodPos != currentPosition && newFoodPos[0]%2==0) {
                break;
            }
        }
        foodPosition = newFoodPos;
        return newFoodPos;
    } else {
        return foodPosition;
    }
}

void Snake::generateTail() {
    for(int i=0; i<history.size(); i++) {
        addString("##", history[i][1], history[i][0]);
    }
}

void Snake::addString(const char text[2], int y, int x) {
    wmove(stdscr, y, x);
    addstr(text);
}

int main() {
    
    initscr();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    cbreak();
    noecho();
//    nodelay(stdscr, TRUE);
    
    Snake snake;
}

