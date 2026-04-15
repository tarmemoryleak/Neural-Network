#include <iostream>
#include <conio.h>
#include <windows.h>

// ไม่ใช้ using namespace std; เผื่อมีอะไรชนกันในอนาคต :)
// เกม flappy bird แบบกากๆ ใน terminal ยังไม่เสร็จ

int main(){
    const int WIDTH = 40;
    const int HEIGHT = 20; // i litearally forgot it need to use in main function so i put it here
    // นก x = 5
    const int BIRD_X = 5;
    int UI = 0; // หน้า UI เริ่มเกม
    std::cout << "Welcome to Flappy Bird in C++!\nPress [SPACE] to start..." << std::endl;
    float birdY = HEIGHT / 2.0; // ตำแหน่ง y
    float velocity = 0.0f; // ความเร็ว
    const float gravity = 0.5f; // ดึงลง
    const float flap = -2.5f; // กระพือปีก
    bool running = true;

    // รับ keyboard กด spacebar เริ่มเก
    while (true){
        if(_kbhit()) {
            char key = _getch();
            if (key == ' ') break;
            if (key == 'q')  return 0;
        }
    }
    // ปุ่มกระโดด 
    while (running){
        if(_kbhit()){
            char key = _getch();
            if (key == ' ') velocity = flap; // กระพือปีกอ๊าาาาา
            if (key == 'q')  running  = false;
        }
        velocity += gravity; // เพิ่มความเร็วจากแรงโน้มถ่วง
        birdY += velocity; // อัพเดตตำแหน่งนก
        if (birdY < 0 || birdY >= HEIGHT) {
            std::cout << "Game over!" << std::endl;
            break; // break
        }
        // วาดหน้าจอ
        system("cls"); 
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
    return 0;
}
