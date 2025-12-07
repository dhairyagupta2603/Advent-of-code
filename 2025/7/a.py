
def read_input(file_name: str) -> list[list[str]]:
    steps: list[list[str]] = []
    with open(file_name, "r") as f:
        rows = f.readlines()
        for row in rows:
            steps.append(list(row.strip("\n")))

    return steps

def compute_splits(steps: list[list[str]], beam_pos: int) -> int:
    steps[0][beam_pos] = '|'
    num_splits: int = 0

    for i, step in enumerate(steps):
        for j, _ in enumerate(step):
            if steps[i-1][j] != '|':
                continue

            if steps[i][j] != '^':
                steps[i][j] = '|'
            else:
                steps[i][j-1] = '|'
                steps[i][j+1] = '|'
                num_splits += 1

    return num_splits


def main() -> None:
    rows = read_input("7/input.txt")

    # determine beam pos
    pos: int = 0
    for i , val in enumerate(rows[0]):
        if val == 'S':
            pos = i

    print(compute_splits(rows[1:], pos))


if __name__ == "__main__":
    main()
