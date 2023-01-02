#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <errno.h>

#define DATA_MAX_BEFORE_POINTERS 10
#define MAX_INPUT_SIZE 255
#define COMMAND_MAX 50
#define DB_ROW_MAX 255

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


/* function declarations */
ListCommand* get_commands();
void input(char *str, int max_size, FILE *stream);
DIR* get_dir(char *path);

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


void print_all_commands(ListCommand *list) {
    Command *aux = list->first;

    printf("\n");
    for (; aux != NULL; aux = aux->next) {
        printf("%d - %s | ", aux->cod, aux->name);
    }
    printf("\n");
}


void print_all_inverted(ListCommand *list) {
    Command *aux = list->last;

    printf("\n");
    for (; aux != NULL; aux = aux->prev) {
        printf("%d - %s | ", aux->cod, aux->name);
    }
    printf("\n");
}


// TODO import commands from a file
ListCommand* get_commands() {
    ListCommand *commands = malloc(sizeof(ListCommand));

    Command *c1 = malloc(sizeof(Command));
    c1->cod = 1;
    strcpy(c1->name, "SELECT");
    add_command(commands, c1);

    Command *c2 = malloc(sizeof(Command));
    c2->cod = 2;
    strcpy(c2->name, "INSERT");
    add_command(commands, c2);

    Command *c3 = malloc(sizeof(Command));
    c3->cod = 3;
    strcpy(c3->name, "UPDATE");
    add_command(commands, c3);

    Command *c4 = malloc(sizeof(Command));
    c4->cod = 4;
    strcpy(c4->name, "DELETE");
    add_command(commands, c4);

    return commands;
}


void user_interaction() {
    // int command_input_size = UNDECLARED_STRUCT_MEMBER_SIZE(Command, name);
    char *command = malloc(COMMAND_MAX);
    char *attributes = malloc(COMMAND_MAX);
    char *database = malloc(COMMAND_MAX);
    char *comparison = malloc(COMMAND_MAX);
    
    int user_input_size = sizeof(char) * MAX_INPUT_SIZE;
    char *user_input = malloc(user_input_size);
    int i;

    printf("\nRun your pseudo-SQL query: ");
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
                comparison = token;
                printf("comparison: %s\n", comparison);
                break;
        }

        // printf("%s\n", token);
        token = strtok(NULL, " ");
    }


    printf("\nYou ran [%s]", user_input);

    // TODO reduce the code below to a filter funcion

    Command *fcommand = NULL;
    Command *aux = commands->first;

    while (aux != NULL && aux->next != NULL) {
        if (strcasecmp(aux->name, command) == 0) {
            fcommand = aux;
        }

        aux = aux->next;
    }

    if (fcommand == NULL) {
        printf("\nCommand [%s] not found!", command);
    }

    /* checking database file */
    char *path = "databases/test";
    FILE *db = fopen(path, "r");
    char ch;
    char *header = malloc(sizeof(char) * DB_ROW_MAX);
    char *rows = malloc(sizeof(char) * DB_ROW_MAX * DB_ROW_MAX);
    // char header[255];
    // char rows[255];

    if (NULL == db) {
        printf("\nDatabase [%s] does not exist!", path);
    }
 
    printf("\ncontent of this file are \n");
 
    i = 0;
    int j = 0;

    while (ch != EOF) {
        ch = fgetc(db);

        printf("%c", ch);

        if (ch == EOF) {
            printf("[FINAL]");
        }

        if (ch == '\n') {
            printf("[ENTER]");
            i++;
            j = -1;
        } else {
        	printf("<%d>", i);
            if (i == 0) {
                header[j] = ch;
            } else {
            	char *row = rows[i];
                row[j] = ch;
            }
        }
        j++;
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    }
 
    // Closing the file
    fclose(db);

    printf("\nheader >> [%s]", header);


    DIR *dir = get_dir("./databases");
    get_dir("/tmp");
    get_dir(".");
}

void input(char *str, int max_size, FILE *stream) {
    fgets(str, max_size, stream);
    str[strlen(str) -1] = '\0';
}

DIR* get_dir(char *path) {
    DIR* dir = opendir("mydir");
    if (dir) {
        /* Directory exists. */
        printf("\nDirectory [%s] exists!", path);
        // closedir(dir);
    } else if (ENOENT == errno) {
        /* Directory does not exist. */
        printf("\nDirectory [%s] does not exist!", path);
    } else {
        /* opendir() failed for some other reason. */
        printf("\nFailed to find the [%s] directory!", path);
    }

    return dir;
}


int main() {
    printf("Hello World!\n");

    commands = get_commands();

    user_interaction();
    
    printf("\n");
}