//โปรเจคนี้คือช่วงสมัยที่ผมฝึกเขียน Neural network กับ ฝึกเขียน RAG ไม่ได้สำคัญอะไรใช้งานไม่ได้ แค่ไฟล์นี้แค่ฝึกเขียนขึ้นมาเล่นๆ ผสมกับ neural 
//This project is where i was practicing writing neural networks and RAG, its nothing important and it's not functional just a file I created for practice combine it with neural networks.
#include <iostream>
#include <vector>
#include <cmath> 


float relu(float x) { //ReLU สั้นๆแบบโครตง่าย
    return x > 0 ? x : 0;
}

struct Neuron {
    std::vector<float> weights;
    float bias;

    float forward(const std::vector<float>& inputs) {
        float sum = bias;
        for (int i = 0; i < weights.size(); i++) {
            sum += weights[i] * inputs[i];
        }
        sum += bias; //เพิ่ม bias หลังจากคำนวณผลรวม
        return relu(sum);
    }
};


float cosine_similarity(const std::vector<float>& a, const std::vector<float>& b){ //หาค่าความเหมือน cosine similarity คล้ายๆระบบ RAG หาความใกล้เคียงของ vector
    float dot = 0.0;
    float norm_a = 0.0;
    float norm_b = 0.0;
    
    for(int i = 0; i < a.size(); i++) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
        
    }
    return dot / (sqrt(norm_a) * sqrt(norm_b));
    
}

float add(float a, float b) {
    return a + b;
}

int main(){

    Neuron n;
    n.weights = {0.2, 0.4, 0.6};
    n.bias = 0.1;
    std::cout<<"Hello TAR!";
    float similarity = 0.95;
    float result = add(0.3, 0.7);
    int dimension = 384;
    bool is_match = true;
    std::vector<float> embedding = {0.1, 0.8, 0.3, 0.5};
    std::vector<float> query = {0.1, 0.8, 0.3, 0.5};
    std::vector<float> doc1 = {0.1, 0.7, 0.4, 0.6};  //คล้ายมาก
    std::vector<float> doc2 = {0.9, 0.1, 0.0, 0.1};  //คล้ายน้อย
    std::vector<float> input1 = {1.0, 1.0, 1.0};
    std::vector<float> input2 = {0.0, 0.0, 5.0}; 
    
    std::cout<<"query vs doc1:"<<cosine_similarity(query, doc1)<<std::endl; //query 
    std::cout<<"query vs doc2:"<<cosine_similarity(query, doc2)<<std::endl;
    std::cout<<"size:"<<embedding.size() <<std::endl;
    std::cout<<"first:"<<embedding[0]<<std::endl;
    std::cout<<"last:"<<embedding[3]<<std::endl;
    std::cout<<"similarity:"<<similarity<<std::endl;
    std::cout<<"Result:" << result << std::endl;
    std::cout<<"dimension:"<<dimension<<std::endl;
    std::cout<<"is_match:"<<std::boolalpha<<is_match<<std::endl;
    
    for (float x :embedding) {
        std::cout << x << " ";   
    }
    std::cout << std::endl;
    std::cout<<"Neuron output for input1: "<<n.forward(input1)<<std::endl;
    std::cout<<"Neuron output for input2: "<<n.forward(input2)<<std::endl;
    return 0;
}
