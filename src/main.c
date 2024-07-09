#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "graph.h"

typedef struct {
    bool debug;
} Config;

void handle_operation(Graph *g, Config *conf, str *line) {
    if (str_compare(line, &str_lit("DEBUG")) == 0) {
        conf->debug = !conf->debug;
        return;
    }

    str stripped = str_strip_whitespaces(line);

    printf("Stripped '" PRI_str "'\n", FMT_str(&stripped));

    str command, args;
    str_partition_whitespace(&stripped, &command, &args);

    printf("Partitioned '" PRI_str "', '" PRI_str "'\n", FMT_str(&command), FMT_str(&args));

    if (str_compare(&command, &str_lit("ADD_NODE")) == 0) {
        (void)g;
        printf("Got add_node with args '" PRI_str "'\n", FMT_str(&args));
    } else {
        printf("Unknown command '" PRI_str "'\n", FMT_str(&command));
    }
}

int main(void) {
    printf("Hello seamen\n");

    Config conf = {0};

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

    printf("deleted: %s\n", graph_del_edge(&g, n1, n2)? "failure": "success");

    graph_fprint_debug(&g, stdout);

    while (1) {
        str line = read_line();
        if (line.data == NULL || line.length == 0) {
            fprintf(stderr, "Error while reading the line\n");
            str_deinit(&line);
            return 1;
        }

        if (str_compare(&line, &str_lit("END")) == 0) {
            str_deinit(&line);
            break;
        }

        handle_operation(&g, &conf, &line);

        str_deinit(&line);
    }

    graph_deinit(&g);

    return 0;
}
