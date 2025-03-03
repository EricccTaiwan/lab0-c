#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_qhead = malloc(sizeof(struct list_head));
    if (!new_qhead)
        return NULL;
    INIT_LIST_HEAD(new_qhead);
    return new_qhead;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *cur, *tmp;
    list_for_each_safe (cur, tmp, head) {
        list_del(cur);
        free(list_entry(cur, element_t, list)->value);
        free(list_entry(cur, element_t, list));
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_elem = malloc(sizeof(element_t));
    if (!new_elem)
        return false;
    INIT_LIST_HEAD(&new_elem->list);
    new_elem->value = strdup(s);
    if (!new_elem->value) {
        free(new_elem);
        return false;
    }
    list_add(&new_elem->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_elem = malloc(sizeof(element_t));
    if (!new_elem)
        return false;
    INIT_LIST_HEAD(&new_elem->list);
    new_elem->value = strdup(s);
    if (!new_elem->value) {
        free(new_elem);
        return false;
    }
    list_add_tail(&new_elem->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *head_elem = list_first_entry(head, element_t, list);
    list_del(&head_elem->list);
    if (sp && bufsize) {
        snprintf(sp, bufsize, "%s", head_elem->value);
    }
    return head_elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *tail_elem = list_last_entry(head, element_t, list);
    list_del(&tail_elem->list);
    if (sp && bufsize) {
        snprintf(sp, bufsize, "%s", tail_elem->value);
    }
    return tail_elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return -1;
    int size = 0;
    struct list_head *cur;
    list_for_each (cur, head)
        size++;
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *fast = head->next;
    struct list_head *slow = head->next;

    while (fast->next != head && fast->next->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    list_del(slow);
    free(list_entry(slow, element_t, list)->value);
    free(list_entry(slow, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *cur, *tmp;
    bool dup = false;

    list_for_each_safe (cur, tmp, head) {
        if (tmp != head && !strcmp(list_entry(cur, element_t, list)->value,
                                   list_entry(tmp, element_t, list)->value)) {
            list_del(cur);
            free(list_entry(cur, element_t, list)->value);
            free(list_entry(cur, element_t, list));
            dup = true;
        } else if (dup) {
            list_del(cur);
            free(list_entry(cur, element_t, list)->value);
            free(list_entry(cur, element_t, list));
            dup = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *cur, *tmp;
    list_for_each_safe (cur, tmp, head) {
        if (tmp == head)
            break;
        cur->prev->next = tmp;
        tmp->prev = cur->prev;
        cur->next = tmp->next;
        tmp->next->prev = cur;
        tmp->next = cur;
        cur->prev = tmp;

        tmp = cur->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *cur, *tmp;
    list_for_each_safe (cur, tmp, head) {
        cur->next = cur->prev;
        cur->prev = tmp;
    }
    head->next = head->prev;
    head->prev = tmp;
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || k <= 1)
        return;
    int size = q_size(head);
    struct list_head *tail;
    LIST_HEAD(tmp);
    LIST_HEAD(new_head);

    for (int i = 0; i <= size - k; i += k) {
        int j = 0;
        list_for_each (tail, head) {
            if (j >= k)
                break;
            j++;
        }
        list_cut_position(&tmp, head, tail->prev);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, &new_head);
    }
    list_splice_init(&new_head, head);
    return;
}

static void merge_two_list(struct list_head *head,
                           struct list_head *left,
                           struct list_head *right)
{
    LIST_HEAD(tmp_head);
    while (!list_empty(left) && !list_empty(right)) {
        if (strcmp(list_entry(left->next, element_t, list)->value,
                   list_entry(right->next, element_t, list)->value) <= 0) {
            list_move_tail(left->next, &tmp_head);
        } else {
            list_move_tail(right->next, &tmp_head);
        }
    }

    list_splice_tail_init(list_empty(left) ? right : left, &tmp_head);
    list_splice_init(&tmp_head, head);
}

static void merge_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *mid = head;
    const struct list_head *fast = (const struct list_head *) head->next;

    while (fast != (const struct list_head *) head &&
           fast->next != (const struct list_head *) head) {
        mid = mid->next;
        fast = (const struct list_head *) fast->next->next;
    }

    LIST_HEAD(left);
    LIST_HEAD(right);

    list_cut_position(&left, head, mid);
    list_splice_init(head, &right);

    merge_sort(&left);
    merge_sort(&right);

    merge_two_list(head, &left, &right);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return;

    merge_sort(head);
    if (descend)
        q_reverse(head);
    return;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->prev;
    const element_t *elem = list_entry(cur, element_t, list);
    const char *min_val = elem->value;

    cur = cur->prev;
    while (cur != head) {
        struct list_head *left_more = cur->prev;
        element_t *cur_elem = list_entry(cur, element_t, list);

        if (strcmp(cur_elem->value, min_val) > 0) {
            list_del(cur);
            free(cur_elem->value);
            free(cur_elem);
        } else {
            min_val = cur_elem->value;
        }

        cur = left_more;
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->prev;
    const element_t *elem = list_entry(cur, element_t, list);
    const char *max_val = elem->value;

    cur = cur->prev;
    while (cur != head) {
        struct list_head *left_more = cur->prev;
        element_t *cur_elem = list_entry(cur, element_t, list);

        if (strcmp(cur_elem->value, max_val) < 0) {
            list_del(cur);
            free(cur_elem->value);
            free(cur_elem);
        } else {
            max_val = cur_elem->value;
        }

        cur = left_more;
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
