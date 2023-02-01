# c-pseudo-sqldb

trying to make a file-based pseudo-sgdb in C language just for fun!  
btw I'm rusty as can be about programming in C...


# execution
for now, I'm working on the SELECT command, reading the file from a preset db file  
to execute the program, run the following command  
```sh
clear;clear;gcc main.c -Wall -o main && ./main < test/select
```

# what's working?
for now, only the `SELECT` command is working, and with it a single filter possibility  
eg.:
```sql
SELECT * FROM test
``` 
```sql
SELECT * FROM people WHERE birth = 1998
``` 


# TODO list
- use doubly-link-list for everything in there, in order to avoid allocating unused memory
- check syntax
- SELECT:
    - find file
    - filter / comparison
    - attributes
    - order
    - limit
