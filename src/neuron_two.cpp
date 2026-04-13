#include <iostream>
using namespace std;

float relu(float x){
    return x > 0 ? x : 0;
}
//สร้าง array
float neuron_array(float input[], float weight[], float bias, int size){
    float sum = 0;
    for(int i = 0; i < size; i++){
        sum += input[i] * weight[i]; //ทำลูป
    }
    return relu(sum + bias);
}

int main(){
    //input
    float x[2] = {1.0, 2.0};
    float w1[2] = {0.5, 0.3}; //neuron แรก
    float w2[2] = {0.2, 0.8}; //neuron สอง ใส่ input เดิม
    float h1 = neuron_array(x, w1, 0.1, 2);
    float h2 = neuron_array(x, w2, 0.0, 2);
    float h[2] = {h1, h2};
    float w3[2] = {0.6, 0.4};
    float output = neuron_array(h, w3, 0.2, 2);  
    cout << "h1: " << h1 << endl;
    cout << "h2: " << h2 << endl;
    cout << "output: " << output << endl;
    return 0;
}
