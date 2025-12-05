# Pre computed factors
factors: dict[int, tuple] = {
    1: (), # single digits never repeat
    2: (1,),
    3: (1,),
    4: (1, 2),
    5: (1,),
    6: (1, 2, 3),
    7: (1,),
    8: (1, 2, 4),
    9: (1, 3),
    10: (1, 2, 5)
}

def is_invalid(id: int) -> bool:
    id = str(id)
    len_id = len(id)

    flag = False
    for factor in factors[len_id]: #O(k)
        for p in range(0, len_id - factor, factor): # O(d/k)
            l = id[p:p+factor]
            r = id[p+factor:p+2*factor]
            if l != r:
                break
        else:
            flag=True
            break
        
    return flag


def sum_invalid_ids(id_ranges: list[tuple[int, int]]) -> int:
    sum_invalid_ids = 0

    for (start_id, end_id) in id_ranges: # O(N)
        for id in range(start_id, end_id+1): # O(M)
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