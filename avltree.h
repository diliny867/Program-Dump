#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef int64_t avl_value_t;

typedef struct avl_node_t {
    struct avl_node_t* left;
    struct avl_node_t* right;
    avl_value_t val;
    int height;
} avl_node_t;

void avl_print(avl_node_t* node);

avl_node_t* avl_node_new(avl_value_t val);
void avl_tree_destroy(avl_node_t* node);

int avl_has(avl_node_t* node, avl_value_t val);
avl_node_t* avl_insert(avl_node_t* node, avl_value_t val);
avl_node_t* avl_remove(avl_node_t* node, avl_value_t val, int* removed);

int avl_depth(avl_node_t* node);
size_t avl_size(avl_node_t* node);

#ifdef AVLTREE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

#define avl_chkheight(node) ((node == NULL) ? 0 : (node)->height)

static void _avl_print(avl_node_t* node, char* prefix, size_t pref_size, int not_last) {
    if(node == NULL) return;
    printf("%.*s%s%lld\n", pref_size, prefix, not_last ? "|-- " : "L-- ", node->val);

    strcpy(prefix + pref_size, not_last ? "|   " : "    ");

    _avl_print(node->left, prefix, pref_size + 4, node->right != NULL);
    _avl_print(node->right, prefix, pref_size + 4, 0);
}
void avl_print(avl_node_t* node){
    if(node == NULL) return;

    size_t pref_size = avl_chkheight(node) * 4;
    char* prefix = (char*)malloc(sizeof(char) * (pref_size + 1));

    prefix[pref_size] = '\0';
    _avl_print(node, prefix, 0, 0);

    free(prefix);
}

avl_node_t* avl_node_new(avl_value_t val){
    avl_node_t* node = (avl_node_t*)malloc(sizeof(avl_node_t));
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    node->val = val;
    return node;
}

static void avl_recalc_height(avl_node_t* node){
    int l = avl_chkheight(node->left);
    int r = avl_chkheight(node->right);
    node->height = max(l, r) + 1;
}

static avl_node_t* avl_srot_left(avl_node_t* a){
    avl_node_t* b = a->right;
    a->right = b->left;
    b->left = a;
    avl_recalc_height(a);
    avl_recalc_height(b);
    return b;
}
static avl_node_t* avl_srot_right(avl_node_t* a){
    avl_node_t* b = a->left;
    a->left = b->right;
    b->right = a;
    avl_recalc_height(a);
    avl_recalc_height(b);
    return b;
}
static avl_node_t* avl_lrot_left(avl_node_t* a){
    a->right = avl_srot_right(a->right);
    return avl_srot_left(a);
}
static avl_node_t* avl_lrot_right(avl_node_t* a){
    a->left = avl_srot_left(a->left);
    return avl_srot_right(a);
}

int avl_has(avl_node_t* node, avl_value_t val){
    if(node == NULL) return 0;
    if(node->val == val) 
        return 1;
    if(node->val < val) 
        return avl_has(node->right, val);
    else 
        return avl_has(node->left, val);
}

int avl_depth(avl_node_t* node){
    if(node == NULL) return 0;
    return node->height + 1;
}

size_t avl_size(avl_node_t* node){
    if(node == NULL) return 0;
    return avl_size(node->left) + avl_size(node->right) + 1;
}

static avl_node_t* avl_balance(avl_node_t* node){
    avl_recalc_height(node);

    int score = avl_chkheight(node->right) - avl_chkheight(node->left);
    if(score == 2){
        if(avl_chkheight(node->right->left) <= avl_chkheight(node->right->right))
            return avl_srot_left(node);
        else
            return avl_lrot_left(node);
    }
    if(score == -2){
        if(avl_chkheight(node->left->right) <= avl_chkheight(node->left->left))
            return avl_srot_right(node);
        else
            return avl_lrot_right(node);
    }

    return node;
}

avl_node_t* avl_insert(avl_node_t* node, avl_value_t val) {
    if(node == NULL) return avl_node_new(val);

    if(node->val < val)
        node->right = avl_insert(node->right, val);
    else
        node->left = avl_insert(node->left, val);

    return avl_balance(node);
}

static avl_node_t* alv_extract_min(avl_node_t* node, avl_node_t** min){
    if(node == NULL) return NULL;

    if(node->left == NULL){
        *min = node;
        return node->right;
    }
    
    node->left = alv_extract_min(node->left, min);

    return avl_balance(node);
}
static avl_node_t* _avl_remove(avl_node_t* node, avl_value_t val, int* removed){
    if(node == NULL) return NULL;

    if(node->val == val){
        if(removed)
            *removed = 1;
        
        avl_node_t* left = node->left;
        avl_node_t* right = node->right;
        free(node);
        if(node->height == 0 || right == NULL)
            return left;

        avl_node_t* min;
        min->right = alv_extract_min(right, &min);
        min->left = left;

        return avl_balance(min);
    }

    if(node->val < val)
        node->right = _avl_remove(node->right, val, removed);
    else
        node->left = _avl_remove(node->left, val, removed);
    
    return avl_balance(node);
}
avl_node_t* avl_remove(avl_node_t* node, avl_value_t val, int* removed){
    if(removed)
        *removed = 0;
    return _avl_remove(node, val, removed);
}

void avl_tree_destroy(avl_node_t* node){
    if(node == NULL) return;

    avl_tree_destroy(node->left);
    avl_tree_destroy(node->right);

    free(node);
}

#endif
