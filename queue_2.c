#include"queue_2.h"
#include"polynomials.h"

struct Queue_2* construct_q_2 () {
    struct Queue_2* q = malloc(sizeof(struct Queue_2));
    q -> head = NULL;
    q -> tail = NULL;
    q -> len = 0;
    return q;
}

void destruct_q_2(struct Queue_2* q) {
    if (q -> head) {
        struct Entry_2* e = q -> head;
        while (e -> next) {
            struct Entry_2* tmp = e;
            e = e -> next;
            destruct_entry_2(tmp);
        }
        destruct_entry_2(e);
    }
    free(q);
}

struct Entry_2* construct_entry_2(struct Polynomial* a, struct Polynomial* b) {
    struct Entry_2* e = malloc(sizeof(struct Entry_2));
    e -> a = a;
    e -> b = b;
    e -> next = NULL;
    return e;
}

void destruct_entry_2(struct Entry_2* e) {
    if (e -> a) {destruct_poly(e -> a);}
    if (e -> b) {destruct_poly(e -> b);}
    free(e);
}

void enqueue_2(struct Queue_2* q, struct Polynomial* a, struct Polynomial* b) {
    struct Entry_2* e = construct_entry_2(a, b);
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
