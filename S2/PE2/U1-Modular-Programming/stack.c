//
// Created by arman on 21.03.2022.
//

#include <stdlib.h>
#include "stack.h"

#define SIZE(x)  (sizeof(x) / sizeof((x)[0]))

struct stack_t{
    int size, countValues, next;
    float* values;
    char error;
};

stack_t* stackNew(){
    stack_t* s;
    s = (stack_t*) calloc(1, sizeof(stack_t));
    s->next = 0;
    s->size = 8;
    s->values = (float*) calloc(8, sizeof(float));
    s->error = 0;
    return s;
}

void stackFree(stack_t *s){
    free(s->values);
    free(s);
}

char stackIsEmpty(stack_t *s){
    return s->size == 0;
}

// private !
static char isFull(stack_t* s) {
    return s->size == s->next;
}

// private !
static void increase(stack_t* s) {
    s->size *= 4;
    s->values = (float*) realloc(s->values,
                                 s->size * sizeof(float));
}

// private !
static void decrease(stack_t* s) {
    s->size /= 4;
    s->values = (float*) realloc(s->values,
                                 s->size * sizeof(float));
}

/**
 * Adds the provided value to the provided stack.
 */
void stackAdd(stack_t* s, float value){
    if (isFull(s)) increase(s);
    s->values[s->next] = value;
    s->next += 1;
}

/**
 * Returns the last added value from the provided stack.
 */
float stackLast(stack_t *s){
    int i = s->next - 1;
    if (i < 0) {
        s->error = 1;
        return -1;
    }
    return s->values[i];
}

/**
 * Removes the last added value from the provided stack.
 */
int stackFreeLast(stack_t *s){
    int i = s->next - 1;
    if (i < 0) {
        s->error = 1;
        return -1;
    }
    free(&s->values[i]);
    return 0;
}

char stackGetError(stack_t *s){
    return s->error;
}