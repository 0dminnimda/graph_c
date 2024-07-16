# Graph Data Structure in C

Simple api, see [graph.h](./src/graph.h)

Also contains usefull simple suit of file, see [basic](./src/basic/)

## About

Written with presumption that the degree of nodes is not large, this mostly affects removal of the nodes.

Nodes are u32 integers, this way graph implementation does not depend on the actual type of the node, thus you have to manage an array of actual nodes.

Operations:

- Add/remove a node or an edge.
- Find reverse post order (iterative, not reursive)
- Find critical path (longest or shortest)

## Build

```shell
make
```

## Play

It also comes with a simple command line wrapper you can play with.

```shell
./main.out
```

## Run tests

```shell
make tests
```

