def read_input(file_name: str) -> list[tuple[int, int]]:
    id_ranges: list[tuple[int, int]] = []
    with open(file_name, "r") as f:
        for line in f.readlines():
            line = line.strip('\n')
            if line == '':
                break

            s, e = map(int, line.split('-'))
            id_ranges.append((s, e))


    return id_ranges

def count_fresh_ids(id_ranges: list[tuple[int, int]]) -> int:
    id_ranges = sorted(id_ranges, key=lambda range: range[0])

    comb_ranges: list[tuple[int, int]] = []
    for id_range in id_ranges:
        if len(comb_ranges) <= 0 or comb_ranges[-1][1] < id_range[0]:
            comb_ranges.append(id_range)
            continue

        comb_ranges[-1] = (comb_ranges[-1][0], max(id_range[1], comb_ranges[-1][1]))

    return sum(map(lambda x: x[1]-x[0]+1, comb_ranges))


def main() -> None:
    id_ranges = read_input("5/input.txt")
    print(count_fresh_ids(id_ranges))


if __name__ == "__main__":
    main()
