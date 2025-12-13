from pprint import pprint


type Coordinate = tuple[int, int]

def read_input(file_name: str) -> list[Coordinate]:
    coordinates: list[Coordinate] = []
    with open(file_name, "r") as f:
        for line in f.readlines():
            coordinate: Coordinate = tuple(
                map(int, line.strip().split(","))
            )  # pyright: ignore[reportAssignmentType]
            coordinates.append(coordinate)

    return coordinates

def area_covered(p1: Coordinate, p2: Coordinate) -> int:
    x1, y1 = p1
    x2, y2 = p2
    return (abs(x2-x1)+1)*(abs(y2-y1)+1)

def get_max_area(coordinates: list[Coordinate]) -> int:
    max_area: int = 0

    for i in range(len(coordinates)-1):
        for j in range(i+1, len(coordinates)):
            area = area_covered(coordinates[i], coordinates[j])
            if area > max_area:
                max_area = area


    return max_area

def main() -> None:
    coordinates = read_input("9/input.txt")
    print(f"Max Area: {get_max_area(coordinates)}")

if __name__ == "__main__":
    main()
