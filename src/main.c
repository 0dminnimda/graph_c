#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "graph.h"

int main(void) {
    printf("Hello seamen\n");

    Graph g;
    graph_init(&g);

    graph_fprint_debug(&g, stdout);

    u32 n1 = graph_add_node(&g);

    graph_fprint_debug(&g, stdout);

    u32 n2 = graph_add_node(&g);

    graph_fprint_debug(&g, stdout);

    graph_add_edge(&g, n1, n2, 20);

    graph_fprint_debug(&g, stdout);

    graph_add_edge(&g, n2, n1, 30);

    graph_fprint_debug(&g, stdout);

    printf("deleted: %s\n", graph_del_edge(&g, n2, n2)? "success": "failure");

    graph_fprint_debug(&g, stdout);

    graph_deinit(&g);

    while (1) {
        str line = read_line();
        if (line.data == NULL || line.length == 0) {
            fprintf(stderr, "Error while reading the line\n");
            str_deinit(&line);
            return 1;
        }

        if (str_compare(&line, &str_lit("q")) == 0) {
            str_deinit(&line);
            break;
        }

        printf("Got string " PRI_str "\n", FMT_str(&line));

        str_deinit(&line);
    }

    return 0;
}
