#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <errno.h>

// DLL = Doubly-Linked-List

#define DATA_MAX_BEFORE_POINTERS 10 // TODO use DLL
#define MAX_INPUT_SIZE 255 // TODO use DLL
#define COMMAND_MAX 255 // TODO use DLL
#define DB_ROW_MAX 255 // TODO use DLL

/* defined function to get the size of a struct member without declaring it */
#define UNDECLARED_STRUCT_MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

/* structs / types */

struct TCommand {
    int cod;
    char name[COMMAND_MAX];
    struct TCommand *prev;
    struct TCommand *next;
};

typedef struct TCommand Command;

typedef struct {
    Command *first;
    Command *last;
} ListCommand;

typedef int boolean;


/* function declarations */
ListCommand* read_commands();
void input(char *str, int max_size, FILE *stream);
void query(char *attributes, char *database, char *filter);
void print_command(Command *command);
void print_all_inverted(ListCommand *list);
void print_all(ListCommand *list);

/* global vars */
ListCommand* commands;


/* functions / methods */

Command* add_command(ListCommand *list, Command *command) {
    if (list->first == NULL) {
        command->prev = NULL;
        command->next = NULL;
        list->first = command;
    } else {
        Command *last = list->first;

        while (last != NULL && last->next != NULL) {
            last = last->next;
        }

        last->next = command;
        command->prev = last;
        list->last = command;
    }

    return command;
}


void print_command(Command *command) {
    // printf("%d - %s %s", command->cod, command->name, command->next == NULL ? "" : "\n");
    printf("%s %s", command->name, command->next == NULL ? "" : "\n");
}

void print_all_commands(ListCommand *list) {
    Command *aux = list->first;

    printf("\n");
    for (; aux != NULL; aux = aux->next) {
        print_command(aux);
    }
    printf("\n");
}


void print_all_inverted(ListCommand *list) {
    Command *aux = list->last;

    printf("\n");
    for (; aux != NULL; aux = aux->prev) {
        print_command(aux);
    }
    printf("\n");
}


ListCommand* read_commands() {
	FILE *db = fopen("system/commands", "r");
	char ch;
	int i = 1;

    ListCommand *commands = malloc(sizeof(ListCommand));
    Command *c = malloc(sizeof(Command));
    char *name = malloc(UNDECLARED_STRUCT_MEMBER_SIZE(Command, name));

    while (ch != EOF) {
        ch = fgetc(db);

        if (ch == '\n' || ch == EOF) {
            c->cod = i;
            strcpy(c->name, name);
            add_command(commands, c);
            i++;
            c = malloc(sizeof(Command));
            name = malloc(UNDECLARED_STRUCT_MEMBER_SIZE(Command, name));

            if (ch == EOF) {
                break;
            }
        } else if (ch != EOF) {
        	strncat(name, &ch, 1);
        }
    }

    fclose(db);

    return commands;
}


void user_interaction() {
    char *command = malloc(COMMAND_MAX);
    char *attributes = malloc(COMMAND_MAX);
    char *database = malloc(COMMAND_MAX);
    char filter[COMMAND_MAX] = "";
    
    int user_input_size = sizeof(char) * MAX_INPUT_SIZE;
    char *user_input = malloc(user_input_size);
    int i;

    printf("\n");
    printf("Run your pseudo-SQL query: ");
    input(user_input, user_input_size, stdin);

    char *token = strtok(user_input, " ");
    
    for (i = 0; token != NULL; i++) {
        switch (i) {
            case 0:
                command = token;
                // printf(">> command: %s\n", command);
                break;
            case 1:
                attributes = token;
                // printf(">> attributes: %s\n", attributes);
                break;
            case 2:
                // from = token;
                break;
            case 3:
                database = token;
                // printf(">> database: %s\n", database);
                break;
            default: // greater than 3
                if (strcmp(token, "where") == 0) {
                    strcat(filter, token);
                } else {
                    strcat(filter, "$");
                    strcat(filter, token);
                }
                printf(">> filter: %s\n", filter);

                break;
        }

        // printf("%s\n", token);
        token = strtok(NULL, " ");
    }


    printf("You ran [%s]\n", user_input);

    // TODO reduce the code below to a filter funcion

    Command *found_command = NULL;
    Command *aux = commands->first;
    boolean has_found = 0;

    while (aux != NULL && aux->next != NULL) {
        if (strcasecmp(aux->name, command) == 0) {
            found_command = aux;
            has_found = 1;
            break;
        }

        aux = aux->next;
    }

    if (has_found == 0) {
        printf("Command [%s] not found!", command);
        user_interaction();
        return;
    }

    // printf(">> found command: %d - %s\n", found_command->cod, found_command->name);

    if (found_command == NULL) {
        printf("Command [%s] not found!\n", command);
    } else {
    	int swcommand = found_command->cod;
    	switch (swcommand) {
    		case 1: /* SELECT */
    			// printf(">> it is select!\n");
    			query(attributes, database, filter);
    			break;
    		default:
    			// printf(">> default (switch)\n");
    			break;
    	}
    }

}

