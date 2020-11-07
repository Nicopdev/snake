import curses as cr
import logging
import os
from time import sleep, time
from numpy import random
import threading

class Snake:
    
    def __init__(self, screen):
        
        self.init_logger()
        cr.curs_set(0)
        self.updateDims(screen)
                
        screen.timeout(1)
                
        if self.w%2 != 0:
            cr.resizeterm(self.h, self.w-1)
        
        self.updateDims(screen)
        self.edges = ((0,self.w), (0, self.h - 1))
        
        self.reset(screen)
        
        self.start(screen)

    def reset(self, screen):
        self.start_time = 0
        self.game_over = False
        self.score = 1
        self.history = list()
        self.current_position = [self.w//2, self.h//2]
        if self.current_position[0]%2 != 0:
            self.current_position[0] += 1
        self.direction = "RIGHT"
        self.food_position = [0, 0]
        self.food_position = self.generate_food(True)
        screen.timeout(1)

    def init_logger(self):
        if not os.path.exists("snake_files"):
            os.makedirs("snake_files")
        
        self.logger = logging.getLogger("snake_logs")
        hdlr = logging.FileHandler("snake_files/snake_logs" + ".log")
        formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
        hdlr.setFormatter(formatter)
        self.logger.addHandler(hdlr)
        self.logger.setLevel(logging.DEBUG)
        
        self.logger.info("\n\n\nLogger initiated")

    def updateDims(self, screen):
        self.h, self.w = screen.getmaxyx()

    def start(self, screen):
        while True:
            while not self.game_over:
                self.update_window(screen)                    
            
            screen.timeout(-1)
            screen.clear()
            screen.addstr(0, 0, "Gameover")        
            screen.getch()
            self.reset(screen)

    def update_window(self, screen):
        current_time = time() - self.start_time
        
        thread = threading.Thread(target=self.generate_tail, args=(screen,))
        if current_time > 0.1:
            
            key = screen.getch()
            if key < 0:
                pass
            else:
                if key == cr.KEY_UP:
                    if self.direction != "DOWN":
                        self.direction = "UP"
                elif key == cr.KEY_DOWN:
                    if self.direction != "UP":
                        self.direction = "DOWN"
                elif key == cr.KEY_LEFT:
                    if self.direction != "RIGHT":    
                        self.direction = "LEFT"
                elif key == cr.KEY_RIGHT:
                    if self.direction != "LEFT":
                        self.direction = "RIGHT"
            
            self.history.insert(0, self.current_position.copy())
            self.history = self.history[:self.score]
            
            if self.direction == "UP":
                self.current_position[1] -= 1
            elif self.direction == "DOWN":
                self.current_position[1] += 1
            elif self.direction == "LEFT":
                self.current_position[0] -= 2
            elif self.direction == "RIGHT":
                self.current_position[0] += 2
            
            # Update screen
            screen.clear()
            food_pos = self.generate_food()
            try:
                screen.addstr(food_pos[1], food_pos[0], "FF")
                screen.addstr(self.current_position[1], self.current_position[0], "AA")
            except:
                # self.logger.info(f"Problem while adding string. Positions: {food_pos} , {self.current_position}")
                print("Error")
                
            thread.start()
            
            self.logger.info(f"{self.current_position} , {self.edges[1][1]}")
            
            if self.current_position[0] < self.edges[0][0] or self.current_position[0] > self.edges[0][1] or self.current_position[1] < self.edges[1][0] or self.current_position[1] > self.edges[1][1] or self.current_position in self.history:
                self.game_over = True
                return
                
            self.start_time = time()
            self.last_score = self.score
    
    def generate_tail(self, screen):
        for position in self.history[:self.score]:
            screen.addstr(position[1], position[0], "AA")
    
    def generate_food(self, ignore=False):
        
        if self.current_position == self.food_position or ignore:
            
            self.score += 1
            
            new_food_pos = None
            while True:
                new_food_pos = [random.randint(self.edges[0][0], self.edges[0][1]), random.randint(self.edges[1][0], self.edges[1][1])]
                if not new_food_pos == self.current_position and new_food_pos[0]%2==0:
                    break
            
            self.food_position = new_food_pos
                
            
            return new_food_pos
        else:
            return self.food_position

def main(screen):
    _ = Snake(screen)


if __name__ == '__main__':
    cr.wrapper(main)
