---
name: 구문 검사
command: gcc -fsyntax-only -Iinclude src/*.c 2>&1
description: 코드의 구문 오류를 검사합니다 (컴파일 없이).
cwd: ${workspaceFolder}
---

