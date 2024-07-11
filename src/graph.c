#include "graph.h"
#include "basic/array.h"
#include <assert.h>

void graph_init(Graph *self) {
    array_init(&self->edges);
    array_init(&self->nodes);
}

void graph_deinit(Graph *self) {
    GraphNode *it;
    array_for(&self->nodes, it) {
        array_deinit(&it->in);
        array_deinit(&it->out);
    }

    array_deinit(&self->edges);
    array_deinit(&self->nodes);
}

u32 graph_add_node(Graph *self) {
    u32 length = self->nodes.length;
    array_resize(&self->nodes, length + 1);
    GraphNode *node = &self->nodes.data[length];
    array_init(&node->in);
    array_init(&node->out);
    return length;
}

bool graph_add_edge(Graph *self, u32 source, u32 target, u16 weight) {
    // That's how we can be fast.
    // Nice to catch errors, but not required.
    // User should never provide the incorrect index.
    assert(source < self->nodes.length && "invalid source");
    assert(target < self->nodes.length && "invalid target");

    // nodes[target].in should be in sync
    // if it's not then it's users fault,
    // don't mess with internal structure.
    u32 *it;
    array_for(&self->nodes.data[source].out, it) {
        if (self->edges.data[*it].target == target)
            return true;
    }

    GraphEdge *edge = array_add(&self->edges);
    *edge = (GraphEdge){source, target, weight};
    size_t index = edge - self->edges.data;
    *array_add(&self->nodes.data[source].out) = index;
    *array_add(&self->nodes.data[target].in) = index;

    return false;
}

u32 graph_remove_edge_to(Graph *self, u32 node, u32 to) {
    array_u32 *arr = &self->nodes.data[node].out;
    u32 *it;
    array_for(arr, it) {
        u32 index = *it;
        if (self->edges.data[index].target == to) {
            array_replace_by_last(arr, it_index);
            /* Must be a variables, because arrray_replace_by_last
             * here will change the contents of *it */
            return index;
        }
    }
    return -1;
}

u32 graph_remove_edge_from(Graph *self, u32 node, u32 from) {
    array_u32 *arr = &self->nodes.data[node].in;
    u32 *it;
    array_for(arr, it) {
        u32 index = *it;
        if (self->edges.data[index].source == from) {
            array_replace_by_last(arr, it_index);
            /* Must be a variables, because arrray_replace_by_last
             * here will change the contents of *it */
            return index;
        }
    }
    return -1;
}

bool graph_replace_node_by_last(Graph *self, u32 id) {
    (void)self;
    (void)id;
    return true;
}

bool graph_del_edge(Graph *self, u32 source, u32 target) {
    u32 index = graph_remove_edge_to(self, source, target);
    u32 index2 = graph_remove_edge_from(self, target, source);
    assert(index == index2);

    if (index == (u32)-1) return false;

    array_replace_by_last(&self->edges, index);
    return true;
}

void graph_reverse_post_order(const Graph *self, u32 root, array_u32 *ordering, array_u32 *back_edges) {
    (void)self;
    (void)root;
    (void)ordering;
    (void)back_edges;
}

bool longest_path_in_acyclic_graph(const Graph *self, array_u32 *path) {
    (void)self;
    (void)path;
    return true;
}

void graph_fprint_debug(const Graph *self, FILE *stream) {
    fprintf(stream, "Graph(%zu nodes, %zu edges):\n",
            self->nodes.length, self->edges.length);

    fprintf(stream, "edges:\n");
    GraphEdge *edge;
    array_for(&self->edges, edge) {
        fprintf(stream, "[%zu] = " PRI_u32 " -> " PRI_u32 " (" PRI_u16 ")\n",
                edge_index, edge->source, edge->target, edge->weight);
    }

    fprintf(stream, "nodes:\n");
    GraphNode *node;
    array_for(&self->nodes, node) {
        u32 *it;
        fprintf(stream, "%zu  in:", node_index);
        array_for(&node->in, it) {
            GraphEdge *edge = &self->edges.data[*it];
            fprintf(stream, " [" PRI_u32 "] " PRI_u32 " (" PRI_u16 "),", *it, edge->source, edge->weight);
        }
        fprintf(stream, "\n");
        fprintf(stream, "%zu out:", node_index);
        array_for(&node->out, it) {
            GraphEdge *edge = &self->edges.data[*it];
            fprintf(stream, " [" PRI_u32 "] " PRI_u32 " (" PRI_u16 "),", *it, edge->target, edge->weight);
        }
        fprintf(stream, "\n");
    }
}
