FLAGS = -Wall -Werror -Wextra -c
CC = gcc

all: build 
build: s21_grep clean

s21_grep: s21_grep.o 
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c 
	$(CC) $(FLAGS) s21_grep.c

clean:
	rm -rf *.o 