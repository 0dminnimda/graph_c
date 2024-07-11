#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include "basic/types.h"
#include "basic/array.h"

typedef array(u32) array_u32;

typedef struct {
    u32 source;
    u32 target;
    u16 weight;
} GraphEdge;

typedef struct {
    array_u32 in;
    array_u32 out;
} GraphNode;

typedef struct {
    array(GraphEdge) edges;
    array(GraphNode) nodes;
} Graph;

void graph_init(Graph *self);
void graph_deinit(Graph *self);
u32 graph_add_node(Graph *self);
bool graph_add_edge(Graph *self, u32 source, u32 target, u16 weight);
void graph_del_node_and_replace_by_last(Graph *self, u32 id);
bool graph_del_edge(Graph *self, u32 source, u32 target);
void graph_reverse_post_order(const Graph *self, u32 root, array_u32 *ordering, array_u32 *back_edges);
bool longest_path_in_acyclic_graph(const Graph *self, array_u32 *path);
void graph_fprint_debug(const Graph *self, FILE *stream);

#endif  // GRAPH_H
