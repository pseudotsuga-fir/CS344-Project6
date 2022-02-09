// Andrew Hepworth

#include "myalloc.h"

struct block *head = NULL;

int myalloc(int size) {
    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->in_use = 0;
    }

    do {
        if(!head->in_use) {
            if(size <= head->size) {
                head->in_use = 1;
                return PTR_OFFSET(head, PADDED_SIZE(sizeof(struct block)));
            }
        }
        head = head->next;
    } while (head);

    return NULL;
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
    print_data();
    p = myalloc(1009);
    print_data();
    p = myalloc(0);
    print_data();
    p = myalloc(1);
    print_data();
}