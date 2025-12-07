def read_input(file_name: str) -> list[str]:
    with open(file_name, "r") as f:
        lines = f.readlines()

    return lines


def main() -> None:
    lines = read_input("6/input.txt")

    grid = []
    for line in lines[:-1]:
        line = line.strip("\n")
        nums = list(filter(lambda x: x != '', line.split(" ")))
        grid.append(nums)

    codes = list(filter(lambda x: x != '', lines[-1].split(" ")))

    total = 0
    for j, _ in enumerate(grid[0]):
        if codes[j] == '*':
            res = 1
            for i, _ in enumerate(grid):
                res *= int(grid[i][j])
        else:
            res = 0
            for i, _ in enumerate(grid):
                res += int(grid[i][j])
        total += res

    print(total)

if __name__ == "__main__":
    main()
