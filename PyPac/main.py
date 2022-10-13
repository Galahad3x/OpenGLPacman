from random import randint, shuffle
from itertools import permutations


def p_maze(maze):
    print("---------------")
    for line in maze:
        for e in line:
            print(e, end="")
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


def diagonal_paths(x, y, maze, comb):
    possibles = [(x - 1, y - 1), (x - 1, y), (x - 1, y + 1), (x, y + 1), (x + 1, y + 1), (x + 1, y), (x + 1, y - 1),
                 (x, y - 1), (x - 1, y - 1), (x - 1, y)]
    true = []
    for n_x, n_y in possibles:
        if n_x >= len(maze) or n_x < 0 or n_y >= len(maze[0]) or n_y < 0:
            true.append(0)
        elif maze[n_x][n_y] == 1 or (n_x, n_y) in comb:
            true.append(1)
        else:
            true.append(0)
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


def possible_n(new_paths, nbs, maze):
    neighbour_choice = []
    if len(nbs) == 1:
        neighbour_choice.append([nbs[0]])
    else:
        # Calculate all combinations
        for comb in permutations(nbs, new_paths):
            neighbour_choice.append(comb)
    true_neighbour = []
    for comb in neighbour_choice:
        to_add = True
        for elem in comb:
            ps = diagonal_paths(elem[0], elem[1], maze, comb)
            for i in range(8):
                if i % 2 == 1 and ps[i] == 1:
                    if ps[i + 1] == 1 and ps[i + 2] == 1:
                        to_add = False
                        break
        if to_add:
            true_neighbour.append(comb)
    shuffle(true_neighbour)
    return true_neighbour


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
    for _ in range(2):
        nbs = neighbours(x, y, maze)
        new_paths = 0
        # paths will be at least one
        ps = len(paths(x, y, maze))
        if len(nbs) == 0 and ps < 2:
            maze[x][y] = 1
            print("X", end="")
            return None
            # return maze
        if ps == 0:
            new_paths = 2
            if randint(0, 100) < 7:
                new_paths += 1
        if ps == 1:
            new_paths = 1
            if randint(0, 100) < 20:
                new_paths += 1
            if randint(0, 100) < 8:
                new_paths += 1
        new_paths = min(new_paths, len(nbs))
        neighbour_choices = possible_n(new_paths, nbs, maze)
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
    maze[start_height][-1] = 2
    maze[start_height][-2] = 2
    maze[start_height + 1][-2] = 2
    maze[start_height + 2][-2] = 2
    maze[start_height + 3][-1] = 2
    maze[start_height + 3][-2] = 2
    # Start dfs
    stack = []
    start_x = 0
    start_y = 0
    # node loop
    stack.append((start_x, start_y))
    # repeat until stack empty
    maze = maze_harder(maze, stack)
    return maze

def do_dfs(maze, stack):
    print()
    print("|" * len(stack), end="")
    if len(stack) == 0:
        return maze
    p_maze(maze)
    x, y = stack[-1]
    stack = stack[:-1]

    # all NEW neighbours
    nbs = neighbours(x, y, maze)
    new_paths = 0
    # paths will be at least one
    ps = len(paths(x, y, maze))
    if len(nbs) == 0 and ps < 2:
        maze[x][y] = 1
        print("X", end="")
        return None
        # return maze
    if ps == 0:
        new_paths = 2
        if randint(0, 100) < 7:
            new_paths += 1
    if ps == 1:
        new_paths = 1
        if randint(0, 100) < 20:
            new_paths += 1
        if randint(0, 100) < 8:
            new_paths += 1
    new_paths = min(new_paths, len(nbs))
    neighbour_choices = possible_n(new_paths, nbs, maze)


def dfs_maze(maze):
    corridor = 2
    stack = []
    is_completed = False
    while not is_completed:
        is_completed = True
        for i,row in enumerate(maze):
            for j, elem in enumerate(row):
                if elem == 0:
                    is_completed = False
                    stack.append((i,j))
                    break
            if not is_completed:
                break
        if not is_completed:
            maze = do_dfs(maze, stack, corridor)
    return maze

def generate_maze_2(rows, cols):
    maze = []
    for i in range(rows):
        maze.append([0] * cols)
    # Add middle room
    start_height = rows // 2 - 2
    maze[start_height][-1] = 1
    maze[start_height][-2] = 1
    maze[start_height + 1][-2] = 1
    maze[start_height + 2][-2] = 1
    maze[start_height + 3][-1] = 1
    maze[start_height + 3][-2] = 1

    maze = dfs_maze(maze)
    maze[start_height + 1][-2] = 2
    maze[start_height + 2][-2] = 2
    return maze


if __name__ in "__main__":
    rows = 10
    cols = 6
    maze = generate_maze(rows, cols)
    p_maze(maze)
