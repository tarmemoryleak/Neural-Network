#include <iostream>
#include <cmath>
using namespace std;

//relu
float relu(float x){
    return x > 0 ? x : 0; //ถ้า x > 0 return x ถ้าไม่ return 0
}

//1 neuron รับ input 2 ตัว
float neuron(float x1, float x2, float w1, float w2, float bias){
    float sum = x1 * w1 + x2 * w2 + bias;  
    return relu(sum);
}

int main(){
    //weight และ bias กำหนดเอง
    float w1 = 0.5;
    float w2 = 0.3;
    float bias = 0.1;
    float x1 = 1.0;
    float x2 = 2.0;
    float output = neuron(x1, x2, w1, w2, bias);
    cout << "Output: " << output << endl;   //ใส่ค่า
    return 0;
}
