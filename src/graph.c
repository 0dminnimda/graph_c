#include "graph.h"

#include <assert.h>

#include "basic/array.h"
#include "basic/type_operations.h"

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
    /* That's how we can be fast.
     * Nice to catch errors, but not required.
     * User should never provide the incorrect index. */
    assert(source < self->nodes.length && "invalid source");
    assert(target < self->nodes.length && "invalid target");

    /* nodes[target].in should be in sync
     * if it's not then it's users fault,
     * don't mess with internal structure. */
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

void graph_replace_edge_by_last(Graph *self, u32 index) {
    array_replace_by_last(&self->edges, index);
    u32 old_index = self->edges.length;
    GraphEdge *edge = &self->edges.data[index];

    u32 *it;
    array_for(&self->nodes.data[edge->source].out, it) {
        if (*it == old_index) {
            *it = index;
            break;
        }
    }
    array_for(&self->nodes.data[edge->target].in, it) {
        if (*it == old_index) {
            *it = index;
            break;
        }
    }
}

void graph_replace_node_by_last(Graph *self, u32 index) {
    array_replace_by_last(&self->nodes, index);
    GraphNode *node = &self->nodes.data[index];

    u32 *it;
    array_for(&node->out, it) {
        self->edges.data[*it].source = index;
    }
    array_for(&node->in, it) {
        self->edges.data[*it].target = index;
    }
}

void graph_del_node_and_replace_by_last(Graph *self, u32 id) {
    /* Same reasoning as in graph_add_edge */
    assert(id < self->nodes.length && "invalid id");

    /* Delete all indices to the edges in the corrected nodes.
     * But leave (one) self to self node, so it would be renamed,
     * and there's no point to remove it from 'other' node,
     * since there's no 'other'. */
    u32 *it;
    array_u32 *arr;

    arr = &self->nodes.data[id].out;
    array_for(arr, it) {
        graph_remove_edge_from(self, self->edges.data[*it].target, id);
    }

    arr = &self->nodes.data[id].in;
    array_for(arr, it) {
        graph_remove_edge_to(self, self->edges.data[*it].source, id);
    }

    /* To remove the edges from the edge array,
     * the edges that will take place of the old ones must have
     * their indices in the nodes be renamed. */

    /* XXX: why does this not work?
     * array_extend_from(arr, &self->nodes.data[id].out); */
    array_for(&self->nodes.data[id].out, it) {
        *array_add(arr) = *it;
    }

    /* The latest indices must be removed first
     * not to cause issues for the later removals. */
    array_sort(arr, u32_compare_reversed);

    /* This may do extra renames which could have been avoided,
     * but for now it'll do and everything else I come up with
     * just seems more compilated. */
    array_for(arr, it) {
        graph_replace_edge_by_last(self, *it);
    }

    array_deinit(&self->nodes.data[id].out);
    array_deinit(&self->nodes.data[id].in);

    graph_replace_node_by_last(self, id);
}

bool graph_del_edge(Graph *self, u32 source, u32 target) {
    u32 index = graph_remove_edge_to(self, source, target);
    u32 index2 = graph_remove_edge_from(self, target, source);
    assert(index == index2);

    if (index == (u32)-1) return false;

    graph_replace_edge_by_last(self, index);
    return true;
}

typedef enum {
    RPO_NEW,
    RPO_SEEN,
    RPO_DONE,
} RPO_State;

void graph_reverse_post_order(const Graph *self, u32 root, array_u32 *ordering, array_u32 *back_edges) {
    /* Same reasoning as in graph_add_edge */
    assert(root < self->nodes.length && "invalid root");

    s64 length = self->nodes.length;

    array(RPO_State) states;
    array_init_with_capacity_and_length(&states, length, length);
    array_init_with_capacity(ordering, length);
    array_init(back_edges);

    array(s64) stack;
    /* Large upper bound. Once for all of the nodes,
     * second time for all the post order nodes,
     * and third for possible repeating nodes from loops. */
    array_init_with_capacity(&stack, length * 3);

    RPO_State *state;
    array_for(&states, state) {
        *state = RPO_NEW;
    }

    *array_add(&stack) = (s64)root;

    while (stack.length) {
        s64 id = array_pop(&stack);

        if (id < 0) {
            /* Post order visit. */
            u32 actual = (u32)(id + length);
            *array_add(ordering) = actual;
            states.data[actual] = RPO_DONE;
            continue;
        }

        /* Will be popped after all children are visited
         * thus post order. */
        *array_add(&stack) = id - length;
        states.data[id] = RPO_SEEN;

        u32 *edge;
        array_for(&self->nodes.data[id].out, edge) {
            u32 child = self->edges.data[*edge].target;
            if (states.data[child] == RPO_SEEN) {
                *array_add(back_edges) = *edge;
            } else if (states.data[child] == RPO_NEW) {
                *array_add(&stack) = (s64)child;
            }
        }
    }

    array_reverse(ordering, u32);
    array_deinit(&stack);
    array_deinit(&states);
}


