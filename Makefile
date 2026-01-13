# Makefile for Snake Game
# MinGW-w64, raylib

# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
INCLUDES = -Iinclude -Iexternal/raylib/src
LIBS = -Llib/mingw -lraylib -lopengl32 -lgdi32 -lwinmm

# DEBUG 빌드 설정
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
    TARGET = $(BIN_DIR)/snake_debug.exe
else
    CFLAGS += -O2
    TARGET = $(BIN_DIR)/snake.exe
endif

# 디렉토리
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin
TEST_DIR = tests

# 소스 파일
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/game.c \
          $(SRC_DIR)/snake.c \
          $(SRC_DIR)/food.c \
          $(SRC_DIR)/renderer.c \
          $(SRC_DIR)/intro.c

# 객체 파일
OBJECTS = $(SOURCES:.c=.o)

# 테스트
TEST_TARGET = $(BIN_DIR)/test_snake.exe

# 기본 타겟
all: $(TARGET)

# 실행 파일 빌드
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

# 객체 파일 빌드
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(SRC_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 디렉토리 생성
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 테스트 빌드 및 실행
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_DIR)/test_snake.c $(SRC_DIR)/snake.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# 정리
clean:
	@powershell -Command "if (Test-Path '$(SRC_DIR)\*.o') { Remove-Item '$(SRC_DIR)\*.o' -Force }" 2>nul
	@powershell -Command "if (Test-Path '$(BIN_DIR)\*.exe') { Remove-Item '$(BIN_DIR)\*.exe' -Force }" 2>nul

# 재빌드
rebuild: clean all

# 실행
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run test
