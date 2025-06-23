CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/karou

# Create directories if they don't exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/karou

# Test with sample programs
test: $(TARGET)
	@echo "Testing basic print statement..."
	@echo 'print("Hello, Karou!");' | $(TARGET) -e 'print("Hello, Karou!");'
	@echo ""
	@echo "Testing variable assignment..."
	@$(TARGET) -e 'let x = 42; print(x);'
	@echo ""
	@echo "Testing arithmetic..."
	@$(TARGET) -e 'let result = 10 + 5 * 2; print(result);'

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help
help:
	@echo "Karou Script Compiler Build System"
	@echo "Available targets:"
	@echo "  all      - Build the compiler (default)"
	@echo "  clean    - Remove build files"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  test     - Run basic tests"
	@echo "  debug    - Build with debug symbols"
	@echo "  help     - Show this help"

.PHONY: all clean install uninstall test debug help
)