#!/bin/bash

# 设置目录和编译器
SRC_DIR="./src"
INCLUDES_DIR="./includes" 
OUTPUT="bd"


# 清理旧的目标文件
echo "Cleaning up old object files..."
rm -f "$SRC_DIR"/*.o "$OUTPUT"

# 编译源文件
echo "Compiling source files..."
object_files=()  # 用于存储生成的对象文件

for file in "$SRC_DIR"/*.cpp; do
    if [ -f "$file" ]; then
        echo "Compiling $file..."
	g++ -O3 -I"$INCLUDES_DIR" -c "$file" -o "${file%.cpp}.o"
	
        # 检查编译是否成功
        if [ $? -ne 0 ]; then
            echo "Compilation failed for $file."
            exit 1
        fi
        
        # 添加对象文件到数组
        object_files+=("${file%.cpp}.o")
    fi
done

# 检查是否生成了对象文件
if [ ${#object_files[@]} -eq 0 ]; then
    echo "No object files generated. Exiting."
    exit 1
else
    echo "${#object_files[@]} object files generated."
fi

# 链接目标文件
echo "Linking object files..."
g++ -o "$OUTPUT" "${object_files[@]}" -lcurl
if [ $? -ne 0 ]; then
    echo "Linking failed."
    exit 1
fi

# 完成构建
echo "Build complete."

