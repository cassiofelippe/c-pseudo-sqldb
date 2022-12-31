#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_MAX_BEFORE_POINTERS 10
#define COMMAND_MAX 50


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


/* functions / methods */

Command* add_command(ListCommand* list, Command* command) {
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
	ListCommand* commands = malloc(sizeof(ListCommand));

	Command* c1 = malloc(sizeof(Command));
	c1->cod = 1;
	strcpy(c1->name, "SELECT");
	add_command(commands, c1);

	Command* c2 = malloc(sizeof(Command));
	c2->cod = 2;
	strcpy(c2->name, "INSERT");
	add_command(commands, c2);

	Command* c3 = malloc(sizeof(Command));
	c3->cod = 3;
	strcpy(c3->name, "UPDATE");
	add_command(commands, c3);

	Command* c4 = malloc(sizeof(Command));
	c4->cod = 4;
	strcpy(c4->name, "DELETE");
	add_command(commands, c4);

	return commands;
}


int main() {
	int i = 0;

	printf("Hello World!\n");

	ListCommand* commands = get_commands();

	print_all_commands(commands);
	
}