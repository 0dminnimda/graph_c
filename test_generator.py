import sys
from pathlib import Path
from dataclasses import dataclass, field
from collections import defaultdict
from typing import IO


@dataclass
class Graph:
    nodes: list[str] = field(default_factory=list)
    edges: defaultdict[str, list[str]] = field(
        default_factory=lambda: defaultdict(list)
    )


def graph_to_instructions(stream: IO[str], g: Graph):
    for node in g.nodes:
        print(f"ADD_NODE {node}", file=stream)

    for source, targets in g.edges.items():
        for target in targets:
            print(f"ADD_EDGE {source} {target}", file=stream)


def set_root(stream: IO[str], root: str):
    print(f"ROOT {root}", file=stream)


def print_rpo(stream: IO[str]):
    print("PRINT_RPO", file=stream)


def send_end(stream: IO[str]):
    print("END", file=stream)


def complete_graph(names: list[str], self_to_self: bool) -> Graph:
    g = Graph(names)

    for name1 in names:
        for name2 in names:
            if not self_to_self and name1 == name2:
                continue

            g.edges[name1].append(name2)

    return g


def rpo_of_complete_graph(stream: IO[str], n: int):
    names = [f"n{i}" for i in range(n)]
    graph = complete_graph(names, False)
    graph_to_instructions(stream, graph)
    set_root(stream, names[0])
    print_rpo(stream)


out_path = Path(sys.argv[1])
names = [chr(c) for c in range(ord('a'), ord('z') + 1)]

with out_path.open("w") as f:
    rpo_of_complete_graph(f, 5)
    send_end(f)

