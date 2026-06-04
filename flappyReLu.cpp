//ไฟล์นี้เป็นการเพิ่มระบบ deep q-network แบบง่ายๆ เข้ามาในเกม Flappy Bird ที่เราเคยทำกันไว้ก่อนหน้านี้ โดยผมปรับโครงสร้างสมองขึ้นเป็น 256 และใช้ ReLU เป็นฟังก์ชันกระตุ้น เพื่อให้ AI สามารถเรียนรู้และปรับตัวได้ดีขึ้นในการเล่นเกมนี้

//ตอนนี้ยังไม่มีระบบ save weight แบบ .bin หลังจากเทรนแล้วซึ่งขี้เกียจทำ ก็คือถ้าออกเกมแม่งก็ลืมหมดเลยที่เทรนไปคือเดี้ยงเลยเพราะงั้นเอาไว้ดูมันเล่นๆชั่วคราว

//ไฟล์นี้ compile ผ่าน vs code ได้เลยนะครับ หรือจะใช้ g++ ก็ได้เช่นกัน แล้วแต่ถนัดของแต่ละคน จะได้ไฟล์ .exe ออกมา 

//เกมนี้เล่นใน Terminal หรือ cmd เท่านั้น ไม่มีกราฟิกอะไรเลย แต่ผมทำให้มันแสดงผลแบบง่ายๆใน console เพื่อให้เห็นการทำงานของ AI ได้ชัดเจนขึ้น 

//Human mode ยังมีบัคใช้งานไม่ได้ ไว้แก้ที่หลังตอนนี้ผมเน้นให้ AI เล่นได้ก่อน เนื่องจาก gravity หรือ physic ผมดันใช้ร่วมกับ ai mode เลยทำให้โหมดคนเล่นใช้ logic ผิดพลาดต้องปรับใหม่อีกที

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

struct MemoryFrame {
    float s_birdY, s_vel, s_pipeDist, s_pipeGapY;
    int action;       
    float reward;     
    float next_s_birdY, next_s_vel, next_s_pipeDist, next_s_pipeGapY;
    bool dead;        
};

const int MAX_MEMORY = 10000;
MemoryFrame replay_buffer[MAX_MEMORY];
int memory_counter = 0;

void Remember(MemoryFrame f) {
    replay_buffer[memory_counter % MAX_MEMORY] = f;
    memory_counter++;
}

//Deep Q-Network กับ ReLU 
class DeepBrain {
public:
    //ปรับสมองเป็น 256 เลยแต่ยังเล่นไม่เก่งมาก Best ที่ผมลองได้มากสุดแค่ 4 ครั้งที่ผ่านท่อได้ต้องปรับค่า reward หรือข้างในใหม่อีกเยอะ
    float w1[256][4];
    float b1[256];
    float w2[2][256];
    float b2[2];

    DeepBrain() {
        for(int i = 0; i < 256; i++) {
            b1[i] = (((float)rand()/RAND_MAX) * 1.0f - 0.5f) * sqrt(2.0f / 4.0f);
            for(int j = 0; j < 4; j++) w1[i][j] = (((float)rand()/RAND_MAX) * 1.0f - 0.5f) * sqrt(2.0f / 4.0f);
        }
        for(int i = 0; i < 2; i++) {
            b2[i] = (((float)rand()/RAND_MAX) * 1.0f - 0.5f) * sqrt(2.0f / 256.0f);
            for(int j = 0; j < 256; j++) w2[i][j] = (((float)rand()/RAND_MAX) * 1.0f - 0.5f) * sqrt(2.0f / 256.0f);
        }
    }

