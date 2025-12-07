def read_input(file_name: str) -> list[list[str]]:
    steps: list[list[str]] = []
    with open(file_name, "r") as f:
        rows = f.readlines()
        for row in rows:
            steps.append(list(row.strip("\n")))

    return steps

def count_timelines(grid: list[list[str]], beam_pos: tuple[int, int]) -> int:
    i, j = beam_pos
    if i >= len(grid): # beam reaches the end
        return 1
    
    if grid[i][j] == '.': # beam doesn't split
        return count_timelines(grid, (i+1, j))
    
    if grid[i][j] == '^':
        grid[i][j] = str(count_timelines(grid, (i, j-1)) + count_timelines(grid, (i, j+1)))
    
    return int(grid[i][j])


def main() -> None:
    rows = read_input("7/input.txt")

    # determine beam pos
    pos: int = 0
    for i , val in enumerate(rows[0]):
        if val == 'S':
            pos = i

    rows[0][pos] = '.'
    print(count_timelines(rows, (0, pos)))


if __name__ == "__main__":
    main()
