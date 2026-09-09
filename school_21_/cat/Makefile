CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c11 -D_GNU_SOURCE
TARGET = s21_cat
SRCS = s21_cat.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.txt

rebuild: clean all

test: $(TARGET)
	./test_cat.sh

.PHONY: all clean rebuild test