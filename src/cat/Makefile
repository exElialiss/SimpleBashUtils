FLAGS = -Wall -Werror -Wextra -c
CC = gcc

all: build 
build: s21_cat clean

s21_cat: s21_cat.o Table.o 
	$(CC) s21_cat.o Table.o -o s21_cat


Table.o: Table.c
	$(CC) $(FLAGS) Table.c



clean:
	rm -rf *.o 