#include <iostream>
#include <conio.h>
#include <windows.h>

// ไม่ใช้ using namespace std; เผื่อมีมีอะไรชนกันในอนาคต :)
// ขนาดหน้าจอ
float birdY; // ตำแหน่ง y ของนก
float velocity; // ความเร็วของนก
bool running = true;
const int WIDTH = 40;
const int HEIGHT = 20; // i move float and bool out of main to make it global variable make it's readable for me it's might not good for big project
const float gravity = 0.5f; // ดึงลง
const float flap = -2.5f; // กระพือปีก

// นก x = 5
const int BIRD_X = 5;
void Restart() {
    while (true){
        if(_kbhit()){
            char key = _getch();
            if (key == ' ') break; // กด space เพื่อเริ่มใหม่อีกรอบ
            if (key == 'q') exit(0); // กด q เพื่อออกจากเกม 
        }
    }
}

int main(){
    // ลบ bool กับ float ออกจาก main แล้วไปไว้ข้างบนเพื่อให้มันดูเป็น global variable แทน 
    while (true){
        birdY = HEIGHT / 2.0; // ตำแหน่ง y
        velocity = 0.0f; // ความเร็ว
        running = true;
        system("cls");
        std::cout << "Welcome to Flappy Bird in C++!\nPress [SPACE] to start..." << std::endl;
        Restart();

    while (running){
        if(_kbhit()){
            char key = _getch();
            if (key == ' ') velocity = flap; // กระพือปีก
            if (key == 'q')  { running  = false; break; }
        }
        velocity += gravity; // เพิ่มความเร็วจากแรงโน้มถ่วง
        birdY += velocity; // อัพเดตตำแหน่งนก
        if (birdY < 0 || birdY >= HEIGHT) {
            system("cls");
            std::cout << "Game over press [SPACE] to restart or [Q] to quit." << std::endl;
            running = false;
            break; // break
        }
        // วาดหน้าจอ
        std::cout << "\x1b[H"; // เอา system cls ออกแล้วใช้ ansi แทนเพื่อให้มันไม่กระพริบแม่งแสบตาทีนี้จะเห็นเกมไม่กระพริบอีกแล้ว
        for (int y = 0; y < HEIGHT; y++){
            for (int x = 0; x < WIDTH; x++){
                if (x == BIRD_X && (int)birdY == y)
                    std::cout << ">"; // นก
                else
                    std::cout << " "; // พื้นหลัง
            }
            std::cout << "|\n";
        }
        for (int i = 0; i < WIDTH; i++) std::cout << "-"; 
        std::cout << "[SPACE] Flap  [Q] Quit" << std::endl; 
        Sleep(50); // หน่วงเวลา
    }
    Restart();
    }
    return 0;
}
