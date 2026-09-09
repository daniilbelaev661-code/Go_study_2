
CAT="./s21_cat"
ORIGINAL="cat"
TEST_FILE="test_input.txt"
OUTPUT_ORIG="orig_output.txt"
OUTPUT_MY="my_output.txt"


cat > $TEST_FILE << EOF
line1
line2

line4
line5

line7
line8
EOF


echo -e "col1\tcol2\tcol3" >> $TEST_FILE


FLAGS_LIST=(
    "-n"
    "-b"
    "-s"
    "-E"
    "-T"
    "-v"
    "-n -b"
    "-n -s"
    "-b -s"
    "-E -T"
    "-n -b -s -E -T"
    "-n -E"
    "-b -T"
    "-s -E"
    "-e"
    "-t"
    "-e -t"
    "-n -e"
    "-b -t"
    "-n -b -e -t"
)

echo "Running integration tests for s21_cat..."
FAILED=0
PASSED=0

for flags in "${FLAGS_LIST[@]}"; do

    $ORIGINAL $flags $TEST_FILE > $OUTPUT_ORIG 2>/dev/null
    $CAT $flags $TEST_FILE > $OUTPUT_MY 2>/dev/null

    if diff -q $OUTPUT_ORIG $OUTPUT_MY > /dev/null; then
        echo "[OK] $flags"
        PASSED=$((PASSED + 1))
    else
        echo "[FAIL] $flags"
        FAILED=$((FAILED + 1))
        echo "  Diff:"
        diff $OUTPUT_ORIG $OUTPUT_MY | head -10
    fi
done


echo "Testing stdin..."
echo "hello stdin" | $ORIGINAL > $OUTPUT_ORIG 2>/dev/null
echo "hello stdin" | $CAT > $OUTPUT_MY 2>/dev/null
if diff -q $OUTPUT_ORIG $OUTPUT_MY > /dev/null; then
    echo "[OK] stdin"
    PASSED=$((PASSED + 1))
else
    echo "[FAIL] stdin"
    FAILED=$((FAILED + 1))
fi


echo "Testing multiple files..."
$ORIGINAL $TEST_FILE $TEST_FILE > $OUTPUT_ORIG 2>/dev/null
$CAT $TEST_FILE $TEST_FILE > $OUTPUT_MY 2>/dev/null
if diff -q $OUTPUT_ORIG $OUTPUT_MY > /dev/null; then
    echo "[OK] multiple files"
    PASSED=$((PASSED + 1))
else
    echo "[FAIL] multiple files"
    FAILED=$((FAILED + 1))
fi


rm -f $TEST_FILE $OUTPUT_ORIG $OUTPUT_MY

echo "----------------------------------------"
echo "Tests passed: $PASSED, failed: $FAILED"
if [ $FAILED -eq 0 ]; then
    echo "All tests PASSED!"
    exit 0
else
    echo "Some tests FAILED!"
    exit 1
fi