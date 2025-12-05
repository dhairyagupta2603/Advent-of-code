def read_input(file_name: str) -> list[list[int]]:
    battery_banks: list[list[int]] = []
    with open(file_name, 'r') as f:
        for line in f.readlines():
            battery_banks.append(list(map(int, line.strip('\n'))))

    return battery_banks


def sum_joltage_accross_banks(banks: list[list[int]]) -> int:
    overall_joltage: int = 0
    for bank in banks:
        # get position of highest joltage 
        max_joltage = 0
        max_joltage_pos = -1
        for pos, joltage in enumerate(bank):
            if max_joltage < joltage:
                max_joltage = joltage
                max_joltage_pos = pos

        # split bank into two arrays - left and right 
        left_comb, right_comb = max_joltage, max_joltage
        if max_joltage_pos > 0:
            left_highest = max(bank[:max_joltage_pos])
            left_comb = int(f'{left_highest}{max_joltage}')
        if max_joltage_pos < len(bank)-1:
            right_highest = max(bank[max_joltage_pos+1:])
            right_comb = int(f'{max_joltage}{right_highest}')

        max_comb = max(left_comb, right_comb)
        overall_joltage += max_comb

    return overall_joltage

def main() -> None:
    battery_banks = read_input('3/input.txt')
    print(sum_joltage_accross_banks(battery_banks))

if __name__ == '__main__':
    main()
