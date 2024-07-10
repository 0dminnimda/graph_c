#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "graph.h"
#include "names.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CLEAR "\033[0m"

#define ERROR(msg) RED "ERROR: " msg CLEAR
#define WARNING(msg) YELLOW "WARNING: " msg CLEAR
#define DEBUG(msg) GREEN "DEBUG: " msg CLEAR

typedef struct {
    Graph graph;
    Names names;
    bool in_debug;
} Context;

bool handle_operation(Context *ctx, str *line) {
    if (str_compare(line, &str_lit("DEBUG")) == 0) {
        ctx->in_debug = !ctx->in_debug;
        printf("Debug mode: turned %s\n" CLEAR, ctx->in_debug? GREEN "on" : RED "off");
        return false;
    }

    str stripped = str_strip_whitespaces(line);

    str command, args;
    str_partition_whitespace(&stripped, &command, &args);

    if (str_compare(&command, &str_lit("ADD_NODE")) == 0) {
        /* ADD_NODE <name> */

        str name;
        /* We don't want to have spaces in names. */
        str_partition_whitespace(&args, &name, &args);

        size_t index;
        if (names_insert(&ctx->names, &name, &index)) {
            printf(WARNING("node '" PRI_str "' already exists\n"), FMT_str(&name));
        } else {
            u32 id = graph_add_node(&ctx->graph);
            if (index != id) {
                printf(ERROR("name index (%zu) does not match graph id (" PRI_u32 ")\n"), index, id);
                return true;
            }
        }

        if (ctx->in_debug) {
            printf(DEBUG("Name: '" PRI_str "', Index: %zu\n"), FMT_str(&name), index);
        }
    } else {
        printf(ERROR("Unknown command '" PRI_str "'\n"), FMT_str(&command));
        return false;
    }

    if (ctx->in_debug) {
        printf(DEBUG("current graph state:\n"));
        graph_fprint_debug(&ctx->graph, stdout);
    }

    return false;
}

int main(void) {
    printf("Welcome to example graph program!\n");

    Context ctx;
    ctx.in_debug = false;

    array_init(&ctx.names);
    graph_init(&ctx.graph);

/*
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
*/

    while (1) {
        printf("\n> ");
        fflush(stdout);

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

        bool got_error = handle_operation(&ctx, &line);

        str_deinit(&line);

        if (got_error) {
            fprintf(stderr, "Fatal error occured, stopping.\n");
            break;
        }
    }

    graph_deinit(&ctx.graph);
    names_deinit(&ctx.names);

    return 0;
}
