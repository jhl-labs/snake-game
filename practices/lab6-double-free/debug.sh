#!/bin/bash
#
# GDB 디버깅 교육용 스크립트 - Double Free
# Claude Code를 사용하여 버그를 자동으로 분석하고 수정합니다.
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "========================================
  GDB 디버깅 교육 - Double Free
========================================"
echo

# 1. 버그가 있는 코드 복사
echo "[1/4] 버그가 있는 problem.c를 main.c로 복사..."
cp problem.c main.c
echo "완료"
echo

# 2. 빌드 및 크래시 확인
echo "[2/4] 빌드 후 크래시 확인..."
gcc -g -o buggy main.c
echo "실행 결과:"
./buggy 2>&1 || true
echo

# 3. Claude Code로 디버깅 및 수정 요청
echo "[3/4] Claude Code에게 gdb 디버깅 및 수정 요청..."
PROMPT="./buggy 실행 시 double free 오류가 발생해. gdb나 AddressSanitizer로 원인을 분석하고 main.c의 버그를 수정해줘. 수정 후 다시 빌드해서 정상 동작하는지 확인해줘."
echo "프롬프트: $PROMPT"
echo
echo "$PROMPT" | claude --dangerously-skip-permissions -p
echo

# 4. 수정 결과 검증
echo "[4/4] 수정된 프로그램 검증..."
echo "실행 결과:"
gcc -g -o buggy main.c
./buggy
echo

echo "========================================"
echo "  교육 완료!"
echo "========================================"
echo
echo "학습 포인트:"
echo "  - free() 후 포인터를 NULL로 설정"
echo "  - break free로 free 호출 추적"
echo "  - AddressSanitizer로 힙 오류 탐지"
echo
