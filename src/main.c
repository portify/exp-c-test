#include <stdio.h>

#include "list.h"

static char char_is_space = 1;
static char char_is_digit = 2;
static char char_is_name  = 4;

static char char_flags[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 4, 0, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

struct value {
  enum {
    t_program,
    t_int
  } type;
  union {
    struct list *v_program;
    int v_int;
  };
};
struct value *value_alloc(int type)
{
  struct value *v = (struct value *)malloc(sizeof (struct value));
  v->type = type;
  return v;
}
struct value *value_program(struct list *v_program)
{
  struct value *v = value_alloc(t_program);
  v->v_program = v_program;
  return v;
}
struct value *value_int(int v_int)
{
  struct value *v = value_alloc(t_int);
  v->v_int = v_int;
  return v;
}

struct program_entry {
  unsigned char is_operator;
  union {
    struct value *operand;
    const char *operator;
  };
};
struct program_entry *program_entry_operand(struct value *operand)
{
  struct program_entry *entry = (struct program_entry *)malloc(sizeof (struct program_entry));
  entry->is_operator = 0;
  entry->operand = operand;
  return entry;
}
struct program_entry *program_entry_operator(const char *operator)
{
  struct program_entry *entry = (struct program_entry *)malloc(sizeof (struct program_entry));
  entry->is_operator = 1;
  entry->operator = operator;
  return entry;
}

struct list *compile(const char *s)
{
  struct list *program_stack = list_alloc();
  list_push(program_stack, list_alloc());
//
  char c;
  while ((c = *s++)) {
    if (c == '(') {
      list_push(program_stack, list_alloc());
    } else if (c == ')') {
      struct list *p1 = (struct list *)list_pop(program_stack);
      struct list *p2 = (struct list *)list_head(program_stack);
      if (p2 == 0) { fprintf(stderr, "error: extra )\n"); return 0; } // TODO: free
      list_push(p2, program_entry_operand(value_program(p1)));
    } else if (char_flags[(int)c] & char_is_space) {
    } else if (char_flags[(int)c] & char_is_digit) {
      printf("operand %c\n", c);
      int v = (int)(c - '0'); // TODO: read multiple digits
      struct list *p = (struct list *)list_head(program_stack);
      list_push(p, program_entry_operand(value_int(v)));
    } else if (char_flags[(int)c] & char_is_name) {
      printf("operator %c\n", c);
      char *op = (char *)malloc(2); // TODO: read multiple chars
      op[0] = c;
      op[1] = 0;
      struct list *p = (struct list *)list_head(program_stack);
      list_push(p, program_entry_operator(op));
    } else {
      fprintf(stderr, "error: unknown character %c\n", c);
      return 0; // TODO: free
    }
  }

  struct list *p = (struct list *)list_pop(program_stack);
  void *head = list_head(program_stack);
  free(program_stack);
  if (head != 0) { fprintf(stderr, "error: missing )\n"); return 0; } // TODO: free
  return p;
}

int main(int argc, char *argv[])
{
  free(compile(argv[1]));
  return 0;
}
