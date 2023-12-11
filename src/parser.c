#include <yaml.h>
#include "parser.h"


static lol_context_t self;

lol_context_t* lol_context() {
    return &self;
}

int read_yaml_config(const char* filename) {
    FILE *file;
    yaml_parser_t parser;
    yaml_document_t *document = NULL;

    file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return -1;
    }
    if (!yaml_parser_initialize(&parser)) {
        printf("Failed to initialize parser\n");
        fclose(file);
        return -1;
    }
    yaml_parser_set_input_file(&parser, file);

    document = calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document)) {
        printf("Failed to parse configuration file '%s'", filename);
        switch (parser.error) {
        case YAML_MEMORY_ERROR:
            printf("Memory error: Not enough memory for parsing");
            break;
        case YAML_READER_ERROR:
            if (parser.problem_value != -1)
                printf("Reader error - %s: #%X at %zd", parser.problem,
                    parser.problem_value, parser.problem_offset);
            else
                printf("Reader error - %s at %zd", parser.problem,
                    parser.problem_offset);
            break;
        case YAML_SCANNER_ERROR:
            if (parser.context)
                printf("Scanner error - %s at line %zu, column %zu"
                        "%s at line %zu, column %zu", parser.context,
                        parser.context_mark.line+1,
                        parser.context_mark.column+1,
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            else
                printf("Scanner error - %s at line %zu, column %zu",
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            break;
        case YAML_PARSER_ERROR:
            if (parser.context)
                printf("Parser error - %s at line %zu, column %zu"
                        "%s at line %zu, column %zu", parser.context,
                        parser.context_mark.line+1,
                        parser.context_mark.column+1,
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            else
                printf("Parser error - %s at line %zu, column %zu",
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            break;
        default:
            /* Couldn't happen. */
            printf("Unknown parser error");
            break;
        }
        free(document);
        yaml_parser_delete(&parser);
        fclose(file);
        return -1;
    }

    lol_context()->document = document;
    yaml_parser_delete(&parser);
    fclose(file);
    return 0;
}

typedef struct {
    yaml_document_t *document;
    yaml_node_t *node;
    yaml_node_pair_t *pair;
    yaml_node_item_t *item;
} lol_yaml_iter_t;

void lol_yaml_iter_init(lol_yaml_iter_t *iter, yaml_document_t *document) {
    memset(iter, 0, sizeof(lol_yaml_iter_t));
    iter->document = document;
    iter->node = yaml_document_get_root_node(document);

    if (iter->node->type == YAML_MAPPING_NODE) {
        iter->pair = iter->node->data.mapping.pairs.start - 1;
    } else {
        iter->item = iter->node->data.sequence.items.start - 1;
    }
}

int lol_yaml_iter_next(lol_yaml_iter_t *iter) {
    if (iter->node->type == YAML_MAPPING_NODE) {
        if (iter->pair) {
            iter->pair++;
            if (iter->pair < iter->node->data.mapping.pairs.top)
                return 1;
        }
    } else if (iter->node->type == YAML_SEQUENCE_NODE) {
        if (iter->item) {
            iter->item++;
            if (iter->item < iter->node->data.sequence.items.top)
                return 1;
        }
    }
    return 0;
}

const char* lol_yaml_iter_key(lol_yaml_iter_t *iter) {
    yaml_node_t *node = NULL;
    if (iter->node->type == YAML_MAPPING_NODE) {
        node = yaml_document_get_node(iter->document, iter->pair->key);
        return (const char*)node->data.scalar.value;
    } else if (iter->node->type == YAML_SEQUENCE_NODE) {
        node = yaml_document_get_node(iter->document, *iter->item);
        return (const char*)node->data.scalar.value;
    }
    return NULL;
}

void lol_yaml_iter_recurse(lol_yaml_iter_t *parent, lol_yaml_iter_t *iter) {
    memset(iter, 0, sizeof(lol_yaml_iter_t));
    iter->document = parent->document;
    if (parent->node->type == YAML_MAPPING_NODE) {
        iter->node = yaml_document_get_node(
            parent->document, parent->pair->value
        );
        if (iter->node->type == YAML_MAPPING_NODE)
            iter->pair = iter->node->data.mapping.pairs.start - 1;
        else if (iter->node->type == YAML_SEQUENCE_NODE)
            iter->item = iter->node->data.sequence.items.start - 1;
    } else if (parent->node->type == YAML_SEQUENCE_NODE) {
        iter->node = yaml_document_get_node(parent->document, *parent->item);
        if (iter->node->type == YAML_MAPPING_NODE)
            iter->pair = iter->node->data.mapping.pairs.start - 1;
        else if (iter->node->type == YAML_SEQUENCE_NODE) {
            iter->item = iter->node->data.sequence.items.start - 1;
        }
    } else {

    }
}

const char *lol_yaml_iter_value(lol_yaml_iter_t *iter)
{
    if (iter->node->type == YAML_SCALAR_NODE) {
        return (const char *)iter->node->data.scalar.value;
    } else if (iter->node->type == YAML_MAPPING_NODE) {
        yaml_node_t *node = yaml_document_get_node(iter->document, iter->pair->value);
        return (const char *)node->data.scalar.value;
    } else if (iter->node->type == YAML_SEQUENCE_NODE) {
        yaml_node_t *node = yaml_document_get_node(iter->document, *iter->item);
        return (const char *)node->data.scalar.value;
    }

    return NULL;
}

void parse_yaml_config() {
    yaml_document_t *document = lol_context()->document;
    lol_yaml_iter_t root_iter;

    lol_yaml_iter_init(&root_iter, document);
    while (lol_yaml_iter_next(&root_iter)) {
        const char *root_key = lol_yaml_iter_key(&root_iter);
        if (!strcmp(root_key, "format")) {
            
        } else if (!strcmp(root_key, "output")) {
            lol_yaml_iter_t output_iter;
            lol_yaml_iter_recurse(&root_iter, &output_iter);
            while (lol_yaml_iter_next(&output_iter)) {
                const char *output_key = lol_yaml_iter_key(&output_iter);
                if (!strcmp(output_key, "file")) {
                    lol_yaml_iter_t file_iter;
                    lol_yaml_iter_recurse(&output_iter, &file_iter);
                    while (lol_yaml_iter_next(&file_iter)) {
                        const char *file_key = lol_yaml_iter_key(&file_iter);
                        if (!strcmp(file_key, "enable")) {
                            lol_context()->file_enable = !strcmp(lol_yaml_iter_value(&file_iter), "true");
                        } else if (!strcmp(file_key, "level")) {
                            lol_context()->file_level = lol_log_level_string_to_enum(lol_yaml_iter_value(&file_iter));
                        } else if (!strcmp(file_key, "location")) {
                            lol_context()->file_location = lol_yaml_iter_value(&file_iter);
                        }
                    }
                } else if (!strcmp(output_key, "stdout")) {
                    lol_yaml_iter_t stdout_iter;
                    lol_yaml_iter_recurse(&output_iter, &stdout_iter);
                    while (lol_yaml_iter_next(&stdout_iter)) {
                        if (!strcmp(lol_yaml_iter_key(&stdout_iter), "enable")) {
                            
                        } else if (!strcmp(lol_yaml_iter_key(&stdout_iter), "level")) {
                            
                        }
                    }
                }
            }
        }
    }
}