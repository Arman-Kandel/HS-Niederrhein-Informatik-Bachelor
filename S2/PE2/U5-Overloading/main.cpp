#include <iostream>
#include "vec2.h"

int main(int argc, char *argv[]) {
    // vec2 tests:
    vec2 v1(2, 2), v2(2, 2);
    if(v1 * v2 != 8) {
        std::cerr << v1*v2;
        std::cerr << " <- Error at multiplication\n";
        return 1;
    }
    if(v1 + v2 != vec2(4, 4)){
        std::cerr << v1;
        std::cerr << " <- Error at plus\n";
        return 1;
    }
    if(v1 - v2 != vec2(0, 0)){
        std::cerr << v1;
        std::cerr << " <- Error at minus\n";
        return 1;
    }
    if(v1.rotate(90) != vec2(-2, -2)){
        std::cerr << v1;
        std::cerr << " <- Error at rotation\n";
        return 1;
    }
    v1.x = 10;
    v1.y = 20;
    if(v1[0] != 10 || v1[1] != 20){
        std::cerr << v1;
        std::cerr << " <- Error at get by index\n";
        return 1;
    }
    return 0;
}