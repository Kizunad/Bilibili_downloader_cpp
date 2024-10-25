echo "formatting..."
find . \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i
