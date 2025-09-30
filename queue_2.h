/* 
A struct for a queue/singly-linked list of pairs of pointers to Polynomials.
*/
#ifndef QUEUE_2_H
#define QUEUE_2_H

#include<stdlib.h>
#include"polynomial.h"

struct Entry_2;

struct Queue_2 {
    struct Entry_2* head;
    struct Entry_2* tail;
    long len;
};

struct Entry_2 {
    struct Polynomial* a;
    struct Polynomial* b;
    struct Entry_2* next;
};

struct Queue_2* Queue_2_constructor();
void Queue_2_destructor(struct Queue_2* q);
void Queue_2_enqueue(
        struct Queue_2* q, struct Polynomial* a, struct Polynomial* b);
struct Entry_2* Entry_2_constructor(struct Polynomial* a, struct Polynomial* b);
void Entry_2_destructor(struct Entry_2* e);

#endif