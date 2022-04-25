#include <iostream>


template <typename T> T min(T t1, T t2){
    return t1 < t2 ? t1 : t2;
}

template <typename T> void swap(T* t1, T* t2){
    T temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}

int main(int argc, char *argv[]) {
    if(min<int>(10, 1) != 1){
        std::cerr << "Failed to determine minimum!\n";
        return -1;
    }
    int a = 10, b = 5;
    swap<int>(&a, &b);
    if(b != 10){
        std::cerr << "Failed to swap values!\n";
        return -1;
    }
    return 0;
}