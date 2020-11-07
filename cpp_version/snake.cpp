#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <array>
#include <deque>

using namespace std;

class Snake {
    public:
        int w, h;
        unsigned int score = 1;
        array<short int, 2> currentPosition = {};
        array<short int, 2> foodPosition = {};
        std::deque<array<short int, 2>> history;
        unsigned char direction = 0; // 0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT
        unsigned long int startTime;
        int edges[2][2] = {{0,0},{0,0}};
        bool gameOver = FALSE;
        void addString(const char text[2], int y, int x);
        void start();
        void updateWindow();
        void updateDims();
        void reset();
        array<short int, 2> generateFood(bool ignore);
        void generateTail();
        Snake();
};

Snake::Snake() {
    curs_set(0);
    updateDims();
    
    if (Snake::w%2 != 0) {
        resizeterm(Snake::h, Snake::w-1);
        updateDims();
    }
    
    Snake::edges[0][1] = w;
    Snake::edges[1][1] = h;
    
    reset();
    start();
}

void Snake::reset() {
    startTime = clock();
    gameOver = FALSE;
    score = 1;
    
    while (!history.empty()) {
        history.erase(history.begin());
    }
    currentPosition[0] = Snake::w/2; currentPosition[1] = h/2;
    if (currentPosition[0]%2 != 0) {
        currentPosition[0] += 1;
    }
    timeout(1);
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
        
        timeout(-1);
        clear();
        wmove(stdscr, 0, 0);
        wprintw(stdscr, "Game-over");
        getch();
        reset();
    }
}

void Snake::updateWindow() {
    unsigned long currentTime = clock() - startTime / CLOCKS_PER_SEC;
    
    if (currentTime > 1) {
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
        
        clear();
        array<short int, 2> foodPos = generateFood(FALSE);
        addString("FF", foodPos[1], foodPos[0]);
        addString("AA", currentPosition[1], currentPosition[0]);
        
        bool found = find(history.begin(), history.end(), currentPosition) == history.end();
        if (currentPosition[0] < edges[0][0] || currentPosition[0] > edges[0][1] || currentPosition[1] < edges[0][1] || currentPosition[1] > edges[1][1] || found) {
            gameOver = TRUE;
            return;
        }
        
        startTime = clock();
    }
}

array<short int, 2> Snake::generateFood(bool ignore) {
    if (currentPosition == foodPosition || ignore) {
        score += 1;
        array<short int, 2> newFoodPos;
        while (TRUE) {
            short int rand_1 = rand()%edges[0][1];
            short int rand_2 = rand()%edges[1][1];
            
            newFoodPos = {rand_1, rand_2};
//            newFoodPos[0] = rand_1; newFoodPos[1] = rand_2;
            
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
        array<short int, 2> position = history[i];
        addString("AA", position[1], position[0]);
    }
}

void Snake::addString(const char text[2], int y, int x) {
    wmove(stdscr, y, x);
    addstr(text);
}

int main() {
    initscr();
    start_color();
    curs_set(0);
    
    Snake snake;
}
