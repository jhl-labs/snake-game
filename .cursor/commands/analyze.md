---
name: 정적 분석
command: cppcheck --enable=all --inconclusive -Iinclude src/ 2>&1
description: cppcheck를 사용하여 코드를 정적 분석합니다.
cwd: ${workspaceFolder}
---

