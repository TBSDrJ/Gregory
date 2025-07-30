#include"queue_2.h"
#include"polynomials.h"

struct Queue_2* Queue_2_constructor () {
    struct Queue_2* q = malloc(sizeof(struct Queue_2));
    q -> head = NULL;
    q -> tail = NULL;
    q -> len = 0;
    return q;
}

void Queue_2_destructor(struct Queue_2* q) {
    if (q -> head) {
        struct Entry_2* e = q -> head;
        while (e -> next) {
            struct Entry_2* tmp = e;
            e = e -> next;
            Entry_2_destructor(tmp);
        }
        Entry_2_destructor(e);
    }
    free(q);
}

struct Entry_2* Entry_2_constructor(struct Polynomial* a, struct Polynomial* b) {
    struct Entry_2* e = malloc(sizeof(struct Entry_2));
    e -> a = a;
    e -> b = b;
    e -> next = NULL;
    return e;
}

void Entry_2_destructor(struct Entry_2* e) {
    if (e -> a) {Polynomial_destructor(e -> a);}
    if (e -> b) {Polynomial_destructor(e -> b);}
    free(e);
}

void Queue_2_enqueue(struct Queue_2* q, struct Polynomial* a, struct Polynomial* b) {
    struct Entry_2* e = Entry_2_constructor(a, b);
    if (q -> len == 0) {
        q -> len = 1;
        q -> head = e;
        q -> tail = e;
    } else {
        q -> len++;
        q -> tail -> next = e;
        q -> tail = e;
    }
}
