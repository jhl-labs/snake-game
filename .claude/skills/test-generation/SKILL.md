---
name: test-generation
description: 테스트 케이스를 생성합니다. "테스트 작성", "TC 만들어", "write test" 요청 시 사용합니다.
allowed-tools: Read, Write, Glob
---

# Test Generation Skill

테스트 케이스를 생성합니다.

## 프로세스

### 1. 대상 분석
- 함수 시그니처
- 입력 파라미터
- 반환 값
- 사이드 이펙트

### 2. 시나리오 식별
- 정상 케이스 (Happy path)
- 경계 조건 (Boundary)
- 에러 케이스 (Error)
- 엣지 케이스 (Edge)

### 3. 테스트 코드 생성
AAA 패턴 사용:
- Arrange: 테스트 데이터 준비
- Act: 함수 실행
- Assert: 결과 검증

## 테스트 템플릿

```c
void test_<function>_<scenario>(void) {
    // Arrange
    <setup code>

    // Act
    <function call>

    // Assert
    assert(<condition>);
}
```
