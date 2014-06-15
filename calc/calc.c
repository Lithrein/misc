#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
 
/**
 * Types definitions
 */
 
/**
 * \enum bool_t
 * \brief A boolean value is either true or false
 */
typedef enum bool_e {false, true} bool_t;
 
/* Token */
 
/**
 * \enum token_type_e
 * \brief Token's type : number or operator.
 */
enum token_type_e {
    NUM, /*!< A number */
    OP /*!< An operator */
};
 
/**
 * \enum operator_e
 * \brief Operator's type
 */
enum operator_e {
    ADD  = '+', /*!< Addition */
    SUB  = '-', /*!< Substraction */
    MUL  = '*', /*!< Multiplication */
    DIV  = '/', /*!< Division */
    LPAR = '(', /*!< Left parenthesis */
    RPAR = ')'  /*!< Right parenthesis */
};
 
/**
 * \struct : token_s
 * \brief It represents a tokens : either an operator or an number
 */
struct token_s {
    union {
        long nb; /*!< A number */
        char op; /*!< An operator */
    } value;
    enum token_type_e type;
};
 
/* List */
 
/**
 * \struct : token_list_s
 * \brief A list of token is a /token/ and a pointer to the /next/ element of the list.
 */
struct token_list_s {
    struct token_s token; /*!< A token */
    struct token_list_s * next; /*!< A pointer to the next token */
};
 
/**
 * \struct : token_queue_s
 * \brief A queue holds the head /hd/ and the tail /tl/ of the list
 */
struct token_queue_s {
    struct token_list_s * hd; /*!< A pointer to head of the queue */
    struct token_list_s * tl; /*!< A pointer to tail of the queue */
};
 
/* Tree */
 
/**
 * \struct oper_s
 * \brief An operation is an operator and two operand
 */
struct oper_s {
    struct token_tree_s * left; /*!< A pointer to the left child */
    struct token_tree_s * right; /*!< A pointer to the right child */
    char op;
};
 
/**
 * \union expr_u
 * \brief An expression is an operation or a number
 */
union expr_u {
    struct oper_s operation;
    long nb;
};
 
/**
 * \struct : token_tree_s
 * \brief A tree of tokens is a /token/ and two pointers toward its chidrens
 */
struct token_tree_s {
    enum token_type_e type;
    union expr_u expr;
};
 
/**
 * List Implementation
 */
 
/**
 * \fn cons
 * \brief Conses an element to the list
 *
 * @param token : struct token_s
 * @param list  : struct token_list_s
 * @return A pointer to the head of the list : struct token_list_s *
 */
struct token_list_s *
cons (struct token_s token, struct token_list_s * list) {
    struct token_list_s * elem = NULL;
    if ((elem = malloc(sizeof *elem)) == NULL) {
        perror("cons");
        return NULL;
    }
 
    elem->token = token;
    elem->next = list;
 
    return elem;
}
 
/**
 * \fn free_token_list
 * \brief Frees the list of tokens /list/
 *
 * @param list : struct token_list_s
 */
void
free_token_list (struct token_list_s * list) {
    while (list != NULL) {
        struct token_list_s * current = list;
        list = list->next;
        free(current), current = NULL;
    }
}
 
/**
 * FIFO implementation
 */
 
/**
 * \fn token_list_init
 * \brief Initializes a queue of tokens
 *
 * @return A pointer to the queue : struct token_queue_s *
 */
struct token_queue_s *
token_queue_init (void) {
    struct token_queue_s * queue = NULL;
    if ((queue = malloc(sizeof *queue)) == NULL) {
        perror("malloc");
        return NULL;
    }
 
    queue->hd = queue->tl = NULL;
    return queue;
}
 
/**
 * \fn token_queue_is_empty
 * \brief Checks if the /queue/ is empty
 *
 * @param queue : struct token_queue_s
 * @return true if empty, otherwize false
 */
bool_t
token_queue_is_empty (struct token_queue_s * queue) {
    return queue->tl == NULL;
}
 
