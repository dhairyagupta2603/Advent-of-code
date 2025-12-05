def is_invalid(id: int) -> bool:
    id = str(id)
    s = len(id)

    if s%2 != 0: 
        return False

    return id[:s//2] == id[s//2:]

def sum_invalid_ids(id_ranges: list[tuple[int, int]]) -> int:
    sum_invalid_ids = 0

    for (start_id, end_id) in id_ranges:
        for id in range(start_id, end_id+1):
            if is_invalid(id):
                sum_invalid_ids += id

    return sum_invalid_ids


def read_input(file_name: str) -> list[tuple[int, int]]:
    with open(file_name, 'r') as f:
        id_ranges = list(map(lambda x: tuple(map(lambda y: int(y), x.split("-"))), f.read().split(',')))

    return id_ranges


def main() -> None:
    id_ranges = read_input('2/input.txt')
    print(sum_invalid_ids(id_ranges))

if __name__ == '__main__':
    main()