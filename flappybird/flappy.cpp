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
// ท่อ
int pipeX;
int gapTop;
const int GAP_SIZE = 6; // ขนาดช่องว่างระหว่างท่อบนกับท่อล่าง ปรับตรงนี้ได้ตามใจชอบเลยจ้าา
// คะแนน 
int score;
int highScore = 0; // high score เก็บสถิติสูงสุดไว้ใน interger 
// นก x = 5
const int BIRD_X = 5;
void Spawnpipe(){
    pipeX = WIDTH - 1; 
    gapTop = 3 + rand() % (HEIGHT - GAP_SIZE - 6); // ไม่ให้ช่องว่างมันอยู่ติดขอบมากเกินไป
}

bool Hitpipe(){
    if (pipeX != BIRD_X) return false; // ถ้าไม่ชนท่อก็ไม่ต้องเช็คอะไร
    int by = (int)birdY;
    if (by >= gapTop && by < gapTop + GAP_SIZE) return false; 
    return true; // ชนท่อ
}
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
    srand((unsigned)time(0)); // seed สุ่มท่อ
    while (true){
        birdY = HEIGHT / 2.0; // ตำแหน่ง y
        velocity = 0.0f; // ความเร็ว
        running = true;
        Spawnpipe(); // เพิ่ม spawn pipe 
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
        pipeX--; // เคลื่อนท่อไปทางซ้าย
        if (pipeX < -1) {
            score++;
            if (score > highScore)
            highScore = score;
            Spawnpipe(); // อัพเดต high score ถ้าคะแนนปัจจุบันมากกว่า
        }
        if (birdY < 0 || birdY >= HEIGHT || Hitpipe()) {
            system("cls");
            std::cout << "Game Over!\n";
            std::cout << "Score: " << score << "  Best: " << highScore << "\n";
            std::cout << "Press [SPACE] to play again or [Q] to quit." << std::endl;
            running = false;
            break; // break
        }
        // วาดหน้าจอ
        std::cout << "\x1b[H"; // เอา system cls ออกแล้วใช้ ansi แทนเพื่อให้มันไม่กระพริบแม่งแสบตา
        for (int y = 0; y < HEIGHT; y++){
            for (int x = 0; x < WIDTH; x++){
                if (x == BIRD_X && (int)birdY == y) {
                    std::cout << ">"; // นก
                } else if (x == pipeX){
                bool inGap = (y >= gapTop && y < gapTop + GAP_SIZE);
                std::cout << (inGap ? " " : "|");
                }   else {
                    std::cout << " "; // พื้นหลัง
                }
            }
            std::cout << "|\n";
        }
        // เส้นล่าง กับ score
        for (int i = 0; i < WIDTH; i++) std::cout << "-"; 
        std::cout << "[SPACE] Flap  [Q] Quit" << std::endl; 
        Sleep(50); // หน่วงเวลา
    }
    Restart();
    }
    return 0;
}
