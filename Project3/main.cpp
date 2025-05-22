#include <iostream>
#include <windows.h>
#include <string>
#include <thread>

// 구현된 게임 헤더
#include "tetris.h"
#include "maze_solver.h"
#include "snake.h"
#include "sudoku.h"
#include "minesweeper.h"

using namespace std;

// 콘솔 텍스트 색상 설정
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 콘솔 창 크기 조절
void resizeConsole() {
    system("mode con: cols=100 lines=40");
}

// 콘솔 글자 크기 조절
void setFontSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    cfi.dwFontSize.X = width;    // 글자 가로
    cfi.dwFontSize.Y = height;   // 글자 세로
    wcscpy_s(cfi.FaceName, L"Consolas"); // 폰트 이름

    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// 게임 선택 UI 출력
void drawMenuUI(int color) {
    system("cls");
    setColor(color);
    cout << R"(

    ("`-''-/").___..--''"`-._                 *                             
     `6_ 6  )   `-.  (     ).`-.__.`)     *                  *             
     (_Y_.)'  ._   )  `._ `. ``-..-'             *                        
   _..`--'_..-_/  /--'_.' ,'                          *                   
  (il),-''  (li),'  ((!.-'                *                          *     
                                        ___                           
                           _[]_/____\__n_                            
                          |_____.--.__()_|                           
                          |LI  //# \\    |                           
                          |    \\__//    |                           
                          |     '--'     |         *                  
                     *    |"|"|"|"|"|"|"| |     *                      
                          |~~~~~~~~~~~~~~|           *                
                         ^^^^^^^^^^^^^^^^^^^^                          
                        /  /  /  /  /  /  /  /                         
                       /  /  /  /  /  /  /  /       *                  
                               ( )       *                              
    )" << endl;

    setColor(14);
    cout << "\n ──────────────────────────────────────────────────────────────────────────────\n";
    cout << "                            ## Select Your Mini Game! ##\n";
    cout << " ──────────────────────────────────────────────────────────────────────────────\n";

    setColor(10);
    cout << "\t[1] 테트리스\t\t[2] 지뢰찾기\t\t[3] 미로찾기 게임\n";
    cout << "\t[4] 스네이크 게임\t[5] 스도쿠\t\t[6] 행맨\n";
    cout << "\t[7] 묵찌빠\t\t[8] 슈팅 게임\t\t[9] 퐁 게임\n";
    cout << "\t[0] 종료\n";

    setColor(14);
    cout << " ──────────────────────────────────────────────────────────────────────────────\n";

    setColor(7);
    cout << "\n  ▶ 두근두근 당신의 선택은???(번호를 입력해주세요)>> ";
}

// 아직 구현 안 된 게임용 메시지
void playPlaceholderGame(const string& name) {
    system("cls");
    cout << "▶ [" << name << "] 게임은 아직 준비 중입니다!\n";
    system("pause");
}

int main() {
    setFontSize(20, 40);   //  글자 크기 조절
    resizeConsole();       //  콘솔 창 크기 조절

    int choice;
    while (true) {
        // 메뉴 번쩍이는 효과
        for (int blink = 0; blink < 3; ++blink) {
            drawMenuUI(11); Sleep(150);
            drawMenuUI(13); Sleep(150);
            drawMenuUI(10); Sleep(150);
        }

        cin >> choice;

        switch (choice) {
        case 1: playTetris(); break;
        case 2: playMinesweeper(); break;
        case 3: playMaze_Solver(); break;
        case 4: playSnake(); break;
        case 5: playSudoku(); break;
        case 6: playPlaceholderGame("행맨"); break;
        case 7: playPlaceholderGame("묵찌빠"); break;
        case 8: playPlaceholderGame("슈팅 게임"); break;
        case 9: playPlaceholderGame("퐁 게임"); break;
        case 0:
            cout << "게임을 종료합니다!\n";
            return 0;
        default:
            cout << "잘못된 입력입니다. 다시 선택해주세요.\n";
            system("pause");
        }
    }

    return 0;
}
