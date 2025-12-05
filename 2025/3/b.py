def read_input(file_name: str) -> list[list[int]]:
    battery_banks: list[list[int]] = []
    with open(file_name, 'r') as f:
        for line in f.readlines():
            battery_banks.append(list(map(int, line.strip('\n'))))

    return battery_banks

def sum_joltage_accross_banks(banks: list[list[int]]) -> int:
    overall_joltage: int = 0
    for bank in banks:
        to_remove: int = len(bank) - 12
        joltages: list[int] = []
        for joltage in bank:
            while len(joltages) > 0 and to_remove > 0 and joltages[-1] < joltage:
                joltages.pop()
                to_remove -= 1

            joltages.append(joltage)

        joltages = joltages[:12]
        overall_joltage += int(''.join(map(str, joltages)))

    return overall_joltage

def main() -> None:
    battery_banks = read_input('3/input.txt')
    print(sum_joltage_accross_banks(battery_banks))

if __name__ == '__main__':
    main()
