//
//  main.cpp
//  line
//
//  Created by Nicolò Padovan on 07/11/2020.
//

#include <time.h>
#include <array>
#include <deque>

using Point = std::array<short, 2>;

class Snake {
    public:
        short w, h;
        std::deque<Point> history;
        unsigned int score = 0;
        Point currentPosition = {10, 10};
        Point foodPosition = {};
        unsigned char direction = 0; // 0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT
        time_t startTime = 0;
        short edges[2][2] = {{0,0},{0,0}};
        bool gameOver = FALSE;
        void addString(const char text[2], int y, int x);
        void start();
        void updateWindow();
        void updateDims();
        void reset();
        Point generateFood(bool ignore);
        void generateTail();
        Snake();
};
