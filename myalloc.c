// Andrew Hepworth

#include "myalloc.h"

struct block *head = NULL;

void *myalloc(int size) {
    
    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->prev = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->in_use = 0;
    }

    int bytes = PADDED_SIZE(size);

    struct block *n = head;
    
    do {
        if(!n->in_use) {
            if (bytes <= n->size) {
                int required_space = REQ_SPACE(size);
                if (n->size >= required_space) {
                    struct block *new = PTR_OFFSET(n, PADDED_SIZE(sizeof(struct block)));
                    new->next = n->next;
                    new->prev = n;
                    new->size = n->size - (PADDED_SIZE(size) + PADDED_SIZE(sizeof(struct block)));
                    new->in_use = 0;
                    n->size = bytes;
                    n->next = new;
                }
                n->in_use = 1;
                return PTR_OFFSET(n, PADDED_SIZE(sizeof(struct block)));
            }
        }
        n = n->next;
    } while (n);

    return NULL;
}

void mergeblock(struct block *cur) {
    if ((cur) && (cur->in_use == 0) && (cur->next) && ((cur->next)->in_use == 0)) {
        cur->size = cur->size + PADDED_SIZE(sizeof(struct block)) + (cur->next)->size;
        cur->next = (cur->next)->next;
    }
}

void myfree(void *p) {
    struct block *b = PTR_OFFSET(p, -PADDED_SIZE(sizeof(struct block)));
    // printf(" %p\n", b);
    b->in_use = 0;
    mergeblock(b);
    mergeblock(b->prev);
}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main() {
    void *p, *q, *r, *s;

    p = myalloc(10); print_data();
    q = myalloc(20); print_data();
    r = myalloc(30); print_data();
    s = myalloc(40); print_data();

    myfree(q); print_data();
    myfree(p); print_data();
    myfree(s); print_data();
    myfree(r); print_data();
}