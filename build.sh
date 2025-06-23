#!/bin/bash

echo "Building Karou Script Compiler..."

# Create directories
mkdir -p obj bin

# Compile all source files
echo "Compiling source files..."
g++ -std=c++17 -Wall -Wextra -O2 -c src/token.cpp -o obj/token.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/lexer.cpp -o obj/lexer.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/ast.cpp -o obj/ast.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/parser.cpp -o obj/parser.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/interpreter.cpp -o obj/interpreter.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/compiler.cpp -o obj/compiler.o

# Link executable
echo "Linking executable..."
g++ obj/*.o -o bin/karou

if [ $? -eq 0 ]; then
    echo "✅ Build successful! Executable created at bin/karou"
    echo ""
    echo "Usage examples:"
    echo "  ./bin/karou examples/hello.ks"
    echo "  ./bin/karou -a examples/calculator.ks"
    echo "  ./bin/karou -i"
    echo "  ./bin/karou -e 'print(\"Hello World!\");'"
else
    echo "❌ Build failed!"
    exit 1
fi