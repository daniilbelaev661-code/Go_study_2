#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 128
#define MAX_LINE_LEN 4096



/*
 Структура для хранения опций командной строки и списка шаблонов.
 Поля соответствуют флагам grep: e, i, v, c, l, n, h, s, f, o.
 pattern_count - количество добавленных шаблонов.
 patterns - массив указателей на строки шаблонов.
 */
typedef struct {
    int e_flag;
    int i_flag;
    int v_flag;
    int c_flag;
    int l_flag;
    int n_flag;
    int h_flag;
    int s_flag;
    int f_flag;
    int o_flag;
    char *patterns[MAX_PATTERNS];
    int pattern_count;
} options;

/*
 Добавляет один шаблон в структуру options.
  Параметры:
    opt     - указатель на структуру options
    pattern - строка с шаблоном 
 */
static int add_pattern(options *opt, const char *pattern) {
    int result = 0;
    if (opt->pattern_count < MAX_PATTERNS) {
        opt->patterns[opt->pattern_count] = strdup(pattern);
        if (!opt->patterns[opt->pattern_count]) {
            fprintf(stderr, "Memory allocation failed\n");
            result = 1;
        } else {
            opt->pattern_count++;
        }
    } else {
        fprintf(stderr, "Too many patterns\n");
        result = 1;
    }
    return result;
}

/*
 Загружает шаблоны из файла и добавляет их в options.
  Параметры:
    opt      - указатель на структуру options
    filename - имя файла
*/
static int load_patterns_from_file(options *opt, const char *filename) {
    int result = 0;
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
        result = 1;
    } else {
        char line[MAX_LINE_LEN];
        while (fgets(line, sizeof(line), fp) && result == 0) {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
            if (len > 0 && add_pattern(opt, line) != 0) {
                result = 1;
            }
        }
        fclose(fp);
    }
    return result;
}

/*
Проверяет, соответствует ли строка line одному заданному шаблону pattern.
  Параметры:
    line    - строка для проверки
    pattern - шаблон (регулярное выражение)
    flags   - флаги компиляции regex (например, REG_ICASE)
 */
static int match_single_pattern(const char *line, const char *pattern, int flags) {
    int result = 0;
    regex_t regex;
    int ret = regcomp(&regex, pattern, flags);
    if (ret == 0) {
        if (regexec(&regex, line, 0, NULL, 0) == 0)
            result = 1;
        regfree(&regex);
    } else {
        char errbuf[256];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Regex error: %s\n", errbuf);
        regfree(&regex);
    }
    return result;
}

/*
 Проверяет, соответствует ли строка line хотя бы одному из шаблонов в options.
  Параметры:
    line - строка для проверки
    opt  - содержит список шаблонов и флаги
  Учитывает флаг i_flag (регистронезависимость).
*/
static int matches_any_pattern(const char *line, const options *opt) {
    int result = 0;
    int flags = REG_EXTENDED | REG_NOSUB;
    if (opt->i_flag)
        flags |= REG_ICASE;

    for (int i = 0; (i < opt->pattern_count) && result == 0; i++) {
        if (match_single_pattern(line, opt->patterns[i], flags))
            result = 1;
    }
    return result;
}

/*
Находит первое вхождение шаблона pattern в строке line.
 Параметры:
    line    - строка для поиска
    pattern - шаблон (регулярное выражение)
    flags   - флаги компиляции regex
    start   - указатель для сохранения начальной позиции совпадения
    end     - указатель для сохранения конечной позиции совпадения
 */
static int find_first_match(const char *line, const char *pattern, int flags, int *start, int *end) {
    int result = 0;
    regex_t regex;
    regmatch_t pmatch[1];
    int ret = regcomp(&regex, pattern, flags);
    if (ret == 0) {
        if (regexec(&regex, line, 1, pmatch, 0) == 0) {
            *start = pmatch[0].rm_so;
            *end = pmatch[0].rm_eo;
            if (*end > *start)
                result = 1;
        }
        regfree(&regex);
    } else {
        regfree(&regex);
    }
    return result;
}
/*
Выводит все непересекающиеся части строки line, соответствующие любому из шаблонов.
 Параметры:
    line - исходная строка
    opt  - структура options (содержит шаблоны и флаг i_flag)
*/
static void print_matching_parts(const char *line, const options *opt) {
    int flags = REG_EXTENDED;
    if (opt->i_flag)
        flags |= REG_ICASE;

    const char *cursor = line;
    while (*cursor) {
        int matched = 0;
        for (int i = 0; i < opt->pattern_count && !matched; i++) {
            int start = 0, end = 0;
            if (find_first_match(cursor, opt->patterns[i], flags, &start, &end)) {
                printf("%.*s\n", end - start, cursor + start);
                cursor += end;
                matched = 1;
            }
        }
        if (!matched)
            cursor++;
    }
}

/*
Выводит строку line с префиксом (имя файла и/или номер строки) в зависимости от опций.
 Параметры:
    line       - строка для вывода
    filename   - имя файла (выводится, если file_count > 1 и не установлен h_flag)
    line_num   - номер строки (выводится, если установлен n_flag)
    file_count - общее количество обрабатываемых файлов
    opt        - структура options
  Вывод производится в stdout в формате: [filename:][line_num:]line.
*/
static void print_line_with_prefix(const char *line, const char *filename, int line_num, int file_count, const options *opt) {
    if (file_count > 1 && !opt->h_flag)
        printf("%s:", filename);
    if (opt->n_flag)
        printf("%d:", line_num);
    printf("%s\n", line);
}

