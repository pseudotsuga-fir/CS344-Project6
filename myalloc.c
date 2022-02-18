// Andrew Hepworth

#include "myalloc.h"

struct block *head = NULL;

void *myalloc(int size) {
    
    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
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
                    new->next = NULL;
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

void myfree(void *p) {
    struct block *b = p-16;
    b->in_use = 0;
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
    void *p;

    myalloc(10);     print_data();
    p = myalloc(20); print_data();
    myalloc(30);     print_data();
    myfree(p);       print_data();
    myalloc(40);     print_data();
    myalloc(10);     print_data();
}