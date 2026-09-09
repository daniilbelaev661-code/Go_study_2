
# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

PROG="./s21_grep"
ORIGINAL="grep"


# Создание тестовых файлов
echo "Creating test files..."
echo -e "apple\nbanana\nApple\nBANANA\n123\n!@#" > test1.txt
echo -e "line with apple\nno match here\nanother apple\nfinal" > test2.txt
echo -e "one\ntwo\nthree\nfour\nfive" > numbers.txt
echo -e "" > empty.txt
echo -e "start middle end" > spaces.txt
echo -e "tab\tseparated" > tab.txt

# Файл с шаблонами для -f
echo -e "apple\nbanana" > patterns.txt
echo -e "^[0-9]" > patterns_regex.txt

# Файл для проверки -o с несколькими совпадениями
echo -e "aaa aaa aaa" > triple.txt

# Список тестовых случаев 
# Формат: "описание; флаги; шаблон; файлы"
TESTS=(
    "Basic search; ; apple; test1.txt"
    "Case insensitive; -i; apple; test1.txt"
    "Invert match; -v; apple; test1.txt"
    "Count lines; -c; apple; test1.txt"
    "List files; -l; apple; test1.txt test2.txt"
    "Line numbers; -n; apple; test1.txt"
    "Suppress filenames; -h; apple; test1.txt test2.txt"
    "Suppress errors; -s; nonexistent; test1.txt"
    "Pattern from file; -f patterns.txt; ; test1.txt"
    "Only matching; -o; apple; test1.txt"
    "Multiple -e with -i; -i -e apple -e banana; ; test1.txt"
    "Combined -i -n; -i -n; apple; test1.txt"
    "Combined -v -c; -v -c; apple; test1.txt"
    "Combined -i -v; -i -v; apple; test1.txt"
    "Combined -l -i; -l -i; apple; test1.txt test2.txt"
    "Combined -c -l (priority -l); -c -l; apple; test1.txt test2.txt"
    "Regex start anchor; -e '^a'; ; test1.txt"
    "Regex end anchor; -e 'e$'; ; test1.txt"
    "Regex dot; -e 'a..le'; ; test1.txt"
    "Regex bracket; -e '[0-9]'; ; test1.txt"
    "Multiple files; ; apple; test1.txt test2.txt"
    "Empty file; ; apple; empty.txt"
    "No match; ; xyz; test1.txt"
    "Option -o with multiple matches; -o; aaa; triple.txt"
    "Option -f with regex; -f patterns_regex.txt; ; numbers.txt"
    "Option -h with single file (no effect); -h; apple; test1.txt"
    "Option -s with nonexistent and existing; -s; apple; nonexistent test1.txt"
)

PASSED=0
FAILED=0
TOTAL=0

echo "----------------------------------------"
echo "Running integration tests for s21_grep..."
echo "----------------------------------------"

for test_case in "${TESTS[@]}"; do
    IFS=';' read -r desc flags pattern files <<< "$test_case"
    # Убираем лишние пробелы
    flags=$(echo "$flags" | xargs)
    pattern=$(echo "$pattern" | xargs)
    files=$(echo "$files" | xargs)

    # Формируем команду для оригинального grep
    if [ -z "$pattern" ]; then
        orig_cmd="$ORIGINAL $flags $files"
        my_cmd="$PROG $flags $files"
    else
        orig_cmd="$ORIGINAL $flags $pattern $files"
        my_cmd="$PROG $flags $pattern $files"
    fi

    # Запуск оригинального grep
    eval "$orig_cmd" > orig_out.txt 2> orig_err.txt
    orig_exit=$?

    # Запуск вашего grep
    eval "$my_cmd" > my_out.txt 2> my_err.txt
    my_exit=$?

    # Сравнение stdout
    diff_out=$(diff orig_out.txt my_out.txt)
    # Сравнение stderr
    diff_err=$(diff orig_err.txt my_err.txt)
    # Сравнение кодов возврата
    if [ $orig_exit -ne $my_exit ]; then
        diff_code="exit codes differ: orig=$orig_exit, my=$my_exit"
    else
        diff_code=""
    fi

    TOTAL=$((TOTAL + 1))
    if [ -z "$diff_out" ] && [ -z "$diff_err" ] && [ -z "$diff_code" ]; then
        echo -e "[${GREEN}OK${NC}] $desc ($flags $pattern $files)"
        PASSED=$((PASSED + 1))
    else
        echo -e "[${RED}FAIL${NC}] $desc ($flags $pattern $files)"
        if [ -n "$diff_out" ]; then
            echo "  --- stdout diff ---"
            echo "$diff_out"
        fi
        if [ -n "$diff_err" ]; then
            echo "  --- stderr diff ---"
            echo "$diff_err"
        fi
        if [ -n "$diff_code" ]; then
            echo "  --- $diff_code"
        fi
        FAILED=$((FAILED + 1))
    fi
done

# Очистка временных файлов
rm -f test1.txt test2.txt numbers.txt empty.txt spaces.txt tab.txt \
      patterns.txt patterns_regex.txt triple.txt \
      orig_out.txt my_out.txt orig_err.txt my_err.txt

echo "----------------------------------------"
echo -e "Tests passed: ${GREEN}$PASSED${NC}, failed: ${RED}$FAILED${NC}, total: $TOTAL"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests PASSED!${NC}"
    exit 0
else
    echo -e "${RED}Some tests FAILED!${NC}"
    exit 1
fi