CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c11 -D_GNU_SOURCE
TARGET = s21_grep
SRCS = s21_grep.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

s21_grep: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

test: $(TARGET)
	chmod +x test_grep.sh
	./test_grep.sh

.PHONY: all clean rebuild test s21_grep