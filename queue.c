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
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list)
        q_release_element(entry);
    free(head);
    return;
}

/* Helper function to create a new element with a given string */
element_t *q_new_elem(const char *s)
{
    element_t *elem = malloc(sizeof(element_t));
    if (!elem)
        return NULL;

    elem->value = strdup(s);
    if (!elem->value) {
        free(elem);
        return NULL;
    }

    INIT_LIST_HEAD(&elem->list);
    return elem;
}

/* Insert an element at head of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *elem = q_new_elem(s);
    if (!elem)
        return false;
    list_add(&elem->list, head);
    return true;
}

/* Insert an element at tail of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *elem = q_new_elem(s);
    if (!elem)
        return false;
    list_add_tail(&elem->list, head);
    return true;
}

/* Remove an element from head of queue */
// cppcheck-suppress constParameterPointer
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *elem = list_first_entry(head, element_t, list);
    list_del(&elem->list);
    if (sp) {
        size_t size = strlen(elem->value);
        if (size > bufsize - 1)
            size = bufsize - 1;
        strncpy(sp, elem->value, size);
        sp[size] = '\0';
    }
    return elem;
}

/* Remove an element from tail of queue */
// cppcheck-suppress constParameterPointer
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *elem = list_last_entry(head, element_t, list);
    list_del(&elem->list);
    if (sp) {
        size_t size = strlen(elem->value);
        if (size > bufsize - 1)
            size = bufsize - 1;
        strncpy(sp, elem->value, size);
        sp[size] = '\0';
    }
    return elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *forward = head->next;
    struct list_head *backward = head->prev;
    while (forward != backward && forward->next != backward) {
        forward = forward->next;
        backward = backward->prev;
    }
    element_t *elem = list_entry(backward, element_t, list);
    list_del(backward);
    q_release_element(elem);
    return true;
}

/* Delete all nodes that have duplicate string */
// cppcheck-suppress constParameterPointer
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;
    element_t *entry, *safe;
    bool flag = false;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (&safe->list != head && !strcmp(entry->value, safe->value)) {
            list_del(&entry->list);
            q_release_element(entry);
            flag = true;
        } else if (flag) {
            list_del(&entry->list);
            q_release_element(entry);
            flag = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *node;
    list_for_each (node, head) {
        struct list_head *prev = node->prev;
        struct list_head *next = node->next->next;
        struct list_head *pair = node->next;
        if (pair != head) {
            node->next = next;
            node->prev = pair;
            pair->next = node;
            pair->prev = prev;
            prev->next = pair;
            next->prev = node;
        }
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *node, *safe, *prev = head;
    list_for_each_safe (node, safe, head) {
        node->next = prev;
        node->prev = safe;
        prev = node;
    }
    node->next = prev;
    node->prev = safe;
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
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
