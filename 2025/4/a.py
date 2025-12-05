def read_input(file_name: str) -> list[list[str]]:
    grid: list[list[str]] = []
    with open(file_name, "r") as f:
        for line in f.readlines():
            grid.append(list(line.strip("\n")))

    return grid


def count_neighbours(grid, row, col) -> int:
    num_neighbours: int = 0
    for i in (row - 1, row, row + 1):
        if i < 0 or i >= len(grid):
            continue

        for j in (col - 1, col, col + 1):
            if j < 0 or j >= len(grid[0]):
                continue

            if i == row and j == col:
                continue

            num_neighbours += 1 if grid[i][j] == "@" else 0

    return num_neighbours


def count_removed_rolls(grid: list[list[str]]) -> int:
    num_removed_rolls: int = 0
    for i, row in enumerate(grid):
        for j, _ in enumerate(row):
            if grid[i][j] != "@":
                continue

            if count_neighbours(grid, i, j) >= 4:
                continue

            num_removed_rolls += 1

    return num_removed_rolls


def main() -> None:
    grid = read_input("4/input.txt")
    print(f"Removed: {count_removed_rolls(grid)}")


if __name__ == "__main__":
    main()
