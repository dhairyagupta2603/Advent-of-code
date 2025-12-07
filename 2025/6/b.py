def read_input(file_name: str) -> list[str]:
    with open(file_name, "r") as f:
        lines = f.readlines()

    return lines



def main() -> None:
    lines = read_input("6/input.txt")

    # get codes
    codes = list(filter(lambda x: x != '', lines[-1].split(" ")))

    # creata aligned grid for the values
    grid = []
    for line in lines[:-1]:
        line = list(line.strip("\n"))
        grid.append(line)

    # creat a 1d tape for each question sperated by thier corresponding operator
    pos = len(codes) - 1
    grid2: list[str] = []
    for j in range(len(grid[0])-1, -1, -1):
        row2: list[str] = []
        for i in range(len(grid)):
            if grid[i][j] == ' ':
                grid[i][j] = ''

            row2.append(grid[i][j])

        row = ''.join(row2)
        if row == '':
            row = codes[pos]
            pos-=1

        grid2.append(row)

    grid2.append(codes[pos])

    # compute the tape 
    running_total = 0
    curr_run: list[int] = []
    for num in grid2:
        if num not in ['+', '*']:
            curr_run.append(int(num))
            continue

        if num == '+':
            total = 0
            for val in curr_run:
                total += val
        else:
            total = 1
            for val in curr_run:
                total *= val

        running_total += total
        curr_run = []
    
    print(running_total)


if __name__ == "__main__":
    main()
