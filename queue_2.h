/* 
A struct for a queue/singly-linked list of pairs of pointers to Polynomials.
*/
#ifndef QUEUE_2_H
#define QUEUE_2_H

#include<stdlib.h>
#include"polynomials.h"

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

struct Queue_2* construct_q_2 ();
void destruct_q_2(struct Queue_2* q);
struct Entry_2* construct_entry_2(struct Polynomial* a, struct Polynomial* b);
void destruct_entry_2(struct Entry_2* e);
void enqueue_2(struct Queue_2* q, struct Polynomial* a, struct Polynomial* b);

#endif