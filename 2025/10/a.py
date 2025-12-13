import re
from collections import deque

class Machine:
    def __init__(self) -> None:
        self.target_indicators: list[bool] = []
        self.button_wirings: list[list[int]] = []
        self.joltage_reqs: list[int] = []


def read_input(file_name: str) -> list[Machine]:
    machines: list[Machine] = []
    with open(file_name, "r") as f:
        for line in f.readlines():
            line = line.strip('\n')
            machine = Machine()

            if indicators:= re.search(r"\[(\.|\#)+\]", line):
                indicators = indicators.group()[1:-1]
                machine.target_indicators = [True if indicator == '#' else False for indicator in indicators]

            if wirings:= re.findall(r"\([0-9,]+\)", line):
                for wiring in wirings:
                    machine.button_wirings.append(list(map(int, wiring[1:-1].split(","))))

            if joltages := re.search(r"\{[0-9,]+\}", line):
                machine.joltage_reqs = list(map(int, joltages.group()[1:-1].split(",")))
            
            machines.append(machine)
    return machines

def count_presses(machine: Machine) -> int:
    q: deque[tuple[list[bool], int]] = deque()
    q.append(([False]*len(machine.target_indicators), 0))
    visited = set()
    min_steps = 0
    while len(q) > 0:
        curr_indicators, step = q.popleft()

        if curr_indicators == machine.target_indicators:
            min_steps = step
            break

        for wiring in machine.button_wirings:
            next_indicators = [ind for ind in curr_indicators]
            for wire in wiring:
                next_indicators[wire] = not next_indicators[wire]

            ni = ''.join(map(lambda x: str(int(x)), next_indicators))
            if ni not in visited:
                visited.add(ni)
                q.append((next_indicators, step+1))

    return min_steps


def main() -> None:
    machines = read_input("10/input.txt")

    presses: int = 0
    for machine in machines:
        presses += count_presses(machine)

    print(f"Total button presses: {presses}")

if __name__ == "__main__":
    main()
