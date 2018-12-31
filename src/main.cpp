//
// Created by tomer on 31/12/18.
//
#include <iostream>
int main (int argc, char *argv[]) {
        char g[3];
        g[0] = '1';
        std::cout<< f(g); <<std::endl
        return 0;
    }
    char* f(char* g){
    char* output[5];
    for(int i =0; i<5;i++){
        output[i] = &g[i];
    }
    return *output;

}