bool longest_path_in_acyclic_graph(const Graph *self, array_u32 *path) {
/*
    u32 length = self->nodes.length;

    array(u32) stack;
    array_init_with_capacity(&stack, length);

    array(u32) distances;
    array_init_with_capacity(&distances, length);

    array(bool) visited;
    array_init_with_capacity_and_length(&visited, length, length);

    for (size_t i = 0; i < length; ++i) {
        visited.data[i] = false;
    }

    for (int i = 0; i < V; i++) {
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);
    }

    for (int i = 0; i < V; i++)
        distances[i] = NINF;
    distances[s] = 0;

    while (Stack.empty() == false) {
        // Get the next vertex from topological order
        int u = Stack.top();
        Stack.pop();

        // Update distances of all adjacent vertices
        list<AdjListNode>::iterator i;
        if (distances[u] != NINF) {
            for (i = adj[u].begin(); i != adj[u].end(); ++i){

                if (distances[i->getV()] < distances[u] + i->getWeight())
                    distances[i->getV()] = distances[u] + i->getWeight();
            }
        }
    }

    for (int i = 0; i < V; i++)
        (distances[i] == NINF) ? cout << "INF " : cout << distances[i] << " ";

    delete [] visited;
*/

    if (self == NULL || path == NULL || self->nodes.length == 0) {
        return false;
    }

    /* Initialize arrays for storing distances and predecessors */
    array_u32 distances;
    array_u32 predecessors;
    array_init(&distances);
    array_init(&predecessors);

    /* Initialize distances and predecessors */
    for (u32 i = 0; i < self->nodes.length; i++) {
        *array_add(&distances) = 0;
        *array_add(&predecessors) = U32_MAX;
    }

    /* Topological sort */
    array_u32 topo_order;
    array_u32 back_edges;
    array_init(&topo_order);
    array_init(&back_edges);
    graph_reverse_post_order(self, 0, &topo_order, &back_edges);

    /* If there are back edges, the graph is not acyclic */
    if (back_edges.length > 0) {
        array_deinit(&distances);
        array_deinit(&predecessors);
        array_deinit(&topo_order);
        array_deinit(&back_edges);
        return false;
    }

    u32 max_dist = 0;
    u32 end_node = 0;

    /* Compute longest path */
    for (u32 i = 0; i < topo_order.length; i++) {
        u32 node = topo_order.data[i];
        u32 dist = distances.data[node];

        if (dist > max_dist) {
            max_dist = dist;
            end_node = node;
        }

        const GraphNode *gnode = &self->nodes.data[node];
        for (u32 j = 0; j < gnode->out.length; j++) {
            u32 edge_idx = gnode->out.data[j];
            const GraphEdge *edge = &self->edges.data[edge_idx];
            u32 new_dist = dist + edge->weight;

            if (new_dist > distances.data[edge->target]) {
                distances.data[edge->target] = new_dist;
                predecessors.data[edge->target] = node;
            }
        }
    }

    /* Reconstruct the critical path */
    array_deinit(path);
    array_init(path);
    u32 current = end_node;
    while (current != U32_MAX) {
        *array_add(path) = current;
        current = predecessors.data[current];
    }

    /* Reverse the path (it's currently in reverse order) */
    array_reverse(path, u32);

    /* Clean up */
    array_deinit(&distances);
    array_deinit(&predecessors);
    array_deinit(&topo_order);
    array_deinit(&back_edges);

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
