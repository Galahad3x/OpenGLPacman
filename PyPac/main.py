from random import randint, shuffle
from itertools import combinations


def p_maze(maze):
    print("---------------")
    for line in maze:
        for e in line:
            print(e, end=" ")
        print()
    print("---------------")


def neighbours(x, y, maze):
    possibles = [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    true = []
    for n_x, n_y in possibles:
        if n_x >= len(maze) or n_x < 0 or n_y >= len(maze[0]) or n_y < 0:
            continue
        if maze[n_x][n_y] == 0:
            true.append((n_x, n_y))
    return true


def paths(x, y, maze):
    possibles = [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    true = []
    for n_x, n_y in possibles:
        if n_x >= len(maze) or n_x < 0 or n_y >= len(maze[0]) or n_y < 0:
            continue
        if maze[n_x][n_y] == 1:
            true.append((n_x, n_y))
    return true


def is_valid(maze):
    for i, row in enumerate(maze):
        for j, val in enumerate(row):
            if val == 1:
                nbs = neighbours(i, j, maze)
                ps = len(paths(i, j, maze))
                if len(nbs) == 0 and ps < 2:
                    print("X", end="")
                    return False
            if val == 0:
                return False
    return True


def possible_n(new_paths, nbs):
    neighbour_choice = []
    if len(nbs) == 1:
        neighbour_choice.append([nbs[0]])
    else:
        # Calculate all combinations
        for elem in combinations(nbs, new_paths):
            neighbour_choice.append(elem)
    # Eliminar neighbours que farien clusters
    shuffle(neighbour_choice)
    return neighbour_choice


def maze_harder(maze, stack):
    print()
    print("|" * len(stack), end="")
    if len(stack) == 0:
        return maze
    p_maze(maze)
    x, y = stack[-1]
    stack = stack[:-1]
    # select random neighbours to become path, to have at least 2
    # nbs is possible NEW neighbours
    # add neighbours to stack
    for _ in range(3):
        nbs = neighbours(x, y, maze)
        new_paths = 0
        # paths will be at least one
        ps = len(paths(x, y, maze))
        if len(nbs) == 0 and ps < 2:
            print("X", end="")
            return None
        if ps == 1:
            new_paths = 1
            if randint(0, 100) < 30:
                new_paths += 1
            if randint(0, 100) < 5:
                new_paths += 1
        new_paths = min(new_paths, len(nbs))
        neighbour_choices = possible_n(new_paths, nbs)
        for neighbour_choice in neighbour_choices:
            st = stack[:]
            stack.extend(neighbour_choice)
            for n in nbs:
                if n in neighbour_choice:
                    maze[n[0]][n[1]] = 1
                else:
                    maze[n[0]][n[1]] = 2
            maze2 = maze_harder(maze, stack)
            if maze2 is None:
                # st = stack[:]
                # maze2 = maze_harder(maze, stack)
                for n in nbs:
                    maze[n[0]][n[1]] = 0
                stack = st
            else:
                return maze2
    else:
        return None


def generate_maze(rows, cols):
    maze = []
    for i in range(rows):
        maze.append([0] * cols)
    # Add middle room
    start_height = rows // 2 - 2
    maze[start_height - 1][-1] = 1
    maze[start_height][-1] = 1
    maze[start_height + 1][-1] = 1
    maze[start_height + 2][-1] = 1
    maze[start_height + 1][-2] = 1
    maze[start_height + 2][-2] = 1
    maze[start_height][-2] = 2
    maze[start_height][-3] = 2
    maze[start_height + 1][-3] = 2
    maze[start_height + 2][-3] = 2
    maze[start_height + 3][-1] = 2
    maze[start_height + 3][-2] = 2
    maze[start_height + 3][-3] = 2
    # Start dfs
    stack = []
    visited = []
    start_x = start_height - 1
    start_y = cols - 1
    # node loop
    stack.append((start_x, start_y))
    # repeat until stack empty
    maze = maze_harder(maze, stack)
    return maze


if __name__ in "__main__":
    rows = 12
    cols = 8
    maze = generate_maze(rows, cols)
    p_maze(maze)