/*
 Обрабатывает совпавшую строку: либо выводит её целиком с префиксом,
 либо (если установлен o_flag) выводит только совпавшие части.
  Параметры:
    line       - строка, которая совпала
    filename   - имя файла
    line_num   - номер строки
    file_count - общее количество файлов
    opt        - структура options
*/
static void handle_matching_line(const char *line, const char *filename, int line_num, int file_count, const options *opt) {
    if (opt->o_flag) {
        print_matching_parts(line, opt);
    } else {
        print_line_with_prefix(line, filename, line_num, file_count, opt);
    }
}

/*
 Обрабатывает один файл (или стандартный ввод) в соответствии с опциями.
  Параметры:
    filename       - имя файла (если "-", то читается stdin)
    opt            - структура options
    file_count     - общее количество файлов (для вывода префиксов)
    error_occurred - указатель на флаг ошибки (устанавливается в 1 при ошибке открытия)
  Возвращает 1, если хотя бы одна строка в файле совпала (с учётом v_flag), иначе 0.
 */
static int process_file(const char *filename, const options *opt, int file_count, int *error_occurred) {
    FILE *fp = NULL;
    int any_match = 0;
    int flag_err = 0;

    if (strcmp(filename, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(filename, "r");
        if (!fp) {
            if (!opt->s_flag) {
                fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
            }
            *error_occurred = 1;
            flag_err = 1;
        }
    }
    if (!flag_err) {
        char line[MAX_LINE_LEN];
        int line_num = 0;
        int match_count = 0;

        while (fgets(line, sizeof(line), fp)) {
            line_num++;
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';

            int is_match = matches_any_pattern(line, opt);
            if (opt->v_flag)
                is_match = !is_match;

            if (is_match) {
                match_count++;
                any_match = 1;
                if (!(opt->c_flag || opt->l_flag)) {
                    handle_matching_line(line, filename, line_num, file_count, opt);
                }
            }
        }

        if (opt->c_flag && !opt->l_flag) {
            if (file_count > 1 && !opt->h_flag)
                printf("%s:", filename);
            printf("%d\n", match_count);
        }

        if (opt->l_flag && any_match) {
            printf("%s\n", filename);
        }

        if (fp != stdin)
            fclose(fp);
    }
    return any_match;
}

/*
 Разбирает аргументы командной строки и заполняет структуру options.
  Параметры:
    argc             - количество аргументов
    argv             - массив аргументов
    opt              - указатель на структуру options (заполняется)
    file_start_index - указатель для сохранения индекса первого аргумента-файла
*/
static int parse_options(int argc, char **argv, options *opt, int *file_start_index) {
    int result = 0;
    int c;
    int pattern_provided = 0;

    while ((c = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && result == 0) {
        switch (c) {
            case 'e':
                if (add_pattern(opt, optarg) != 0) result = 1;
                pattern_provided = 1;
                break;
            case 'i': opt->i_flag = 1; break;
            case 'v': opt->v_flag = 1; break;
            case 'c': opt->c_flag = 1; break;
            case 'l': opt->l_flag = 1; break;
            case 'n': opt->n_flag = 1; break;
            case 'h': opt->h_flag = 1; break;
            case 's': opt->s_flag = 1; break;
            case 'f':
                opt->f_flag = 1;
                if (load_patterns_from_file(opt, optarg) != 0) result = 1;
                pattern_provided = 1;
                break;
            case 'o': opt->o_flag = 1; break;
            default:
                fprintf(stderr, "Usage: %s [OPTIONS] PATTERN [FILE...]\n", argv[0]);
                result = 1;
        }
    }

    if (result == 0) {
        if (!pattern_provided) {
            if (optind < argc) {
                if (add_pattern(opt, argv[optind]) != 0)
                    result = 1;
                else
                    optind++;
            } else {
                fprintf(stderr, "No pattern provided\n");
                result = 1;
            }
        }
    }

    if (result == 0)
        *file_start_index = optind;

    return result;
}

int main(int argc, char **argv) {
    options opt = {0};
    int file_start;
    int exit_code = EXIT_SUCCESS;
    int any_match = 0;
    int error_occurred = 0;

    if (parse_options(argc, argv, &opt, &file_start) != 0) {
        exit_code = 2;
    } else {
        int file_count = argc - file_start;
        if (file_count == 0) {
            any_match = process_file("-", &opt, 1, &error_occurred);
        } else {
            for (int i = file_start; i < argc; i++) {
                int file_match = process_file(argv[i], &opt, file_count, &error_occurred);
                if (file_match)
                    any_match = 1;
            }
        }
        if (error_occurred) {
            exit_code = 2;
        } else if (any_match) {
            exit_code = 0;
        } else {
            exit_code = 1;
        }
    }

    for (int i = 0; i < opt.pattern_count; i++)
        free(opt.patterns[i]);

    return exit_code;
}