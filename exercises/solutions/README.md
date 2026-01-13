# Solutions - 모범 답안

> 교육자용 자료입니다. 수강생에게는 비공개로 유지하세요.

## 파일 목록

| 파일 | 설명 |
|------|------|
| `buggy_snake_fixed.c` | 버그 수정 실습 답안 |

## 버그 수정 답안 요약

### Bug #1: NULL 포인터 체크 누락
- **위치**: `snake_init()`
- **수정**: 함수 시작에 `if (p_snake == NULL) return false;` 추가

### Bug #2: 배열 경계 체크 누락
- **위치**: `snake_grow()`
- **수정**: `if (p_snake->length >= MAX_SNAKE_LENGTH) return false;` 추가

### Bug #3: Off-by-one 에러
- **위치**: `is_wall_collision()`
- **수정**: `x > GRID_WIDTH` → `x >= GRID_WIDTH`

### Bug #4: 방향 반전 체크 누락
- **위치**: `snake_set_direction()`
- **수정**: 반대 방향 체크 로직 추가

### Bug #5: 이동 로직 오류
- **위치**: `snake_move()` - LEFT 케이스
- **수정**: `p_snake->body[0].x++` → `p_snake->body[0].x--`

### Bug #6: 자기 충돌 체크 범위 오류
- **위치**: `is_self_collision()`
- **수정**: `for (int i = 0;` → `for (int i = 1;`

### Bug #7: 메모리 누수
- **위치**: `get_direction_name()`
- **수정**: `malloc` 제거, 정적 문자열 반환으로 변경
