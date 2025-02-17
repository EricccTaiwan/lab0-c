#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_qhead = malloc(sizeof(struct list_head));
    if (new_qhead == NULL)
        return NULL;
    INIT_LIST_HEAD(new_qhead);
    return new_qhead;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head == NULL)
        return;

    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        list_del(&entry->list);
        free(entry->value);
        free(entry);
    }
    free(head);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    // if (!head)
    //     return false;
    // element_t *new_ele = malloc(sizeof(element_t));
    // if (!new_ele)
    //     return false;
    // INIT_LIST_HEAD(&new_ele->list);
    // new_ele->value = strdup(s);
    // if (!new_ele->value) {
    //     free(new_ele);
    //     return false;
    // }
    // list_add(&new_ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    // if (head == NULL)
    //     return false;
    // element_t *new_element = malloc(sizeof(element_t));
    // if (new_element == NULL)
    //     return false;

    // INIT_LIST_HEAD(&new_element->list);
    // new_element->value = strdup(s);
    // if (new_element->value == NULL) {
    //     free(new_element);
    //     return false;
    // }
    // list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    return -1;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return false;
    struct list_head *fast = head;
    struct list_head *slow = head;
    while (fast->next != head && fast->next->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    element_t *del_node = list_entry(slow, element_t, list);
    list_del(slow);
    free(del_node->value);
    free(del_node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    // if (head == NULL || list_empty(head))
    //     return false;
    // bool flag = false;
    // element_t *cur, *next;
    // list_for_each_entry_safe (cur, next, head, list) {
    //     while (&next->list != head && !strcmp(cur->value, next->value)) {
    //         list_del(&next->list);
    //         free(next->value);
    //         free(next);
    //         next = list_entry(next->list.next, element_t, list);
    //         flag = true;
    //     }

    //     if (flag) {
    //         list_del(&cur->list);
    //         free(cur->value);
    //         free(cur);
    //         flag = false;
    //     }
    // }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *left, *right;
    list_for_each_safe (left, right, head) {
        if (right == head)
            break;
        left->prev->next = right;
        right->prev = left->prev;
        left->next = right->next;
        right->next->prev = left;
        right->next = left;
        left->prev = right;

        left = right;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return;

    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head) {
        cur->next = cur->prev;
        cur->prev = safe;
    }
    cur->next = cur->prev;
    cur->prev = safe;
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL || list_empty(head) || k <= 1)
        return;

    struct list_head *current = head->next;
    struct list_head *tail;
    LIST_HEAD(dummy);
    LIST_HEAD(new_head);

    while (current != head) {
        tail = current;
        int count = 0;
        for (int i = 0; i < k && tail != head; i++) {
            tail = tail->next;
            count++;
        }
        if (count == k) {
            list_cut_position(&dummy, head, tail->prev);
            q_reverse(&dummy);
            list_splice_tail_init(&dummy, &new_head);
        } else {
            list_splice_tail_init(current, &new_head);
            break;
        }
        current = tail;
    }
    list_splice_tail(&new_head, head);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
