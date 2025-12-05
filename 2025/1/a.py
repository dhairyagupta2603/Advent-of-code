def read_input(file_name: str) -> list[tuple[str, int]]:
    with open(file_name, 'r') as f:
        rotation_codes: list[tuple[str, int]] = []
        for line in f:
            line = line.strip('\n')
            code = (line[0], int(line[1:]))
            rotation_codes.append(code)

    return rotation_codes


def count_zeros(codes: list[tuple[str, int]]) -> int:
    curr_num = 50
    num_zeros = 0

    for code in codes:
        if code[0] == 'L':
            next_num = (curr_num - code[1]) % 100
        else:
            next_num = (curr_num + code[1]) % 100

        curr_num = next_num
        if curr_num == 0:
            num_zeros += 1
            
    return num_zeros
            

def main() -> None:
    codes = read_input('input.txt')
    print(count_zeros(codes))

if __name__ == '__main__':
    main()