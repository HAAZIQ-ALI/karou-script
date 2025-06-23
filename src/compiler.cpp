#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>

class KarouCompiler {
private:
    std::string sourceCode;
    std::unique_ptr<Program> ast;
    Interpreter interpreter;
    
public:
    bool loadFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        sourceCode = buffer.str();
        file.close();
        
        return true;
    }
    
    bool loadString(const std::string& code) {
        sourceCode = code;
        return true;
    }
    
    bool parse() {
        Parser parser(sourceCode);
        ast = parser.parseProgram();
        
        auto errors = parser.getErrors();
        if (!errors.empty()) {
            std::cerr << "Parse errors:" << std::endl;
            for (const auto& error : errors) {
                std::cerr << "  " << error << std::endl;
            }
            return false;
        }
        
        return true;
    }
    
    void printAST() {
        if (ast) {
            std::cout << "=== Abstract Syntax Tree ===" << std::endl;
            std::cout << ast->toString() << std::endl;
        }
    }
    
    void run() {
        if (ast) {
            std::cout << "=== Execution Output ===" << std::endl;
            interpreter.interpret(*ast);
        }
    }
    
    void triggerEvent(const std::string& elementId) {
        interpreter.triggerEvent(elementId);
    }
};

void printUsage(const char* programName) {
    std::cout << "Karou Script Compiler v1.0" << std::endl;
    std::cout << "Usage: " << programName << " [options] <file.ks>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help     Show this help message" << std::endl;
    std::cout << "  -a, --ast      Print the Abstract Syntax Tree" << std::endl;
    std::cout << "  -i, --interactive  Run in interactive mode" << std::endl;
    std::cout << "  -e, --eval <code>  Evaluate code directly" << std::endl;
}

void interactiveMode() {
    std::cout << "Karou Script Interactive Mode" << std::endl;
    std::cout << "Type 'exit' to quit, 'help' for commands" << std::endl;
    
    KarouCompiler compiler;
    std::string input;
    
    while (true) {
        std::cout << "karou> ";
        std::getline(std::cin, input);
        
        if (input == "exit" || input == "quit") {
            break;
        }
        
        if (input == "help") {
            std::cout << "Commands:" << std::endl;
            std::cout << "  help - Show this help" << std::endl;
            std::cout << "  exit - Exit interactive mode" << std::endl;
            std::cout << "  trigger <elementId> - Trigger an onClick event" << std::endl;
            std::cout << "  Or enter Karou Script code directly" << std::endl;
            continue;
        }
        
        if (input.substr(0, 7) == "trigger") {
            std::string elementId = input.substr(8);
            compiler.triggerEvent(elementId);
            continue;
        }
        
        if (input.empty()) continue;
        
        if (compiler.loadString(input) && compiler.parse()) {
            compiler.run();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    bool showAST = false;
    bool interactive = false;
    std::string filename;
    std::string evalCode;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-a" || arg == "--ast") {
            showAST = true;
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        } else if (arg == "-e" || arg == "--eval") {
            if (i + 1 < argc) {
                evalCode = argv[++i];
            } else {
                std::cerr << "Error: --eval requires code argument" << std::endl;
                return 1;
            }
        } else if (arg[0] != '-') {
            filename = arg;
        }
    }
    
    if (interactive) {
        interactiveMode();
        return 0;
    }
    
    KarouCompiler compiler;
    
    // Handle direct code evaluation
    if (!evalCode.empty()) {
        if (compiler.loadString(evalCode) && compiler.parse()) {
            if (showAST) {
                compiler.printAST();
            }
            compiler.run();
        }
        return 0;
    }
    
    // Handle file compilation
    if (filename.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    if (!compiler.loadFile(filename)) {
        return 1;
    }
    
    if (!compiler.parse()) {
        return 1;
    }
    
    if (showAST) {
        compiler.printAST();
    }
    
    compiler.run();
    
    return 0;
}