/**
 * \fn token_queue_push
 * \brief Pushes an element /token/ on the back of the tokens' queue /queue/
 *
 * @param token : struct token_s
 * @param queue : struct token_queue_s *
 * @return true is success, otherwize false : bool_t
 */
bool_t
token_queue_push (struct token_s token, struct token_queue_s * queue) {
    struct token_list_s * elem = NULL;
    if ((elem = cons(token, NULL)) == NULL) {
        perror("token_queue_push");
        return false;
    }
    if (token_queue_is_empty(queue))
        queue->hd = elem;
    else
        queue->tl->next = elem;
    queue->tl = elem;
 
    return true;
}
 
/**
 * \fn token_queue_pop
 * \brief Pops an element
 *
 * @param queue : struct token_queue_s *
 * @param elem  : struct token_s *
 * @return true is success, otherwize false : bool_t
 */
bool_t
token_queue_pop (struct token_queue_s * queue, struct token_s * elem) {
    struct token_list_s * cel;
    if ((cel = queue->hd) == NULL) {
        perror("token_queue_pop");
        return false;
    }
 
    if (elem != NULL)
        *elem = cel->token;
 
    if ((queue->hd = cel->next) == NULL)
        queue->tl = NULL;
 
    free(cel), cel = NULL;
 
    return true;
}
 
/**
 * \fn token_queue_free
 * \brief Frees a queue of tokens
 *
 * @param queue : struct token_queue_s *
 */
void
free_token_queue (struct token_queue_s * queue) {
    free_token_list(queue->hd);
    free(queue);
}
 
/**
 * \fn create_token_tree
 * \brief Creates a tree node with the value /token/
 *
 * @param token : struct token_s
 * @return struct token_tree_s *
 */
struct token_tree_s *
create_token_tree (struct token_s token) {
    struct token_tree_s * node = malloc(sizeof *node);
    if (!node) {
        perror("create_token_tree");
        return NULL;
    }
 
    node->type = token.type;
    if (token.type == NUM)
        node->expr.nb = token.value.nb;
    else
        node->expr.operation.op = token.value.op;
 
    return node;
}
 
/**
 * \fn free_token_tree
 * \brief Frees a /tree/ of tokens
 *
 * @param tree : struct token_tree_s
 */
void
free_token_tree (struct token_tree_s * tree) {
    if (tree) {
        if (tree->type == NUM) {
            free(tree);
        } else {
            free_token_tree(tree->expr.operation.left);
            free_token_tree(tree->expr.operation.right);
            free(tree);
        }
    }
}
 
/**
 * \fn print_token_tree
 * \brief Prints a /tree/ in an postfix way
 *
 * @param tree : struct token_tree_s
 */
void
print_token_tree (struct token_tree_s * tree) {
    if (tree) {
        if (tree->type == NUM) {
            printf("%ld ", tree->expr.nb);
        } else {
            print_token_tree(tree->expr.operation.left);
            print_token_tree(tree->expr.operation.right);
            printf("%c ", tree->expr.operation.op);
        }
    }
}
 
/**
 * Lexical analysis
 */
 
/**
 * \fn is_operator
 * \brief Checks if /c/ is an operator
 *
 * @param c : unsigned int
 * @return true if /c/ is an operatoir, otherwize false : bool_t
 */
bool_t
is_operator(unsigned int c) {
    return c == ADD || c == SUB || c == MUL || c == DIV || c == LPAR || c == RPAR;
}
 
/**
 * \fn is_token
 * \brief Checks if /c/ is a token
 *
 * @param c : unsigned int
 * @return true if /c/ is a token, otherwize false : bool_t
 */
bool_t
is_token (unsigned int c) {
    return isdigit(c) || is_operator(c);
}
 
/**
 * \fn read_token
 * \brief Reads a token and returns it
 *
 * @param stream : char **
 * @return The read token : token
 */
struct token_s
read_token (char ** stream) {
    struct token_s t;
 
    while (isspace(**stream))
        (*stream)++;
 
    if (is_token((unsigned int)**stream)) {
        if (isdigit((unsigned int)**stream)) {
            t.value.nb = strtol(*stream, stream, 10);
            t.type = NUM;
        } else {
            t.value.op = **stream;
            t.type = OP;
            (*stream)++;
        }
    }
 
    return t;
}
 
