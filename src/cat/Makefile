FLAGS = -Wall -Werror -Wextra -c
CC = gcc

all: build 
build: s21_cat clean

s21_cat: s21_cat.o Table.o cat.o
	$(CC) s21_cat.o Table.o cat.o -o s21_cat

s21_cat.o: s21_cat.c 
	$(CC) $(FLAGS) s21_cat.c

Table.o: Table.c
	$(CC) $(FLAGS) Table.c

cat.o: cat.c
	$(CC) $(FLAGS) cat.c

clean:
	rm -rf *.o 