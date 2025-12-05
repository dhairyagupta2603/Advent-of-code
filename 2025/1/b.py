def read_input(file_name: str) -> list[tuple[str, int]]:
    with open(file_name, "r") as f:
        rotation_codes: list[tuple[str, int]] = []
        for line in f:
            line = line.strip("\n")
            code = (line[0], int(line[1:]))
            rotation_codes.append(code)

    return rotation_codes


def count_zeros(codes: list[tuple[str, int]]) -> int:
    num_zeros: int = 0
    next_num: int = 50

    for code in codes:
        for _ in range(code[1]):
            next_num += -1 if code[0] == "L" else 1

            if next_num < 0:
                next_num = 99
            if next_num > 99:
                next_num = 0

            if next_num == 0:
                num_zeros += 1

    return num_zeros


def main() -> None:
    codes = read_input("1/input.txt")
    print(count_zeros(codes))


if __name__ == "__main__":
    main()
