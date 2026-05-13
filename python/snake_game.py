"""Terminal Snake Game — use WASD or arrow keys, q to quit."""

import curses
import random
import time

def main(stdscr):
    curses.curs_set(0)
    stdscr.nodelay(True)
    stdscr.keypad(True)

    curses.start_color()
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)   # snake
    curses.init_pair(2, curses.COLOR_RED,   curses.COLOR_BLACK)   # food
    curses.init_pair(3, curses.COLOR_YELLOW, curses.COLOR_BLACK)  # score

    max_y, max_x = stdscr.getmaxyx()
    height, width = max_y - 2, max_x - 2  # playfield inside border

    def new_food(snake):
        while True:
            pos = (random.randint(1, height), random.randint(1, width))
            if pos not in snake:
                return pos

    # Initial state
    cy, cx = height // 2, width // 2
    snake = [(cy, cx), (cy, cx - 1), (cy, cx - 2)]
    direction = (0, 1)  # moving right
    food = new_food(snake)
    score = 0
    speed = 0.15

    KEY_MAP = {
        ord('w'): (-1, 0), curses.KEY_UP:    (-1, 0),
        ord('s'): ( 1, 0), curses.KEY_DOWN:  ( 1, 0),
        ord('a'): ( 0,-1), curses.KEY_LEFT:  ( 0,-1),
        ord('d'): ( 0, 1), curses.KEY_RIGHT: ( 0, 1),
    }

    def draw():
        stdscr.clear()
        # Border
        stdscr.border()
        # Food
        fy, fx = food
        stdscr.addch(fy, fx, '@', curses.color_pair(2) | curses.A_BOLD)
        # Snake
        for i, (sy, sx) in enumerate(snake):
            ch = '█' if i == 0 else '▪'
            stdscr.addch(sy, sx, ch, curses.color_pair(1) | curses.A_BOLD)
        # Score
        stdscr.addstr(0, 2, f" Score: {score} ", curses.color_pair(3))
        stdscr.addstr(0, max_x - 20, " WASD/↑↓←→ Q退出 ")
        stdscr.refresh()

    last_move = time.time()

    while True:
        key = stdscr.getch()
        if key == ord('q'):
            break
        if key in KEY_MAP:
            nd = KEY_MAP[key]
            # Prevent reversing
            if (nd[0] + direction[0], nd[1] + direction[1]) != (0, 0):
                direction = nd

        now = time.time()
        if now - last_move < speed:
            time.sleep(0.02)
            draw()
            continue
        last_move = now

        # Move
        hy, hx = snake[0]
        dy, dx = direction
        ny, nx = hy + dy, hx + dx

        # Wall collision
        if ny < 1 or ny > height or nx < 1 or nx > width:
            break
        # Self collision
        if (ny, nx) in snake:
            break

        snake.insert(0, (ny, nx))
        if (ny, nx) == food:
            score += 10
            food = new_food(snake)
            speed = max(0.05, speed - 0.003)  # speed up slightly
        else:
            snake.pop()

        draw()

    # Game over screen
    stdscr.nodelay(False)
    msg = f"游戏结束！最终得分: {score}  按任意键退出"
    stdscr.addstr(max_y // 2, max(0, (max_x - len(msg)) // 2), msg, curses.color_pair(3) | curses.A_BOLD)
    stdscr.refresh()
    stdscr.getch()


if __name__ == "__main__":
    curses.wrapper(main)
