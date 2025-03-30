#include <cstdint>
#include <iostream>
#include <fstream>
#include <bitset>

#define LENGTH 1
#define MAXLEN 100
#define STUDENT_ID_LAST_DIGIT 7

int16_t module(int16_t A , int16_t B, int16_t *reminder);
int16_t multi(int16_t a , int16_t b);
void Put(int16_t *state, int16_t *move, int16_t *memory,int16_t n);
void Remove(int16_t *state, int16_t *move, int16_t *memory, int16_t n);

int16_t lab1(int16_t n) {
    int16_t mask = 1;
    int16_t result = 0;
    if ((mask & n) > 0){
        for (int i = 0 ; i < 16 ; i++){
            if ((mask & n) == 0){
                result++;
            }
            mask += mask;
        }
    }else{
        for (int i = 0; i < 16; i++){
            if ((mask & (0 - n)) == 0){
                result++;
            }
            mask += mask;
        }
    }

    return result + STUDENT_ID_LAST_DIGIT;
}

int16_t lab2(int16_t n) {
    int16_t dn = 0;
    int16_t vn = 3;
    
    int16_t index = 1;
    int16_t reminder;
    while ((index++ - n) != 0){
        if (dn == 0){
            module((multi(vn, 2) + 2), 4096, &vn);
        }else{
            module((multi(vn, 2) - 2), 4096, &vn);
        }
        module(vn, 8, &reminder);
        if (reminder == 0){
            dn = 1 - dn;
            continue;
        }
        module(vn, 10, &reminder);
        if ((reminder - 8) == 0){
            dn = 1 - dn;
        }
    }
    
    return vn;
}

int16_t lab3(char s1[], char s2[]) {
    int16_t difference = 0;
    int index = 0;
    while (1){
        difference = s1[index] - s2[index];
        if (difference != 0){
            break;
        }
        if (s1[index] == 0 || s2[index] == 0){
            break;
        }
        index++;
    }
    
    return difference;
}

int16_t lab4(int16_t *memory, int16_t n) {
    int16_t state = 0;
    int16_t move = 0;
    Remove (&state, &move, memory, n);
    return move;
}

int main() {
    std::fstream file;
    file.open("test.txt", std::ios::in);

    // lab1
    int16_t n = 0;
    std::cout << "===== lab1 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab1(n) << std::endl;
    }

    // lab2
    std::cout << "===== lab2 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab2(n) << std::endl;
    }

    // lab3
    std::cout << "===== lab3 =====" << std::endl;
    char s1[MAXLEN]; char s2[MAXLEN];
    for (int i = 0; i < LENGTH; ++i) {
        file >> s1 >> s2;
        std::cout << lab3(s1, s2) << std::endl;
    }
    
    // lab4
    std::cout << "===== lab4 =====" << std::endl;
    int16_t memory[MAXLEN], move;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        int16_t state = 0;
        move = lab4(memory, n);
        for(int j = 0; j < move; ++j){
            std::cout << std::bitset<16>(memory[j]) << std::endl;
        }
    }
    
    file.close();
    return 0;
}

int16_t multi(int16_t a , int16_t b){
    int16_t result = a;
    while ((b-- - 1) > 0){
        result += a;
    }
    return result;
}

int16_t module(int16_t A , int16_t B, int16_t *reminder){
    int16_t result = 0;
    while ((A -= B) >= 0){
        result++;
    }
    *reminder = A + B;
    return result;
}

void Remove(int16_t *state, int16_t *move, int16_t *memory, int16_t n){
    int16_t mask = 1;
    if (n == 0) return ;
    if (n == 1){
        *state += 1;
        memory[*move] = *state;
        *move += 1;
        return ;
    }else{
        Remove (state, move, memory, n - 2);
        for (int i = 1 ; i < n ; i++){
            mask += mask;
        }
        *state += mask;
        memory[*move] = *state;
        *move += 1;
        Put(state, move, memory, n - 2);
        Remove(state, move, memory, n - 1);
    }
}

void Put(int16_t *state, int16_t *move, int16_t *memory,int16_t n){
    int16_t mask = 1;
    if (n == 0) return ;
    if (n == 1){
        *state -= 1;
        memory[*move] = *state;
        *move += 1;
        return ;
    }else{
        Put (state, move, memory, n - 1);
        Remove (state, move, memory, n - 2);
        for (int i = 1 ; i < n ; i++){
            mask += mask;
        }
        *state -= mask;
        memory[*move] = *state;
        *move += 1;
        Put(state, move, memory, n - 2);
    }
}