    void Forward(float birdY, float vel, float pipeDist, float pipeGapY, float* out_Q) {
        float h_input[256];
        float h_output[256]; 

        for(int i = 0; i < 256; i++) {
            h_input[i] = (birdY * w1[i][0]) + (vel * w1[i][1]) + (pipeDist * w1[i][2]) + (pipeGapY * w1[i][3]) + b1[i];
            h_output[i] = (h_input[i] > 0.0f) ? h_input[i] : 0.0f; //ReLU 256 
        }

        for(int i = 0; i < 2; i++) {
            float sum = b2[i];
            for(int j = 0; j < 256; j++) {
                sum += h_output[j] * w2[i][j];
            }
            out_Q[i] = sum; 
        }
    }
    //เทรนแบบง่ายๆ
    void Train(MemoryFrame f, float learning_rate = 0.02f, float gamma = 0.95f) {
        float current_Q[2];
        Forward(f.s_birdY, f.s_vel, f.s_pipeDist, f.s_pipeGapY, current_Q);

        float next_Q[2];
        Forward(f.next_s_birdY, f.next_s_vel, f.next_s_pipeDist, f.next_s_pipeGapY, next_Q);
        float max_next_Q = (next_Q[0] > next_Q[1]) ? next_Q[0] : next_Q[1];

        float target = f.reward; 
        if (!f.dead) {
            target += gamma * max_next_Q; 
        }

        float error = target - current_Q[f.action];

        float h_input[256], h_output[256];
        for(int i = 0; i < 256; i++) {
            h_input[i] = (f.s_birdY * w1[i][0]) + (f.s_vel * w1[i][1]) + (f.s_pipeDist * w1[i][2]) + (f.s_pipeGapY * w1[i][3]) + b1[i];
            h_output[i] = (h_input[i] > 0.0f) ? h_input[i] : 0.0f;
        }

        float d_output = error; 
    
        for(int j = 0; j < 256; j++) {
            w2[f.action][j] += learning_rate * d_output * h_output[j];
        }
        b2[f.action] += learning_rate * d_output;

        for(int i = 0; i < 256; i++) {
            if (h_input[i] > 0.0f) { 
                float d_hidden = d_output * w2[f.action][i];
                w1[i][0] += learning_rate * d_hidden * f.s_birdY;
                w1[i][1] += learning_rate * d_hidden * f.s_vel;
                w1[i][2] += learning_rate * d_hidden * f.s_pipeDist;
                w1[i][3] += learning_rate * d_hidden * f.s_pipeGapY;
                b1[i] += learning_rate * d_hidden;
            }
        }
    }
};

float birdY; 
float velocity; 
bool running = true;
bool isAiMode = true; 

const int WIDTH = 40;
const int HEIGHT = 20; 


const float gravity = 0.2f;  //ขออนุญาติปรับ gravity ลดลงเนื่องจากอยากให้นกได้บินเล่นบ้างไม่งั้นแม่งเดี้ยงรัวๆ
const float flap = -1.3f;    

int pipeX;
int gapTop;
const int GAP_SIZE = 7; //ถ้าอยากให้ท่อกว้างขึ้นก็ปรับตรงนี้ขึ้นเอา แต่ผมเลือก 7 กำลังดีมาตรฐานชายไทย
int score;
int highScore = 0; 
int generation = 0; 
const int BIRD_X = 5;

//ระบบควบคุมอัตราการมั่ว
float epsilon = 0.8f;          
const float epsilon_decay = 0.998f; 
const float epsilon_min = 0.05f;   

void Spawnpipe(){
    pipeX = WIDTH - 1; 
    gapTop = 3 + rand() % (HEIGHT - GAP_SIZE - 6); 
}

bool Hitpipe(){
    if (pipeX != BIRD_X) return false; 
    int by = (int)birdY;
    if (by >= gapTop && by < gapTop + GAP_SIZE) return false; 
    return true; 
}

void WaitRestartHuman() {
    while (true){
        if(_kbhit()){
            char key = _getch();
            if (key == ' ') break; 
            if (key == 'q' || key == 'Q') exit(0); 
        }
    }
}

void CheckManualQuit() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'q' || key == 'Q') exit(0);
    }
}

DeepBrain AI; 

