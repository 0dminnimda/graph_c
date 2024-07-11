#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"
#include "basic/str_builder.h"
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
    u32 root_node;
} Context;

typedef enum {
    OK,
    FATAL_ERROR,
    NICE_STOP
} Result;

typedef enum {
    INVALID,
    EMPTY,
    END,
    DEBUG,
    ADD_NODE,
    ADD_EDGE,
    REMOVE_NODE,
    REMOVE_EDGE,
    ROOT,
} Command;

#define CMD_CASE(n) if (str_compare(string, &str_lit(STR(n))) == 0) return n
Command str_to_command(const str *string) {
    CMD_CASE(END);
    CMD_CASE(DEBUG);
    CMD_CASE(ADD_NODE);
    CMD_CASE(ADD_EDGE);
    CMD_CASE(REMOVE_NODE);
    CMD_CASE(REMOVE_EDGE);
    CMD_CASE(ROOT);
    return INVALID;
}
#undef CMD_CASE

bool str_to_node(Context *ctx, str *string, str *name, u8 arg_no, bool can_create, u32 *id, Result *result) {
    /* We don't want to have spaces in names. */
    str_partition_whitespace(string, name, string);

    if (name->length == 0) {
        printf(ERROR("You must provide a name of the node #" PRI_u8 "\n"), arg_no);
        *result = OK;
        return true;
    }

    size_t index;
    if (can_create) {
        if (names_insert(&ctx->names, name, &index)) {
            printf(WARNING("Node '" PRI_str "' already exists\n"), FMT_str(name));
        } else {
            u32 id = graph_add_node(&ctx->graph);
            if (index != id) {
                printf(ERROR("Name index (%zu) does not match graph id (" PRI_u32 ")\n"
                             "This is a bug in the program\n"), index, id);
                *result = FATAL_ERROR;
                return true;
            }
        }
    } else {
        if (!names_find(&ctx->names, name, &index)) {
            printf(ERROR("Node #" PRI_u8 " '" PRI_str "' does not exist\n"), arg_no, FMT_str(name));
            *result = OK;
            return true;
        }
    }

    *id = (u32)index;
    return false;
}

Result handle_command(Context *ctx, Command cmd, str args) {
    Result result;

    if (cmd == EMPTY) {
        /* If in debug mode let's print current state. */
        if (ctx->in_debug) {
            printf(DEBUG("Current graph state:\n"));
            graph_fprint_debug(&ctx->graph, stdout);
        }

        /* Just skip an empty line */
        return OK;
    } else if (cmd == END) {
        return NICE_STOP;
    } else if (cmd == DEBUG) {
        ctx->in_debug = !ctx->in_debug;
        printf("Debug mode: turned %s\n" CLEAR, ctx->in_debug? GREEN "on" : RED "off");
        return OK;
    } else if (cmd == ADD_NODE) {
        /* ADD_NODE <name> */

        u32 id;
        str name;
        if (str_to_node(ctx, &args, &name, 1, true, &id, &result)) {
            return result;
        }

        if (ctx->in_debug) {
            printf(DEBUG("Name: '" PRI_str "', Index: " PRI_u32 "\n"), FMT_str(&name), id);
        }
    } else if (cmd == ADD_EDGE) {
        /* ADD_EDGE <name1> <name2> [<weight>] */

        u32 id1, id2;
        str name1, name2;
        if (str_to_node(ctx, &args, &name1, 1, false, &id1, &result)) {
            return result;
        }
        if (str_to_node(ctx, &args, &name2, 2, false, &id2, &result)) {
            return result;
        }

        u16 weight = 1;
        if (args.length != 0) {
            S2I_Result parse_result = str_to_u16(&args, &weight);
            if (parse_result != S2I_OK) {
                if (parse_result == S2I_OUT_OF_RANGE) {
                    printf(ERROR("weight must be in range [0, " STR(U16_MAX) "]\n"));
                } else {
                    printf(ERROR("failed to parse weight '" PRI_str "' as a number\n"), FMT_str(&args));
                }
                return OK;
            }
        }

        if (graph_add_edge(&ctx->graph, id1, id2, weight)) {
            printf(WARNING("edge between nodes '" PRI_str "' and '" PRI_str "' already exists\n"),
                   FMT_str(&name1), FMT_str(&name2));
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index1: " PRI_u32 ", Index2: " PRI_u32 ", Weight: " PRI_u16 "\n"),
                   id1, id2, weight);
        }
    } else if (cmd == REMOVE_NODE) {
        /* REMOVE_NODE <name> */
    } else if (cmd == REMOVE_EDGE) {
        /* REMOVE_EDGE <name1> <name2> */

        // printf("deleted: %s\n", graph_del_edge(&ctx.graph, n1, n2)? "failure": "success");
    } else if (cmd == ROOT) {
        /* ROOT <name> */

        str name;
        if (str_to_node(ctx, &args, &name, 1, false, &ctx->root_node, &result)) {
            return result;
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index: " PRI_u32 "\n"), ctx->root_node);
        }
    }

    if (args.length != 0) {
        printf(WARNING("Junk in the back is ignored ('" PRI_str "')\n"), FMT_str(&args));
    }

    return OK;
}

Result handle_line(Context *ctx, str line) {
    Command cmd;

    str stripped = str_strip_whitespaces(&line);
    if (stripped.length == 0) {
        return handle_command(ctx, EMPTY, stripped);
    }

    str command, args;
    str_partition_whitespace(&stripped, &command, &args);

    cmd = str_to_command(&command);
    if (cmd == INVALID) {
        printf(ERROR("Unknown command '" PRI_str "'\n"), FMT_str(&command));
        return OK;
    }

    return handle_command(ctx, cmd, args);
}

int main(void) {
    printf("Welcome to example graph program!\n");

    Context ctx;
    ctx.in_debug = false;
    ctx.root_node = -1;

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

    StrBuilder sb;
    array_init(&sb);

    while (1) {
        printf("\n> ");
        fflush(stdout);

        sb.length = 0;  /* No need to collect all inputs, only this one. */
        read_line_use_buffer(&sb);
        str line = str_builder_get(&sb);

        if (line.data == NULL) {
            fprintf(stderr, "Error while reading the line\n");
            break;
        }

        Result res = handle_line(&ctx, line);

        if (res == FATAL_ERROR) {
            fprintf(stderr, "Fatal error occured, stopping.\n");
            break;
        } else if (res == NICE_STOP) {
            break;
        }
    }

    array_deinit(&sb);

    graph_deinit(&ctx.graph);
    names_deinit(&ctx.names);

    return 0;
}
