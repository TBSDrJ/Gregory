#include"frac.h"

// Functions for Queue_long
struct Queue_long* construct_queue_long() {
    struct Queue_long* q = malloc(sizeof(struct Queue_long));
    q -> head = NULL;
    q -> tail = NULL;
    q -> len = 0;
    return q;
}

void destruct_queue_long(struct Queue_long* q) {
    struct Entry_long* e = q -> head;
    while (e) {e = destruct_entry_long(e);}
    free(q);
}

void enqueue_long(struct Queue_long* q, unsigned long n) {
    struct Entry_long* e = construct_entry_long(n);
    if (q -> tail) {
        q -> tail -> next = e;
        q -> tail = e;
        q -> len++;
    } else {
        q -> head = e;
        q -> tail = e;
        q -> len = 1;
    }
}

void dump_queue(struct Queue_long* q) {
    struct Entry_long* e = q -> head;
    while(e) {
        printf("%li", e -> content);
        e = e -> next;
    }
    printf("\n");
}

// Functions for Entry_long
struct Entry_long* construct_entry_long(unsigned long n) {
    struct Entry_long* e = malloc(sizeof(struct Entry_long));
    e -> content = n;
    e -> next = NULL;
    return e;
}

struct Entry_long* destruct_entry_long(struct Entry_long* e) {
    struct Entry_long* next = e -> next;
    if (e) {free(e);}
    return next;
}

// Functions for Fraction
struct Fraction* construct_frac(long numerator, long denominator) {
    return NULL;
}

int main() {
    struct Queue_long* q = construct_queue_long();
    enqueue_long(q, 3141592653);
    enqueue_long(q, 5897932384);
    enqueue_long(q, 6264338327);
    dump_queue(q);
    destruct_queue_long(q);
}