int main(){
    srand((unsigned)time(0)); 
    
    system("cls");
    std::cout << "======================================\n";
    std::cout << "    TAR-01 FLAPPY DEEP LEARNING 64    \n";
    std::cout << "======================================\n\n";
    std::cout << " [1] HUMAN MODE\n";
    std::cout << " [2] AI DEEP Q-NETWORK 64 NODES MODE\n\n";
    std::cout << " Select mode (press 1 or 2): ";
    //ไม่ใช้ using namespace std; เหมือนเดิม ถ้าใช้ using namespace std เป็น habit ที่ไม่ดีเท่าไหร่แต่ก็ใช้ได้แล้วแต่คนชอบ แต่ผมขอไม่ใช้
    while(true) {
        if(_kbhit()) {
            char choice = _getch();
            if(choice == '1') { isAiMode = false; break; }
            if(choice == '2') { isAiMode = true;  break; }
        }
    }

    while (true){
        generation++;
        birdY = HEIGHT / 2.0; 
        velocity = 0.0f; 
        score = 0; 
        running = true;
        Spawnpipe(); 
        
        if (isAiMode && epsilon > epsilon_min) {
            epsilon *= epsilon_decay;
        }

        if (!isAiMode) {
            system("cls");
            std::cout << "Welcome to Flappy Bird (HUMAN MODE)! \nPress [SPACE] to fly..." << std::endl;
            WaitRestartHuman();
        }

        while (running){
            CheckManualQuit(); 

            MemoryFrame frame;
            frame.s_birdY = birdY;
            frame.s_vel = velocity;
            frame.s_pipeDist = (float)(pipeX - BIRD_X);
            frame.s_pipeGapY = (float)gapTop;
            frame.dead = false;
            frame.action = 0;

            if (isAiMode) {
                float q_values[2];
                AI.Forward(frame.s_birdY, frame.s_vel, frame.s_pipeDist, frame.s_pipeGapY, q_values);

                if (((float)rand()/RAND_MAX) < epsilon) {
                    frame.action = rand() % 2;
                } else {
                    frame.action = (q_values[1] > q_values[0]) ? 1 : 0; 
                }
                if (frame.action == 1) velocity = flap; 
            } else {
                for (int delay = 0; delay < 5; delay++) {
                if (_kbhit()) {
                    char key = _getch();
                    if (key == ' ') {
                        velocity = flap;
                        frame.action = 1; 
                    }
                    if (key == 'q' || key == 'Q') exit(0);
                }
                Sleep(10);
            }
        }

            velocity += gravity; 
            birdY += velocity; 
            pipeX--; 

            //ตรวจสอบคะแนนและป้อนผลลัพธ์ Reward
            if (pipeX < -1) {
                score++;
                if (score > highScore) highScore = score;
                Spawnpipe(); 
                frame.reward = 100.0f;
            } else {
                frame.reward = 0.1f; 
            }

            if (birdY < 0 || birdY >= HEIGHT || Hitpipe()) {
                frame.reward = -30.0f; 
                frame.dead = true;
                running = false; 
            }

            frame.next_s_birdY = birdY;
            frame.next_s_vel = velocity;
            frame.next_s_pipeDist = (float)(pipeX - BIRD_X);
            frame.next_s_pipeGapY = (float)gapTop;

            if (isAiMode) {
                Remember(frame); 
            }

            std::cout << "\x1b[H"; 
            std::cout << "\x1b[1;" << (WIDTH - 15) << "H";
            if (isAiMode) {
                std::cout << "[AI] Gen:" << generation << " Score:" << score << " Best:" << highScore << " (Eps:" << epsilon << ")\n";
            } else {
                std::cout << "[HUMAN] Score:" << score << " Best:" << highScore << "\n";
            }

            for (int y = 0; y < HEIGHT; y++){
                for (int x = 0; x < WIDTH; x++){
                    if (x == BIRD_X && (int)birdY == y) {
                        std::cout << ">"; 
                    } else if (x == pipeX){
                        bool inGap = (y >= gapTop && y < gapTop + GAP_SIZE);
                        std::cout << (inGap ? " " : "|");
                    } else {
                        std::cout << " "; 
                    }
                }
                std::cout << "|\n";
            }
            for (int i = 0; i < WIDTH; i++) std::cout << "-"; 
            std::cout << "\n" << (isAiMode ? "[AI Mode Training...] Hold [Q] to Quit." : "[Human Mode] Press [SPACE] to Flap / [Q] to Quit.") << std::endl; 
            
            Sleep(isAiMode ? 30 : 50); 
        }

        if (isAiMode) {
            int batch_size = (memory_counter < 128) ? memory_counter : 128;
            for(int i = 0; i < batch_size; i++) {
                int rand_index = rand() % (memory_counter > MAX_MEMORY ? MAX_MEMORY : memory_counter);
                AI.Train(replay_buffer[rand_index], 0.01f); 
            }
        } else {
            system("cls");
            std::cout << "=========================\n";
            std::cout << "       GAME OVER         \n";
            std::cout << "=========================\n";
            std::cout << " Score: " << score << "   Best: " << highScore << "\n\n";
            std::cout << " Press [SPACE] to play again or [Q] to quit..." << std::endl;
            WaitRestartHuman();
        }
    }
    return 0;
}