/**
 * \fn tokenize
 * \brief This function divides the stream in a list of tokens
 *
 * @param stream : char *
 * @return A pointer to the list of tokens : struct token_queue_s *
 */
struct token_queue_s *
tokenize (char * stream) {
    struct token_queue_s * tokens;
 
    tokens = token_queue_init();
 
    while (*stream)
        token_queue_push(read_token(&stream), tokens);
 
    return tokens;
}
 
/**
 * Syntax analysis
 */
 
void syntax_error (const struct token_s f_tkn, const char * msg);
struct token_tree_s * _expr (struct token_queue_s *);
struct token_tree_s * _term (struct token_queue_s *);
struct token_tree_s * factor (struct token_queue_s *);
struct token_tree_s * term (struct token_queue_s *);
struct token_tree_s * expr (struct token_queue_s *);
 
/**
 * \fn syntax_error
 * \brief prints a syntax error
 *
 * @param f_tkn : const struct token_s
 * @param msg   : const char *
 */
void
syntax_error (const struct token_s f_tkn, const char * msg) {
    if (f_tkn.type == NUM)
        fprintf(stderr, "Syntax error : '%ld' found. %s\n", f_tkn.value.nb, msg);
    else /* operators */
        fprintf(stderr, "Syntax error : '%c' found. %s\n", f_tkn.value.op, msg);
}
 
/**
 * \fn _expr
 * \brief
 *
 * @param tkn_list : struct token_queue_s
 * @return A pointer to the ast : struct token_tree_s
 */
struct token_tree_s *
_expr (struct token_queue_s * tkn_list) {
    struct token_tree_s * node = NULL;
 
    if (token_queue_is_empty(tkn_list)) return NULL;
 
    if (tkn_list->hd->token.type == OP && (tkn_list->hd->token.value.op == ADD || tkn_list->hd->token.value.op == SUB)) {
        node = create_token_tree(tkn_list->hd->token);
 
        token_queue_pop(tkn_list, NULL); /* eat */
 
        node->expr.operation.left = term(tkn_list);
        node->expr.operation.right = _expr(tkn_list);
    }
 
    return node;
}
 
/**
 * \fn _term
 * \brief
 *
 * @param tkn_list : struct token_queue_s
 * @return A pointer to the ast : struct token_tree_s
 */
struct token_tree_s *
_term (struct token_queue_s * tkn_list) {
    struct token_tree_s * node = NULL;
 
    if (token_queue_is_empty(tkn_list)) return NULL;
 
    if (tkn_list->hd->token.type == OP && (tkn_list->hd->token.value.op == MUL || tkn_list->hd->token.value.op == DIV)) {
        node = create_token_tree(tkn_list->hd->token);
 
        token_queue_pop(tkn_list, NULL); /* eat */
 
        node->expr.operation.left = factor(tkn_list);
        node->expr.operation.right = _term(tkn_list);
    }
 
    return node;
}
 
/**
 * \fn factor
 * \brief
 *
 * @param tkn_list : struct token_queue_s
 * @return A pointer to the ast : struct token_tree_s
 */
struct token_tree_s *
factor (struct token_queue_s * tkn_list) {
    struct token_tree_s * node = NULL;
 
    if (token_queue_is_empty(tkn_list)) return NULL;
 
    if (tkn_list->hd->token.type == NUM) {
        node = create_token_tree(tkn_list->hd->token);
 
        token_queue_pop(tkn_list, NULL); /* eat */
    } else if (tkn_list->hd->token.type == OP && tkn_list->hd->token.value.op == LPAR) {
        token_queue_pop(tkn_list, NULL); /* eat */
        node =  expr(tkn_list);
    }
 
    return node;
}
 
/**
 * \fn term
 * \brief
 *
 * @param tkn_list : struct token_queue_s
 * @return A pointer to the ast : struct token_tree_s
 */
