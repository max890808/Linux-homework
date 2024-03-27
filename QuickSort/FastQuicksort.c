#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

typedef struct __node {
    struct list_head list;
    long value;
} node_t;

struct list_head *list_tail(struct list_head *head)
{
    if(!head || list_empty(head))
        return NULL;
    return head->prev;
}

int list_length(struct list_head *head)
{
    if(!head || list_empty(head))
        return 0;
    int n = 0;
    struct list_head *node;
    list_for_each(node, head){
        n++;
    }
    return n;
}

void list_construct(struct list_head *head, int n)
{
    node_t *node = malloc(sizeof(node_t));
    list_add(&node->list, head);
    node->value = n;
}

void list_free(struct list_head *head)
{
    if(!head)
        return ;
    struct list_head *next = head->next;
    node_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        list_del(&(entry->list));
        free(entry);
    }
    free(head);
}

struct list_head *list_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head) {
        INIT_LIST_HEAD(head);
        return head;
    }
    return NULL;
}

void rand_pivot(struct list_head *list)
{
    int r = rand() % list_length(list);
    struct list_head *cur = list->next;
    for (int i = 0; i < r && cur->next != list; i++)
        cur = cur->next;
    list_move(cur, list);
    return;
}

void quick_sort(struct list_head **list)
{
    int n = list_length(*list);
    int value;
    int i = 0;
    int max_level = n+1;
    struct list_head *begin[max_level];
    struct list_head *result = list_new(), *left = list_new(), *right = list_new();

    begin[0] = *list;
    for(int i = 1; i < max_level; i++) {
        begin[i] = list_new();
    }

    while (i >= 0) {
        struct list_head *L = begin[i]->next, *R = begin[i]->prev;
        if (L != R) {
            rand_pivot(begin[i]);
            struct list_head *pivot = begin[i]->next;
            value = list_entry(pivot, node_t, list)->value;
            list_del(pivot);
            node_t *entry, *safe;
            list_for_each_entry_safe (entry, safe, begin[i],list) {
                list_move(&(entry->list),entry->value > value ? right : left);
            }
            list_splice_init(left, begin[i]);
            list_add(pivot,begin[i + 1]);
            list_splice_init(right, begin[i+2]);
            i += 2;
        } else {
            if (list_is_singular(begin[i]))
                list_splice_init(begin[i],result);
            i--;
        }
    }
    *list = result;
    for(int j=1 ; j<max_level ; j++){
        free(begin[j]);
    }
    free(right);
    free(left);
}

/* Verify if list is order */
static bool list_is_ordered(struct list_head *list)
{       
    bool first = true;
    int value;
    struct list_head *node;
    list_for_each(node,list) {
        int this_value = list_entry(node, node_t, list)->value;
        if (first) {
            value = this_value;
            first = false;
        } else {
            if (this_value < value)
                return false;
            value = this_value;;
        }
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

int main(int argc, char **argv)
{
    srand( time(NULL) );
    clock_t START, END;
    struct list_head *head = list_new();

    size_t count = 1000;

    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    // shuffle(test_arr, count);

    while (count--)
        list_construct(head, test_arr[count]);
    START = clock();
    quick_sort(&head);
    END = clock();
    assert(list_is_ordered(head));

    list_free(head);

    free(test_arr);
    double diff = END - START;
    printf(" %f  sec\n", diff / CLOCKS_PER_SEC );

    return 0;
}
