# ============================================================================
# Switchback Rails - Makefile
# CS-1002: Programming Fundamentals Project (AI&DS)
# Fall 2025
# ============================================================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I$(CORE_DIR) -I$(SFML_DIR)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
CORE_DIR = core
SFML_DIR = sfml
OBJ_DIR = obj
OUT_DIR = out
DATA_DIR = data
SPRITES_DIR = Sprites
DOCS_DIR = docs

# Target executable
TARGET = switchback_rails

# Source files
CORE_SOURCES = $(CORE_DIR)/game.cpp $(CORE_DIR)/fileio.cpp
SFML_SOURCES = $(SFML_DIR)/render.cpp
MAIN_SOURCE = main.cpp

# Object files (in obj/ directory)
OBJECTS = $(OBJ_DIR)/game.o $(OBJ_DIR)/render.o $(OBJ_DIR)/fileio.o $(OBJ_DIR)/main.o

# Header files (for dependency tracking)
CORE_HEADERS = $(CORE_DIR)/game.h $(CORE_DIR)/fileio.h
SFML_HEADERS = $(SFML_DIR)/render.h

# Colors for pretty output
COLOR_RESET = \033[0m
COLOR_BOLD = \033[1m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m
COLOR_CYAN = \033[36m

# ============================================================================
# Default target - build everything
# ============================================================================

.PHONY: all
all: banner directories $(TARGET) success

# ============================================================================
# Build the executable
# ============================================================================

$(TARGET): $(OBJECTS)
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)  Linking executable: $(COLOR_BOLD)$(TARGET)$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Executable created successfully!$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Compile object files
# ============================================================================

$(OBJ_DIR)/game.o: $(CORE_DIR)/game.cpp $(CORE_DIR)/game.h
	@echo "$(COLOR_YELLOW)Compiling: core/game.cpp$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $(CORE_DIR)/game.cpp -o $(OBJ_DIR)/game.o

$(OBJ_DIR)/render.o: $(SFML_DIR)/render.cpp $(SFML_DIR)/render.h $(CORE_DIR)/game.h
	@echo "$(COLOR_YELLOW)Compiling: sfml/render.cpp$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $(SFML_DIR)/render.cpp -o $(OBJ_DIR)/render.o

$(OBJ_DIR)/fileio.o: $(CORE_DIR)/fileio.cpp $(CORE_DIR)/fileio.h $(CORE_DIR)/game.h
	@echo "$(COLOR_YELLOW)Compiling: core/fileio.cpp$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $(CORE_DIR)/fileio.cpp -o $(OBJ_DIR)/fileio.o

$(OBJ_DIR)/main.o: main.cpp $(CORE_DIR)/game.h $(SFML_DIR)/render.h $(CORE_DIR)/fileio.h
	@echo "$(COLOR_YELLOW)Compiling: main.cpp$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJ_DIR)/main.o

# ============================================================================
# Create necessary directories
# ============================================================================

.PHONY: directories
directories:
	@mkdir -p $(CORE_DIR)
	@mkdir -p $(SFML_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(DATA_DIR)/levels
	@mkdir -p $(SPRITES_DIR)
	@mkdir -p $(DOCS_DIR)

# ============================================================================
# Clean build artifacts
# ============================================================================

.PHONY: clean
clean:
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_YELLOW)Cleaning build artifacts...$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET)
	@echo "$(COLOR_GREEN)Clean complete!$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Clean output files (logs and CSV)
# ============================================================================

.PHONY: clean-output
clean-output:
	@echo "$(COLOR_YELLOW)Cleaning output files...$(COLOR_RESET)"
	@rm -f $(OUT_DIR)/*.csv
	@rm -f $(OUT_DIR)/*.txt
	@echo "$(COLOR_GREEN)Output directory cleaned!$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Deep clean (everything)
# ============================================================================

.PHONY: distclean
distclean: clean clean-output
	@echo "$(COLOR_YELLOW)Deep cleaning...$(COLOR_RESET)"
	@rm -rf $(OUT_DIR)
	@echo "$(COLOR_GREEN)Complete clean finished!$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Rebuild everything from scratch
# ============================================================================

.PHONY: rebuild
rebuild: clean all

# ============================================================================
# Run the simulation with default level
# ============================================================================

.PHONY: run
run: all
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)Running Switchback Rails...$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@./$(TARGET) levels/hard_level.lvl

# ============================================================================
# Run specific level files
# ============================================================================

.PHONY: run-easy
run-easy: all
	@echo "$(COLOR_CYAN)Running EASY level...$(COLOR_RESET)"
	@./$(TARGET) levels/easy_level.lvl

.PHONY: run-medium
run-medium: all
	@echo "$(COLOR_CYAN)Running MEDIUM level...$(COLOR_RESET)"
	@./$(TARGET) levels/medium_level.lvl

.PHONY: run-hard
run-hard: all
	@echo "$(COLOR_CYAN)Running HARD level...$(COLOR_RESET)"
	@./$(TARGET) levels/hard_level.lvl

.PHONY: run-complex
run-complex: all
	@echo "$(COLOR_CYAN)Running COMPLEX NETWORK level...$(COLOR_RESET)"
	@./$(TARGET) levels/complex_network.lvl

.PHONY: run-ultra
run-ultra: all
	@echo "$(COLOR_CYAN)Running ULTRA COMPLEX level (FOG)...$(COLOR_RESET)"
	@./$(TARGET) levels/ultra_complex.lvl

# ============================================================================
# Debug build (with debug symbols, no optimization)
# ============================================================================

.PHONY: debug
debug: CXXFLAGS = -std=c++11 -Wall -Wextra -g -O0 -I.
debug: clean all
	@echo "$(COLOR_GREEN)Debug build complete!$(COLOR_RESET)"
	@echo "$(COLOR_YELLOW)  Run with: gdb ./$(TARGET)$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Check for memory leaks with valgrind
# ============================================================================

.PHONY: valgrind
valgrind: debug
	@echo "$(COLOR_CYAN)Running valgrind memory check...$(COLOR_RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		./$(TARGET) $(DATA_DIR)/levels/easy_level.lvl

# ============================================================================
# Test determinism (run twice and compare outputs)
# ============================================================================

.PHONY: test-determinism
test-determinism: all
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)  Testing Determinism...$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_YELLOW)Running simulation - First pass...$(COLOR_RESET)"
	@./$(TARGET) $(DATA_DIR)/levels/easy_level.lvl > /dev/null 2>&1
	@cp $(OUT_DIR)/trace.csv $(OUT_DIR)/trace_test1.csv
	@echo "$(COLOR_YELLOW)Running simulation - Second pass...$(COLOR_RESET)"
	@./$(TARGET) $(DATA_DIR)/levels/easy_level.lvl > /dev/null 2>&1
	@cp $(OUT_DIR)/trace.csv $(OUT_DIR)/trace_test2.csv
	@echo "$(COLOR_YELLOW)Comparing outputs...$(COLOR_RESET)"
	@if diff $(OUT_DIR)/trace_test1.csv $(OUT_DIR)/trace_test2.csv > /dev/null; then \
		echo "$(COLOR_GREEN)DETERMINISM TEST PASSED!$(COLOR_RESET)"; \
		echo "$(COLOR_GREEN)  Both runs produced identical outputs.$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_RED)DETERMINISM TEST FAILED!$(COLOR_RESET)"; \
		echo "$(COLOR_RED)  Outputs differ between runs.$(COLOR_RESET)"; \
	fi
	@rm -f $(OUT_DIR)/trace_test1.csv $(OUT_DIR)/trace_test2.csv
	@echo ""

# ============================================================================
# Install dependencies (SFML)
# ============================================================================

.PHONY: install-deps
install-deps:
	@echo "$(COLOR_CYAN)Installing dependencies...$(COLOR_RESET)"
	@bash libraries.sh
	@echo "$(COLOR_GREEN)Dependencies installed!$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Check if SFML is installed
# ============================================================================

.PHONY: check-sfml
check-sfml:
	@echo "$(COLOR_CYAN)Checking SFML installation...$(COLOR_RESET)"
	@if pkg-config --exists sfml-all; then \
		echo "$(COLOR_GREEN)SFML is installed$(COLOR_RESET)"; \
		echo "  Version: $$(pkg-config --modversion sfml-all)"; \
	else \
		echo "$(COLOR_RED)SFML is NOT installed$(COLOR_RESET)"; \
		echo "  Run: make install-deps"; \
	fi
	@echo ""

# ============================================================================
# Display help information
# ============================================================================

.PHONY: help
help:
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_BOLD)   SWITCHBACK RAILS - MAKEFILE HELP$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)BUILD TARGETS:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make$(COLOR_RESET) or $(COLOR_CYAN)make all$(COLOR_RESET)       Build the project"
	@echo "  $(COLOR_CYAN)make clean$(COLOR_RESET)             Remove build artifacts"
	@echo "  $(COLOR_CYAN)make clean-output$(COLOR_RESET)      Remove output CSV/TXT files"
	@echo "  $(COLOR_CYAN)make distclean$(COLOR_RESET)         Complete clean (build + output)"
	@echo "  $(COLOR_CYAN)make rebuild$(COLOR_RESET)           Clean and rebuild everything"
	@echo "  $(COLOR_CYAN)make debug$(COLOR_RESET)             Build with debug symbols"
	@echo ""
	@echo "$(COLOR_BOLD)RUN TARGETS:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make run$(COLOR_RESET)               Run with default level (hard)"
	@echo "  $(COLOR_CYAN)make run-easy$(COLOR_RESET)          Run easy level"
	@echo "  $(COLOR_CYAN)make run-medium$(COLOR_RESET)        Run medium level"
	@echo "  $(COLOR_CYAN)make run-hard$(COLOR_RESET)          Run hard level"
	@echo "  $(COLOR_CYAN)make run-complex$(COLOR_RESET)       Run complex network level"
	@echo "  $(COLOR_CYAN)make run-ultra$(COLOR_RESET)         Run ultra complex level (FOG)"
	@echo ""
	@echo "$(COLOR_BOLD)TESTING TARGETS:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make test-determinism$(COLOR_RESET)  Test if outputs are deterministic"
	@echo "  $(COLOR_CYAN)make valgrind$(COLOR_RESET)          Check for memory leaks"
	@echo ""
	@echo "$(COLOR_BOLD)UTILITY TARGETS:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make install-deps$(COLOR_RESET)      Install SFML and dependencies"
	@echo "  $(COLOR_CYAN)make check-sfml$(COLOR_RESET)        Check if SFML is installed"
	@echo "  $(COLOR_CYAN)make help$(COLOR_RESET)              Show this help message"
	@echo ""
	@echo "$(COLOR_BOLD)EXAMPLES:$(COLOR_RESET)"
	@echo "  $(COLOR_YELLOW)make clean && make$(COLOR_RESET)             # Clean build"
	@echo "  $(COLOR_YELLOW)make run-ultra$(COLOR_RESET)                 # Test complex level"
	@echo "  $(COLOR_YELLOW)make test-determinism$(COLOR_RESET)          # Verify determinism"
	@echo "  $(COLOR_YELLOW)make debug && gdb ./switchback_rails$(COLOR_RESET) # Debug session"
	@echo ""
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"

# ============================================================================
# Banner
# ============================================================================

.PHONY: banner
banner:
	@echo ""
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_BOLD)$(COLOR_CYAN)          SWITCHBACK RAILS - COMPILATION$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Success message
# ============================================================================

.PHONY: success
success:
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)    BUILD SUCCESSFUL!$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)To run the simulation:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)./$(TARGET) data/levels/easy_level.lvl$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)Or use quick commands:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make run-easy$(COLOR_RESET)    - Easy level"
	@echo "  $(COLOR_CYAN)make run-medium$(COLOR_RESET)  - Medium level"
	@echo "  $(COLOR_CYAN)make run-hard$(COLOR_RESET)    - Hard level"
	@echo "  $(COLOR_CYAN)make run-complex$(COLOR_RESET)   - Ultra complex (FOG)"
	@echo ""
	@echo "$(COLOR_BOLD)For help:$(COLOR_RESET)"
	@echo "  $(COLOR_CYAN)make help$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BLUE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(COLOR_RESET)"
	@echo ""

# ============================================================================
# Code statistics
# ============================================================================

.PHONY: stats
stats:
	@echo "$(COLOR_CYAN)  Code Statistics:$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_YELLOW)Lines of code:$(COLOR_RESET)"
	@wc -l $(CORE_SOURCES) $(SFML_SOURCES) $(MAIN_SOURCE) $(CORE_HEADERS) $(SFML_HEADERS) 2>/dev/null | tail -1 | awk '{print "  Total: " $1 " lines"}' || echo "  Unable to calculate"
	@echo ""
	@echo "$(COLOR_YELLOW)File breakdown:$(COLOR_RESET)"
	@wc -l $(CORE_SOURCES) $(SFML_SOURCES) $(MAIN_SOURCE) $(CORE_HEADERS) $(SFML_HEADERS) 2>/dev/null | head -n -1 | awk '{printf "  %-30s %6d lines\n", $2, $1}' || echo "  No files found"
	@echo ""

# ============================================================================
# Archive project for submission
# ============================================================================

.PHONY: archive
archive: clean
	@echo "$(COLOR_CYAN)  Creating submission archive...$(COLOR_RESET)"
	@ROLLNO=$$(basename $$(pwd) | grep -o '[0-9]\+$$'); \
	if [ -z "$$ROLLNO" ]; then \
		echo "$(COLOR_RED)Error: Could not extract roll number from directory name$(COLOR_RESET)"; \
		echo "$(COLOR_YELLOW)Please name your directory: PF-Project-SwitchbackRails-{RollNo}$(COLOR_RESET)"; \
		exit 1; \
	fi; \
	ARCHIVE="SwitchbackRails-$$ROLLNO.zip"; \
	cd .. && zip -r "$$ARCHIVE" "$$(basename $$(pwd))" \
		-x "*.git*" -x "*.o" -x "*.swp" -x "*~" -x "*.zip"; \
	echo "$(COLOR_GREEN)✓ Archive created: $$ARCHIVE$(COLOR_RESET)"; \
	echo "$(COLOR_YELLOW)  Ready for submission to Google Classroom!$(COLOR_RESET)"

# ============================================================================
# Check submission requirements
# ============================================================================

.PHONY: check-submission
check-submission:
	@echo "$(COLOR_CYAN)  Checking submission requirements...$(COLOR_RESET)"
	@echo ""
	@ERROR=0; \
	echo "$(COLOR_YELLOW)Checking root files...$(COLOR_RESET)"; \
	for file in README.md Makefile libraries.sh main.cpp; do \
		if [ -f "$file" ]; then \
			echo "  $(COLOR_GREEN)✓$(COLOR_RESET) $file"; \
		else \
			echo "  $(COLOR_RED)✗$(COLOR_RESET) $file $(COLOR_RED)(MISSING)$(COLOR_RESET)"; \
			ERROR=1; \
		fi; \
	done; \
	echo ""; \
	echo "$(COLOR_YELLOW)Checking core/ files...$(COLOR_RESET)"; \
	for file in game.cpp game.h fileio.cpp fileio.h; do \
		if [ -f "$(CORE_DIR)/$file" ]; then \
			echo "  $(COLOR_GREEN)✓$(COLOR_RESET) core/$file"; \
		else \
			echo "  $(COLOR_RED)✗$(COLOR_RESET) core/$file $(COLOR_RED)(MISSING)$(COLOR_RESET)"; \
			ERROR=1; \
		fi; \
	done; \
	echo ""; \
	echo "$(COLOR_YELLOW)Checking sfml/ files...$(COLOR_RESET)"; \
	for file in render.cpp render.h; do \
		if [ -f "$(SFML_DIR)/$file" ]; then \
			echo "  $(COLOR_GREEN)✓$(COLOR_RESET) sfml/$file"; \
		else \
			echo "  $(COLOR_RED)✗$(COLOR_RESET) sfml/$file $(COLOR_RED)(MISSING)$(COLOR_RESET)"; \
			ERROR=1; \
		fi; \
	done; \
	echo ""; \
	echo "$(COLOR_YELLOW)Checking directories...$(COLOR_RESET)"; \
	for dir in core sfml data/levels Sprites out docs; do \
		if [ -d "$dir" ]; then \
			echo "  $(COLOR_GREEN)✓$(COLOR_RESET) $dir/"; \
		else \
			echo "  $(COLOR_RED)✗$(COLOR_RESET) $dir/ $(COLOR_RED)(MISSING)$(COLOR_RESET)"; \
			ERROR=1; \
		fi; \
	done; \
	echo ""; \
	echo "$(COLOR_YELLOW)Checking level files...$(COLOR_RESET)"; \
	LEVEL_COUNT=0; \
	for level in data/levels/*.lvl; do \
		if [ -f "$level" ]; then \
			LEVEL_COUNT=$((LEVEL_COUNT + 1)); \
			echo "  $(COLOR_GREEN)✓$(COLOR_RESET) $level"; \
		fi; \
	done; \
	if [ $LEVEL_COUNT -eq 0 ]; then \
		echo "  $(COLOR_RED)✗$(COLOR_RESET) No level files found!"; \
		ERROR=1; \
	else \
		echo "  $(COLOR_GREEN)Found $LEVEL_COUNT level file(s)$(COLOR_RESET)"; \
	fi; \
	echo ""; \
	echo "$(COLOR_YELLOW)Checking sprite files...$(COLOR_RESET)"; \
	SPRITE_COUNT=0; \
	for sprite in Sprites/*.png; do \
		if [ -f "$sprite" ]; then \
			SPRITE_COUNT=$((SPRITE_COUNT + 1)); \
		fi; \
	done; \
	if [ $SPRITE_COUNT -eq 0 ]; then \
		echo "  $(COLOR_RED)✗$(COLOR_RESET) No sprite files found!"; \
		ERROR=1; \
	else \
		echo "  $(COLOR_GREEN)✓$(COLOR_RESET) Found $SPRITE_COUNT sprite file(s)"; \
	fi; \
	echo ""; \
	if [ $ERROR -eq 0 ]; then \
		echo "$(COLOR_GREEN)✓ All requirements met!$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_RED)✗ Some requirements are missing!$(COLOR_RESET)"; \
	fi; \
	echo ""


