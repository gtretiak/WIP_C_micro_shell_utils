// Assignment name:		vbc
// Expected files:		*.c *h
// Allowed functions:	malloc, calloc, realloc, free, printf, isdigit, write

// Write a program that prints the result of a mathematical expression given as argument.
// It must handle addition, multiplication and parenthesis. All values are between 0 and 9 included.
// In case of an unexpected symbol, you must print "Unexpected token '%c'\n".
// If the expression ends unexpectedly, you must print "Unexpected end of input\n".
// The same rule applies if finding an unexpected '(' or ')'.
// In case of a syscall failure, you must exit with 1.

// Examples:
// $> ./vbc '1' | cat -e
// 1$
// $> ./vbc '2+3' | cat -e
// 5$
// $> ./vbc '3*4+5' | cat -e
// 17$
// $> ./vbc '3+4*5' | cat -e
// 23$
// $> ./vbc '(3+4)*5' | cat -e
// 35$
// $> ./vbc '(((((2+2)*2+2)*2+2)*2+2)*2+2)*2' | cat -e
// 188$
// $> ./vbc '1+'
// Unexpected end of input
// $> ./vbc '1+2)' | cat -e
// Unexpected token ')'$

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum { 
        ADD, 
        MULTI,
        VAL 
    }   type;
    int           val;
    struct node   *l;
    struct node   *r;
} node;

node *new_node(node n)
{
    node *ret = calloc(1, sizeof(*ret));
    if (!ret)
        return NULL;
    *ret = n;
    return ret;
}

void destroy_tree(node *n)
{
    if (!n)
        return;
    if (n->type != VAL) {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c) {
        (*s)++;
        return 1;
    }
    return 0;
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return 1;
    unexpected(**s);
    return 0;
}

static node *parse_expr_r(char **s);
static node *parse_term(char **s);
static node *parse_factor(char **s);

static node *parse_factor(char **s)
{
    if (isdigit((unsigned char)**s)) {
        node n = { .type = VAL, .val = **s - '0', .l = NULL, .r = NULL };
        (*s)++;
        return new_node(n);
    }
    if (accept(s, '(')) {
        node *e = parse_expr_r(s);
        if (!e)
            return NULL;
        if (!expect(s, ')')) {
            destroy_tree(e);
            return NULL;
        }
        return e;
    }
    unexpected(**s);
    return NULL;
}

static node *parse_term(char **s)
{
    node *left = parse_factor(s);
    if (!left)
        return NULL;
    while (accept(s, '*')) {
        node *right = parse_factor(s);
        if (!right) {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = MULTI, .l = left, .r = right };
        left = new_node(n);
        if (!left)
            return NULL;
    }
    return left;
}

static node *parse_expr_r(char **s)
{
    node *left = parse_term(s);
    if (!left)
        return NULL;
    while (accept(s, '+')) {
        node *right = parse_term(s);
        if (!right) {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = ADD, .l = left, .r = right };
        left = new_node(n);
        if (!left)
            return NULL;
    }
    return left;
}

node *parse_expr(char *s)
{
    char *p = s;
    node *ret = parse_expr_r(&p);
    if (!ret)
        return NULL;
    if (*p) {                          
        unexpected(*p);
        destroy_tree(ret);
        return NULL;
    }
    return ret;
}

int eval_tree(node *tree)
{
    switch (tree->type) {
        case ADD:   return eval_tree(tree->l) + eval_tree(tree->r);
        case MULTI: return eval_tree(tree->l) * eval_tree(tree->r);
        case VAL:   return tree->val;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return 1;
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return 0;
}
