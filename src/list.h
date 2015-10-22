#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>

struct list_entry {
  void *data;
  struct list_entry *next;
  struct list_entry *prev;
};

struct list {
  struct list_entry *tail;
  struct list_entry *head;
};

#define list_tail(s) ((s)->tail != 0 ? (s)->tail->data : 0)
#define list_head(s) ((s)->head != 0 ? (s)->head->data : 0)

struct list *list_alloc()
{
  struct list *s = (struct list *)malloc(sizeof (struct list));
  s->tail = 0;
  s->head = 0;
  return s;
}

void list_push(struct list *list, void *data)
{
  struct list_entry *entry = (struct list_entry *)malloc(sizeof (struct list_entry));
  entry->prev = list->head;
  entry->next = 0;
  entry->data = data;

  if (list->head == 0) {
    list->tail = entry;
  } else {
    list->head->next = entry;
  }

  list->head = entry;
}

void *list_remove(struct list *list, struct list_entry *entry)
{
  if (entry == list->tail) {
    if (entry == list->head) {
      list->tail = 0;
      list->head = 0;
    } else {
      list->tail = entry->next;
      list->tail->prev = 0;
    }
  } else if (entry == list->head) {
    list->head = entry->prev;
    list->head->next = 0;
  } else {
    struct list_entry *next = entry->next;
    struct list_entry *prev = entry->prev;
    next->prev = prev;
    prev->next = next;
  }

  void *data = entry->data;
  free(entry);
  return data;
}

void *list_pop(struct list *list)
{
  struct list_entry *entry = list->head;
  return entry != 0 ? list_remove(list, entry) : 0;
}

#endif
