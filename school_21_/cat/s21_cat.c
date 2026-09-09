#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Структура для хранения опций командной строки.
 Поля соответствуют флагам cat:
    number_nonblank  - нумеровать только непустые строки (флаг -b)
    number_all       - нумеровать все строки (флаг -n)
    squeeze_blank    - сжимать последовательные пустые строки (флаг -s)
    show_ends        - показывать символ конца строки как '$' (флаг -E или -e)
    show_tabs        - показывать табуляции как '^I' (флаг -T или -t)
    show_nonprintable - показывать непечатаемые символы (флаг -v, также включается -e и -t)
 */
typedef struct {
    int number_nonblank;
    int number_all;
    int squeeze_blank;
    int show_ends;
    int show_tabs;
    int show_nonprintable;
} options;

/*
Выводит один символ с учётом опций отображения.
  Параметры:
    c   - байт символа (unsigned char)
    opt - указатель на структуру options
  Если включён show_tabs, табуляция выводится как "^I".
  Если включён show_nonprintable, непечатаемые символы (<32, !=tab, !=newline)
  выводятся в виде ^X (или ^? для 127), а символы >=128 выводятся как M-^X и т.д.
  В противном случае символ выводится как есть.
 */
static inline void print_char(unsigned char c, const options *opt) {
    if (opt->show_tabs && c == '\t') {
        putchar('^');
        putchar('I');
    } else if (opt->show_nonprintable) {
        if (c < 32 && c != '\t' && c != '\n') {
            putchar('^');
            putchar(c == 127 ? '?' : c + 64);
        } else if (c >= 128) {
            putchar('M');
            putchar('-');
            if (c < 160) {
                putchar('^');
                putchar((c - 128) + 64);
            } else {
                putchar(c);
            }
        } else {
            putchar(c);
        }
    } else {
        putchar(c);
    }
}

/*
 Выводит номер строки с отступом в 6 символов и табуляцией, если это требуется.
  Параметры:
    line_number - указатель на текущий номер строки (инкрементируется, если строка нумеруется)
    is_empty    - флаг: 1 если строка пустая (содержит только '\n')
    opt         - указатель на структуру options
  Нумерация происходит, если:
    - number_nonblank = 1 и строка не пустая
    - number_all = 1 (приоритет у number_nonblank, если он включён, то number_all игнорируется)
  В любом случае номер увеличивается только при выводе номера.
 */
static void print_line_number(int *line_number, int is_empty, const options *opt) {
    if (opt->number_nonblank && !is_empty) {
        printf("%6d\t", (*line_number)++);
    } else if (opt->number_all && !opt->number_nonblank) {
        printf("%6d\t", (*line_number)++);
    }
}

/*
 Обрабатывает входной поток (файл или stdin) с применением заданных опций.
  Параметры:
    fp  - открытый указатель на FILE
    opt - указатель на структуру options
  Читает файл построчно, применяет сжатие пустых строк, нумерацию и
  преобразование символов согласно опциям. Результат выводится в stdout.
 */
static void process_file(FILE *fp, const options *opt) {
    char line[4096];
    int line_number = 1;
    int prev_blank = 0;

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        int is_empty = (len == 1 && line[0] == '\n');

        int should_skip = opt->squeeze_blank && is_empty && prev_blank;

        if (!should_skip) {
            print_line_number(&line_number, is_empty, opt);

            int has_newline = 0;
            if (len > 0 && line[len - 1] == '\n') {
                has_newline = 1;
                line[--len] = '\0';
            }

            for (size_t i = 0; i < len; i++) {
                print_char((unsigned char)line[i], opt);
            }

            if (opt->show_ends)
                putchar('$');

            if (has_newline)
                putchar('\n');

            prev_blank = is_empty;
        }
    }
}

static int parse_options(int argc, char **argv, options *opt) {
    int c, option_index = 0;
    int ret = 0;

    static struct option long_options[] = {
        {"number-nonblank", no_argument, 0, 'b'},
        {"number",          no_argument, 0, 'n'},
        {"squeeze-blank",   no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, "bensEtTv", long_options, &option_index)) != -1 && ret==0) {
        switch (c) {
            case 'b': opt->number_nonblank = 1; break;
            case 'e': opt->show_ends = 1; opt->show_nonprintable = 1; break;
            case 'E': opt->show_ends = 1; break;
            case 'n': opt->number_all = 1; break;
            case 's': opt->squeeze_blank = 1; break;
            case 't': opt->show_tabs = 1; opt->show_nonprintable = 1; break;
            case 'T': opt->show_tabs = 1; break;
            case 'v': opt->show_nonprintable = 1; break;
            default:
                fprintf(stderr, "Usage: %s [options] [file...]\n", argv[0]);
                ret = 1;
                break;
        }
    }
    return ret;
}

int main(int argc, char **argv) {
    options opt = {0};
    if (parse_options(argc, argv, &opt) != 0) {
        return 1;
    }

    if (optind == argc) {
        process_file(stdin, &opt);
    } else {
        for (int i = optind; i < argc; i++) {
            FILE *fp = NULL;
            int is_stdin = 0;

            if (strcmp(argv[i], "-") == 0) {
                fp = stdin;
                is_stdin = 1;
            } else {
                fp = fopen(argv[i], "r");
                if (fp == NULL) {
                    fprintf(stderr, "%s: %s: %s\n", argv[0], argv[i], strerror(errno));
                }
            }
            if (fp != NULL) {
                process_file(fp, &opt);
                if (!is_stdin) {
                    fclose(fp);
                }
            }
        }
    }
    return 0;
}