void query(char *attributes, char *database, char *filter) {
    /* checking database file */
    char dbpath[] = "databases/";
    char path[sizeof(char*) * strlen(dbpath) + strlen(database)];
    /* clear the var path */
    memset(path, 0, strlen(path));
    /* set it's value */
    strcat(path, dbpath);
    strcat(path, database);

    // printf(">> path: %s\n", path);

    FILE *db = fopen(path, "r");
    char ch;
    char *header = malloc(sizeof(char) * DB_ROW_MAX);
    char rows[DB_ROW_MAX][DB_ROW_MAX];
    /* matrix containing each item (property: value) */
    char *map[DB_ROW_MAX][DB_ROW_MAX];
    /* mapping control (columns and rows) */
    int mccol = 0;

    if (NULL == db) {
        printf("Database [%s] does not exist!\n", path);
    }
 
    // printf(">> content of this file are\n");
 
    int i = -1, j = 0, isheader = 1;

    while (ch != EOF) {
        ch = fgetc(db);

        if (ch == '\n') {
            isheader = 0;

            rows[i][j] = '\0';

            i++;
            j = -1;
        } else if (ch != EOF) {
            if (isheader == 1) {
                header[j] = ch;
            } else {
            	rows[i][j] = ch;
            }
        }
        j++;
    }

    /* records how many rows there are */
    int rows_length = i;
    int cols_length = 0;
 
    /* closing the db table file */
    fclose(db);

    /* map the header */
    char *maptoken = strtok(header, ",");
    for (i = 0; maptoken != NULL; i++) {
        map[0][i] = maptoken;
        maptoken = strtok(NULL, ",");
        cols_length++;
    }

    // printf(">> mapped header %s %s %s\n", map[0][0], map[0][1], map[0][2]);

    /* map the rows */
    for (i = 0; i <= rows_length; i++) {
        char *row = rows[i];
        // printf(">> row %s\n", row);
        maptoken = strtok(row, ",");

        for (j = 0; maptoken != NULL; j++) {
            map[i + 1][j] = maptoken;
            maptoken = strtok(NULL, ",");
        }
    }

    // printf(">> filter %s\n", filter);

    boolean do_filter = 0;
    char property[MAX_INPUT_SIZE] = "";
    char comparator[2] = "";
    char value[MAX_INPUT_SIZE] = "";

    char *token = strtok(filter, "$");

    // printf(">> token %s\n", token);

    /* map the items */
    for (i = 0; token != NULL; i++) {
        switch (i) {
            case 0: // is where, do nothing
                do_filter = 1;
                break;
            case 1: // first property
                for (j = 0; j < cols_length; j++) {
                    // printf(">> comparing %s with %s = %d\n", map[0][j], token, strcmp(map[0][j], token));
                    if (strcmp(map[0][j], token) == 0) {
                        strcpy(property, token);
                        mccol = j;
                        break;
                    }
                }

                /* in case the property was not found */
                if (strcmp(property, "") == 0) {
                    printf("Property [%s] not found in table [%s]!\n", token, database);
                    return;
                }
                break;
            case 2: // comparator
                strcpy(comparator, token);

                if (strcmp(comparator, "") == 0) {
                    printf("Invalid comparator [%s]!\n", token);
                    return;
                }

                break;
            case 3: // value
                strcpy(value, token);

                if (strcmp(value, "") == 0) {
                    printf("Invalid value [%s]!\n", token);
                    return;
                }

                break;
            default:
                token = NULL;
                break;
        }
        token = strtok(NULL, "$");
    }

    // printf(">> property %s, comparator %s, value %s\n", property, comparator, value);


    // printf(">> header %s\n", map[0][3]);

    /* print the header */
    for (i = 0; map[0][i] != NULL; i++) {
        printf("%s%s", map[0][i], map[0][i+1] == NULL ? "" : ",");
    }

    printf("\n");

    int last_property = i;

    /* print the records */
    for (i = 1; i <= rows_length + 1; i++) {
        char *row_value = map[i][mccol];

        /* skip the record if there are filters and the value doesn't match */
        if (do_filter && strcmp(row_value, value) != 0) {
            continue;
        }

        for (j = 0; j < last_property; j++) {
            printf("%s%s", map[i][j], map[i][j+1] == NULL ? "" : ",");
        }
        printf("\n");
    }

    printf("\n");

}

void input(char *str, int max_size, FILE *stream) {
    fgets(str, max_size, stream);
    str[strlen(str) -1] = '\0';
}

int main() {
    printf("Hello World!\n");

    commands = read_commands();

    print_all_commands(commands);

    user_interaction();
    
    printf("\nexiting...\n");
}