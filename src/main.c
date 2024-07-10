#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "graph.h"
#include "names.h"

typedef struct {
    Graph graph;
    Names names;
    bool in_debug;
} Context;

void handle_operation(Context *ctx, str *line) {
    if (str_compare(line, &str_lit("DEBUG")) == 0) {
        ctx->in_debug = !ctx->in_debug;
        printf("Debug mode: turned %s\n", ctx->in_debug? "on" : "off");
        return;
    }

    str stripped = str_strip_whitespaces(line);

    str command, args;
    str_partition_whitespace(&stripped, &command, &args);

    if (str_compare(&command, &str_lit("ADD_NODE")) == 0) {
        str node_name;
        str_partition_whitespace(&args, &node_name, &args);

        u32 index = names_insert(&ctx->names, &node_name) - ctx->names.data;
        if (ctx->in_debug) {
            printf("Name: '" PRI_str "', Index: " PRI_u32 "\n", FMT_str(&node_name), index);
        }
    } else {
        printf("Error: Unknown command '" PRI_str "'\n", FMT_str(&command));
    }
}

int main(void) {
    printf("Hello seamen\n");

    Context ctx;
    ctx.in_debug = false;

    array_init(&ctx.names);

    graph_init(&ctx.graph);

    graph_fprint_debug(&ctx.graph, stdout);

    u32 n1 = graph_add_node(&ctx.graph);

    graph_fprint_debug(&ctx.graph, stdout);

    u32 n2 = graph_add_node(&ctx.graph);

    graph_fprint_debug(&ctx.graph, stdout);

    graph_add_edge(&ctx.graph, n1, n2, 20);

    graph_fprint_debug(&ctx.graph, stdout);

    graph_add_edge(&ctx.graph, n2, n1, 30);

    graph_fprint_debug(&ctx.graph, stdout);

    printf("deleted: %s\n", graph_del_edge(&ctx.graph, n1, n2)? "failure": "success");

    graph_fprint_debug(&ctx.graph, stdout);

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

        handle_operation(&ctx, &line);

        str_deinit(&line);
    }

    graph_deinit(&ctx.graph);

    names_deinit(&ctx.names);

    return 0;
}
