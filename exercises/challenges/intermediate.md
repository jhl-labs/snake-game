# Challenge: Intermediate Level

중급 난이도 도전 과제입니다.

---

## Challenge 4: 벽 통과 모드

### 목표
벽에 부딪히면 반대편으로 나오는 모드를 추가합니다.

### 요구사항
- 토글 가능한 옵션
- 왼쪽 벽 → 오른쪽으로 나옴
- 위쪽 벽 → 아래쪽으로 나옴

### 힌트
```
AI Agent에게:
"game.c에 wrap_around_mode 옵션을 추가해서,
 활성화되면 벽을 통과할 수 있게 해줘"
```

### 고려사항
- 기존 벽 충돌 로직과의 분리
- 게임 설정에 모드 추가

---

## Challenge 5: 장애물 추가

### 목표
맵에 랜덤 장애물을 배치합니다.

### 요구사항
- 게임 시작 시 3-5개의 장애물 생성
- 장애물과 충돌 시 게임 오버
- 음식은 장애물 위에 생성되지 않음

### 힌트
```
AI Agent에게:
"새로운 obstacle.c 모듈을 만들고,
 장애물 생성 및 충돌 감지를 구현해줘"
```

### 예상 새 파일
- `include/obstacle.h`
- `src/obstacle.c`

---

## Challenge 6: 일시정지 기능

### 목표
게임 일시정지/재개 기능을 구현합니다.

### 요구사항
- P 키로 일시정지/재개 토글
- 일시정지 시 "PAUSED" 표시
- 일시정지 중 방향키 무시

### 힌트
```
AI Agent에게:
"GameState enum에 GAME_STATE_PAUSED를 추가하고,
 P키로 토글하는 기능을 구현해줘"
```

---

## Challenge 7: 유닛 테스트 작성

### 목표
핵심 함수들에 대한 유닛 테스트를 작성합니다.

### 요구사항
- snake_init, snake_move, snake_grow 테스트
- 경계 조건 테스트 포함
- 에러 케이스 테스트 포함

### 힌트
```
AI Agent에게:
"exercises/test_example.c를 참고해서
 src/snake.c의 모든 함수에 대한 테스트를 작성해줘"
```

---

## 완료 체크리스트

- [ ] Challenge 4 완료
- [ ] Challenge 5 완료
- [ ] Challenge 6 완료
- [ ] Challenge 7 완료
- [ ] 모든 테스트 통과
