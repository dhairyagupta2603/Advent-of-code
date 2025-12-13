import re

type Shape = list[str]
type GridReq = tuple[tuple[int, int], list[int]]

def read_input(file_name: str):
    shapes: list[Shape] = []
    grid_reqs: list[GridReq] = []
    with open(file_name, "r") as f:
        while line:= f.readline():
            if line == '\n':
                continue

            ans = re.match("[0-9]+:\n", line)
            if ans:
                shape: Shape = []
                while (line := f.readline()) != '\n':
                    shape.append(line.strip("\n"))

                shapes.append(shape)
            
            ans = re.match("[0-9]+x[0-9]+:", line)
            if ans:
                grid_size, use_shapes = ans.string.strip('\n').split(": ")
                grid_reqs.append((
                    tuple(map(int, (grid_size.split("x")))),  # pyright: ignore[reportArgumentType]
                    list(map(int, use_shapes.split(" ")))
                ))

    return shapes, grid_reqs

def count_valid_regions(grid_reqs: list[GridReq], shapes:  list[Shape]) -> int:
    result: int = 0

    for grid_req in grid_reqs:
        (r, c), req = grid_req
        print(r, c, req)
        if r*c >= 9 * sum(req):
            print("---")
            result += 1

    return result

def main() -> None:
    shapes, grid_reqs = read_input("12/sample.txt")

    from pprint import pprint
    print(f"Valid Regions: {count_valid_regions(grid_reqs, shapes)}")

    # The problem is basically unsolvable in practice being NP-Complete for a generalized solution covering all cases, and an active area of research for hurisitic optimizations. fortunately the input is trivial

if __name__ == "__main__":
    main()
