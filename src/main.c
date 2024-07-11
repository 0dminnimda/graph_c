#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "basic/str_to_int.h"
#include "basic/macros.h"
#include "basic/types.h"
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

        if (name.length == 0) {
            printf(ERROR("You must provide a name of the node\n"));
            return false;
        }

        if (args.length != 0) {
            printf(WARNING("Junk in the back is ignored ('" PRI_str "')\n"), FMT_str(&args));
        }

        size_t index;
        if (names_insert(&ctx->names, &name, &index)) {
            printf(WARNING("node '" PRI_str "' already exists\n"), FMT_str(&name));
        } else {
            u32 id = graph_add_node(&ctx->graph);
            if (index != id) {
                printf(ERROR("name index (%zu) does not match graph id (" PRI_u32 ")\n"
                            "This is a bug in the program\n"), index, id);
                return true;
            }
        }

        if (ctx->in_debug) {
            printf(DEBUG("Name: '" PRI_str "', Index: %zu\n"), FMT_str(&name), index);
        }
    } else if (str_compare(&command, &str_lit("ADD_EDGE")) == 0) {
        /* ADD_EDGE <name1> <name2> [<weight>] */

        str name1, name2, weight_s;
        /* We don't want to have spaces in names. */
        str_partition_whitespace(&args, &name1, &args);
        str_partition_whitespace(&args, &name2, &weight_s);

        if (name1.length == 0 || name2.length == 0) {
            printf(ERROR("You must provide two node names, got %d\n"),
                   (name1.length? 1:0) + (name2.length? 1:0));
            return false;
        }

        size_t index1, index2;
        if (!names_find(&ctx->names, &name1, &index1)) {
            printf(ERROR("First name '" PRI_str "' does not exist\n"), FMT_str(&name1));
            return false;
        }
        if (!names_find(&ctx->names, &name2, &index2)) {
            printf(ERROR("Second name '" PRI_str "' does not exist\n"), FMT_str(&name2));
            return false;
        }

        u16 weight = 1;
        if (weight_s.length != 0) {
            S2I_Result parse_result = str_to_u16(&weight_s, &weight);
            if (parse_result != S2I_OK) {
                if (parse_result == S2I_OUT_OF_RANGE) {
                    printf(ERROR("weight must be in range [0, " STR(U16_MAX) "]\n"));
                } else {
                    printf(ERROR("failed to parse weight '" PRI_str "' as a number\n"), FMT_str(&weight_s));
                }
                return false;
            }
        }

        if (weight_s.length != 0) {
            printf(WARNING("Junk in the back is ignored ('" PRI_str "')\n"), FMT_str(&weight_s));
        }

        if (graph_add_edge(&ctx->graph, (u32)index1, (u32)index2, weight)) {
            printf(WARNING("edge between nodes '" PRI_str "' and '" PRI_str "' already exists\n"),
                       FMT_str(&name1), FMT_str(&name2));
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index1: %zu, Index2: %zu, Weight: " PRI_u16 "\n"),
                   index1, index2, weight);
        }
    } else {
        printf(ERROR("Unknown command '" PRI_str "'\n"), FMT_str(&command));
        return false;
    }

    if (ctx->in_debug) {
        printf(DEBUG("Current graph state:\n"));
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
