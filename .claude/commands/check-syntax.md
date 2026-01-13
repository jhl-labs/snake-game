---
description: 코드 구문 오류를 검사합니다
allowed-tools: Bash(gcc:*)
---

코드의 구문 오류를 검사해주세요.

```bash
gcc -fsyntax-only -Iinclude -Iexternal/raylib/src src/*.c 2>&1
```
