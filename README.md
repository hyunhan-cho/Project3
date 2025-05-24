🎮 C++ Mini Game Project
콘솔 기반의 다양한 미니게임을 하나의 프로젝트로 통합한 C++ 학습 프로젝트입니다.

🧩 구성된 게임 목록
Tetris : 블럭 회전 및 라인 제거 기능 구현

Maze Solver : DFS를 이용한 미로 자동 탐색

Minesweeper : 지뢰 찾기 및 주변 지뢰 수 표시

Sudoku : 사용자 입력 기반 퍼즐 검증 기능 포함

Snake (예정) : 게임 루프 및 충돌 처리 구현 예정

🗂️ 폴더 구조
bash
복사
편집
/MiniGameProject
├── main.cpp              # 게임 메뉴 및 메인 실행 루프
├── tetris/
│   ├── Tetris.cpp
│   └── Tetris.h
├── maze/
│   ├── Maze.cpp
│   └── Maze.h
├── minesweeper/
│   ├── Minesweeper.cpp
│   └── Minesweeper.h
├── sudoku/
│   ├── Sudoku.cpp
│   └── Sudoku.h
└── ...
🧠 기술 스택
언어: C++

툴: Visual Studio 2022, GitHub

컴파일러: MSVC

버전관리: Git / GitHub

🚀 실행 방법
Visual Studio로 main.cpp 오픈

빌드 후 실행 (F5)

콘솔 메뉴를 통해 게임 선택

👨‍👩‍👧‍👦 팀원 (예시)
조현한 — 팀장 / 구조 설계, Tetris구현

김상택 — Sudoku 구현

부서연  — Snake 구현

 조은영 — Maze구현

📌 기타
각 게임은 독립된 헤더 및 소스 파일로 구성되어 유지보수가 쉽습니다.

콘솔 환경에서도 사용자 인터랙션이 가능한 구조로 개발했습니다.
