#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <assert.h>

#include "binarytree.h"

int h = 5;

struct TreeNode* createNode(int data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct TreeNode* generateRandomTree(int height, int *number) {
    if (height <= 0) {
        return NULL;
    }
    struct TreeNode* root = createNode(rand() % 100);
    number += 1;
    root->left = generateRandomTree(height - 1, number);
    root->right = generateRandomTree(height - 1, number);
    return root;
}

void preOrderTraversal(struct TreeNode* root, int* pre) {
    if (root != NULL) {
        *pre = root->val;
        preOrderTraversal(root->left, pre+1);
        preOrderTraversal(root->right, pre+1);
    }
}

void inOrderTraversal(struct TreeNode* root, int* in) {
    if (root != NULL) {
        inOrderTraversal(root->left, in+1);
        *in = root->val;
        inOrderTraversal(root->right, in+1);
    }
}

int main(){
    int num_of_node = 0;
    srand( time(NULL) );
    struct TreeNode *tree = generateRandomTree(h, &num_of_node);
    
    int *pre = malloc(num_of_node * sizeof(int));
    preOrderTraversal(tree, pre);

    int *in = malloc(num_of_node * sizeof(int));
    inOrderTraversal(tree, in);

    struct TreeNode *check_tree = buildTree(pre, num_of_node, in, num_of_node);
    
    int *check_pre = malloc(num_of_node * sizeof(int));
    preOrderTraversal(tree, check_pre);

    int *check_in = malloc(num_of_node * sizeof(int));
    inOrderTraversal(tree, check_in);

    for(int i = 0; i < num_of_node; i++){
        assert(*(pre + i) == *(check_pre + i));
        assert(*(in + i) == *(in + i));
    }
    printf("Correct!\n");

    free(tree);
    free(check_tree);
    free(pre);
    free(in);
    free(check_pre);
    free(check_in);
}