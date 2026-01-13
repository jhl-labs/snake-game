---
description: 정적 분석을 수행합니다
allowed-tools: Bash(cppcheck:*)
---

코드를 정적 분석해주세요.

```bash
cppcheck --enable=all --inconclusive -Iinclude src/ 2>&1
```

발견된 문제점을 분석하고 심각도별로 정리해주세요.
