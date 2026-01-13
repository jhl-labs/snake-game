---
alwaysApply: true
description: 프로젝트의 전체 아키텍처 설계 원칙과 MVC 패턴 기반의 모듈 분리 규칙을 정의합니다.
---

# 아키텍처 원칙

## 프로젝트 개요
- **언어**: C (C11 표준)
- **라이브러리**: raylib
- **빌드 시스템**: Make (MinGW-w64)
- **플랫폼**: Windows
- **아키텍처**: MVC 패턴 (Model-View-Controller 분리)

## 일반 원칙
1. **SOLID 원칙 준수**: 모든 클래스와 모듈은 SOLID 원칙을 따라야 함
2. **관심사 분리**: 모델(Model), 로직(Logic), 뷰(View), 컨트롤러(Controller)를 명확히 분리
3. **단일 책임 원칙**: 각 모듈은 하나의 명확한 책임만 가져야 함
4. **의존성 역전**: 구체적인 구현보다 추상화에 의존

## 모듈 분리 원칙

### Model 계층
- 게임 상태 데이터 구조체
- 데이터 조작 함수 (예: `snake.c`, `food.c`)
- raylib에 의존하지 않음

### Logic 계층
- 게임 규칙 및 로직 (예: `game.c`)
- 충돌 감지, 점수 계산 등
- raylib에 의존하지 않음

### View 계층
- 렌더링 로직 (예: `renderer.c`)
- raylib 함수 사용
- 모델 데이터를 화면에 표시

### Controller 계층
- 입력 처리 (예: `input.c` 또는 `main.c`에 통합)
- 사용자 입력을 게임 로직으로 변환
- raylib 입력 함수 사용 가능
