---
name: 코드 포맷팅
command: clang-format -i src/**/*.c include/**/*.h 2>nul || echo clang-format이 설치되지 않았습니다. 수동으로 포맷팅해주세요.
description: 코드를 자동으로 포맷팅합니다 (clang-format 필요).
cwd: ${workspaceFolder}
---

