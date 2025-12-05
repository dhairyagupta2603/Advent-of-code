def read_input(file_name: str) -> tuple[list[tuple[int, int]], list[int]]:
    id_ranges: list[tuple[int, int]] = []
    ids: list[int] = []
    with open(file_name, "r") as f:
        lines = f.readlines()
        pos = 0
        while pos < len(lines):
            line = lines[pos].strip('\n')
            pos+=1
            if line == '':
                break

            s, e = map(int, line.split('-'))
            id_ranges.append((s, e))

        for line in lines[pos:]:
            line = line.strip('\n')
            ids.append(int(line))

    return id_ranges, ids

def count_fresh_ids(id_ranges: list[tuple[int, int]], ids: list[int]) -> int:
    num_fresh: int = 0
    for id in ids:
        for s, e in id_ranges:
            if s <= id and id <= e:
                num_fresh += 1
                break

    return num_fresh


def main() -> None:
    id_ranges, ids = read_input("5/input.txt")
    print(count_fresh_ids(id_ranges, ids))


if __name__ == "__main__":
    main()
