#include <stdio.h>

#include "basic/array.h"
#include "basic/read_line.h"
#include "basic/str.h"
#include "basic/str_builder.h"
#include "basic/str_to_int.h"
#include "basic/macros.h"
#include "basic/types.h"
#include "basic/main.h"
#include "graph.h"
#include "names.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CLEAR "\033[0m"

#define PAINT(color, msg) (ctx->glow? color msg CLEAR : msg)

#define ERROR(msg) (ctx->ugly_ui? msg : PAINT(RED, "ERROR: " msg))
#define WARNING(msg) (ctx->ugly_ui? msg : PAINT(YELLOW, "WARNING: " msg))
#define DEBUG(msg) (ctx->ugly_ui? msg : PAINT(GREEN, "DEBUG: " msg))

typedef struct {
    Graph graph;
    Names names;
    bool in_debug;
    bool glow;
    bool ugly_ui;
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
    PRINT_RPO,
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
    CMD_CASE(PRINT_RPO);
    return INVALID;
}
#undef CMD_CASE

#define NODE_EXISTS (ctx->ugly_ui? \
    "Node \"" PRI_str "\" is already exist\n": \
    WARNING("Node '" PRI_str "' already exists\n"))

#define MISSING_NODE (ctx->ugly_ui? \
    PRI_u8 "\rNode \"" PRI_str "\" is not exist\n": \
    ERROR("Node #" PRI_u8 " '" PRI_str "' does not exist\n"))

#define EDGE_EXISTS (ctx->ugly_ui? \
    "Edge between nodes " PRI_str " and " PRI_str " is already exist": \
    WARNING("edge between nodes '" PRI_str "' and '" PRI_str "' already exists\n"))

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
            printf(NODE_EXISTS, FMT_str(name));
        } else {
            u32 id = graph_add_node(&ctx->graph);
            if (index != id) {
                printf(ERROR("Name index (%zu) does not match graph id (" PRI_u32 ")\n"
                             "This is a bug in the program, report it.\n"), index, id);
                *result = FATAL_ERROR;
                return true;
            }
        }
    } else {
        if (!names_find(&ctx->names, name, &index)) {
            printf(MISSING_NODE, arg_no, FMT_str(name));
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
            names_fprint_debug(&ctx->names, stdout);
            graph_fprint_debug(&ctx->graph, stdout);
        }

        /* Just skip an empty line */
        return OK;
    } else if (cmd == END) {
        return NICE_STOP;
    } else if (cmd == DEBUG) {
        ctx->in_debug = !ctx->in_debug;
        printf("Debug mode: turned %s\n", ctx->in_debug? PAINT(GREEN, "on") : PAINT(RED, "off"));
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
            printf(EDGE_EXISTS, FMT_str(&name1), FMT_str(&name2));
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index1: " PRI_u32 ", Index2: " PRI_u32 ", Weight: " PRI_u16 "\n"),
                   id1, id2, weight);
        }
    } else if (cmd == REMOVE_NODE) {
        /* REMOVE_NODE <name> */

        u32 id;
        str name;
        if (str_to_node(ctx, &args, &name, 1, false, &id, &result)) {
            return result;
        }

        array_replace_by_last(&ctx->names, id);
        graph_del_node_and_replace_by_last(&ctx->graph, id);

        if (ctx->names.length != ctx->graph.nodes.length) {
            printf(ERROR("Count of names (%zu) does not match count of nodes (%zu)\n"
                         "This is a bug in the program, report it.\n"),
                   ctx->names.length, ctx->graph.nodes.length);
            return FATAL_ERROR;
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index: " PRI_u32 "\n"), id);
        }
    } else if (cmd == REMOVE_EDGE) {
        /* REMOVE_EDGE <name1> <name2> */

        u32 id1, id2;
        str name1, name2;
        if (str_to_node(ctx, &args, &name1, 1, false, &id1, &result)) {
            return result;
        }
        if (str_to_node(ctx, &args, &name2, 2, false, &id2, &result)) {
            return result;
        }

        if (!graph_del_edge(&ctx->graph, id1, id2)) {
            printf(EDGE_EXISTS, FMT_str(&name1), FMT_str(&name2));
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index1: " PRI_u32 ", Index2: " PRI_u32 "\n"), id1, id2);
        }
    } else if (cmd == ROOT) {
        /* ROOT <name> */

        str name;
        if (str_to_node(ctx, &args, &name, 1, false, &ctx->root_node, &result)) {
            return result;
        }

        if (ctx->in_debug) {
            printf(DEBUG("Index: " PRI_u32 "\n"), ctx->root_node);
        }
    } else if (cmd == PRINT_RPO) {
        /* PRINT_RPO */


        if (ctx->root_node == (u32)-1) {
            printf(ERROR("root node needs to be set\n"));
            return OK;
        }
        if (ctx->root_node >= ctx->names.length) {
            printf(ERROR("root node is outdated and needs to be reset\n"));
            return OK;
        }

        array_u32 ordering, back_edges;
        graph_reverse_post_order(&ctx->graph, ctx->root_node, &ordering, &back_edges);

        u32 *it;
        array_for(&ordering, it) {
            if (it_index) printf(" ");
            printf(PRI_str, FMT_str(&ctx->names.data[*it]));
        }
        printf("\n");

        array_for(&back_edges, it) {
            GraphEdge *edge = &ctx->graph.edges.data[*it];
            printf("Found back edge: " PRI_str " -> " PRI_str "\n",
                   FMT_str(&ctx->names.data[edge->source]),
                   FMT_str(&ctx->names.data[edge->target]));
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

int Main(Clargs *clargs) {
    Context ctx;
    ctx.in_debug = false;
    ctx.glow = true;
    ctx.ugly_ui = false;
    ctx.root_node = -1;

    str *clarg;
    array_for(clargs, clarg) {
        if (str_compare(clarg, &str_lit("--no-glow")) == 0) {
            ctx.glow = false;
        } else if (str_compare(clarg, &str_lit("--ugly-ui")) == 0) {
            ctx.ugly_ui = true;
            ctx.glow = false;
        }
    }

    array_init(&ctx.names);
    graph_init(&ctx.graph);

    StrBuilder sb;
    array_init(&sb);

    if (!ctx.ugly_ui) {
        printf("Welcome to example graph program!\n");
    }

    while (1) {
        if (!ctx.ugly_ui) {
            printf("\n> ");
            fflush(stdout);
        }

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

DEFINE_MAIN_CLARGS
