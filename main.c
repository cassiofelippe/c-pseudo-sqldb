#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <errno.h>

// DLL = Doubly-Linked-List

#define DATA_MAX_BEFORE_POINTERS 10 // TODO use DLL
#define MAX_INPUT_SIZE 255 // TODO use DLL
#define COMMAND_MAX 50 // TODO use DLL
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
    char *filter = malloc(COMMAND_MAX);
    
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
                printf("command: %s\n", command);
                break;
            case 1:
                attributes = token;
                printf("attributes: %s\n", attributes);
                break;
            case 2:
                // from = token;
                break;
            case 3:
                database = token;
                printf("database: %s\n", database);
                break;
            case 4:
                // where = token;
                break;
            case 5:
                filter = token;
                printf("filter: %s\n", filter);
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

    printf("found command: %d - %s\n", found_command->cod, found_command->name);

    if (found_command == NULL) {
        printf("Command [%s] not found!\n", command);
    } else {
    	int swcommand = found_command->cod;
    	switch (swcommand) {
    		case 1: /* SELECT */
    			printf("it is select!\n");
    			query(attributes, database, filter);
    			break;
    		default:
    			printf("default (switch)\n");
    			break;
    	}
    }

}

void query(char *attributes, char *database, char *filter) {
    /* checking database file */
    char dbpath[] = "databases/";
    char path[sizeof(char*) * strlen(dbpath) + strlen(database)];
    strcat(path, dbpath);
    strcat(path, database);

    printf(">> path: %s\n", path);

    FILE *db = fopen(path, "r");
    char ch;
    char *header = malloc(sizeof(char) * DB_ROW_MAX);
    char rows[DB_ROW_MAX][DB_ROW_MAX];

    if (NULL == db) {
        printf("Database [%s] does not exist!\n", path);
    }
 
    printf("\ncontent of this file are \n");
 
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
 
    /* closing the db table file */
    fclose(db);

    printf("%s\n", header);

    for (i = 0; i <= rows_length; i++) {
    	printf("%s\n", rows[i]);
    }

    
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