struct token_tree_s *
term (struct token_queue_s * tkn_list) {
    struct token_tree_s * node = NULL;
 
    if (token_queue_is_empty(tkn_list)) return NULL;
 
     if (tkn_list->hd->token.type == NUM || (tkn_list->hd->token.type == OP && tkn_list->hd->token.value.op == LPAR)) {
        struct token_tree_s * tmp1 = factor(tkn_list);
        struct token_tree_s * tmp2 = _term(tkn_list);
 
        if (tmp2 == NULL) {
            node = tmp1;
        } else {
            struct oper_s * current = &(tmp2->expr.operation);
            while (current->right)
                current = &(current->right->expr.operation);
            current->right = tmp1;
 
            node = tmp2;
        }
    } else {
        syntax_error(tkn_list->hd->token, "Number or left parenthesis is expected");
    }
 
    return node;
}
 
/**
 * \fn expr
 * \brief
 *
 * @param tkn_list : struct token_queue_s
 * @return A pointer to the ast : struct token_tree_s
 */
struct token_tree_s *
expr (struct token_queue_s * tkn_list) {
    struct token_tree_s * node = NULL;
 
    if (token_queue_is_empty(tkn_list)) return NULL;
 
    if (tkn_list->hd->token.type == NUM || (tkn_list->hd->token.type == OP && tkn_list->hd->token.value.op == LPAR)) {
        struct token_tree_s * tmp1 = term(tkn_list);
        struct token_tree_s * tmp2 = _expr(tkn_list);
 
        if (tmp2 == NULL) {
            node = tmp1;
        } else {
            struct oper_s * current = &(tmp2->expr.operation);
            while (current->right)
                current = &(current->right->expr.operation);
            current->right = tmp1;
 
            node = tmp2;
        }
    } else {
        syntax_error(tkn_list->hd->token, "Number or left parenthesis is expected");
    }
 
    if (tkn_list->hd && tkn_list->hd->token.type == OP && tkn_list->hd->token.value.op == RPAR)
        token_queue_pop(tkn_list, NULL); /* eat */
 
    return node;
}
 
/**
 * \fn parse
 * \brief Use a list of /tkn_list/ to make an abstract syntax tree
 *
 * @param tkn_list : struct token_queue_s *
 * @return An abstract syntax tree : struct token_tree_s *
 */
struct token_tree_s *
parse (struct token_queue_s * tkn_list) {
    return expr(tkn_list);
}
 
/**
 * AST computation
 */
 
/**
 * \fn compute_ast
 * \brief Computes the /ast/
 *
 * @param ast : struct token_tree_s
 */
float
compute_ast (struct token_tree_s * ast) {
    float res = 0;
 
    if (ast) {
        if (ast->type == NUM) {
            return ast->expr.nb;
        } else  {
            switch (ast->expr.operation.op) {
                case '+' : return compute_ast(ast->expr.operation.right) + compute_ast(ast->expr.operation.left);
                case '-' : return compute_ast(ast->expr.operation.right) - compute_ast(ast->expr.operation.left);
                case '*' : return compute_ast(ast->expr.operation.right) * compute_ast(ast->expr.operation.left);
                case '/' : return compute_ast(ast->expr.operation.right) / compute_ast(ast->expr.operation.left);
            }
        }
    }
    return res;
}
 
/**
 * \fn eval
 * \brief This function evaluates a mathematical expression and returns the result of this one
 *
 * @param expr : char *
 * @return The result of the mathematical expression : float
 */
float
eval (char * expr) {
    float result = 0;
    struct token_queue_s * tokens;
    struct token_tree_s * ast = NULL;
 
    tokens = tokenize(expr);
    ast = parse(tokens);
 
    result = compute_ast(ast);
 
    free_token_tree(ast), ast = NULL;
    free_token_queue(tokens), tokens = NULL;
 
    return result;
}
 
int
main (void) {
    char expression[] = "2*(2+3)*(2-3)/5"; /* -2 */
 
    printf("%s = %g\n", expression, eval(expression));
 
    return EXIT_SUCCESS;
}
