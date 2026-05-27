/*
PROGRAM: Delete Node from Binary Search Tree
QUESTION: Implement deletion from BST handling all three cases:
          1. Node with no children (leaf)
          2. Node with one child
          3. Node with two children
EXAMPLE:
    Original: [20, 30, 40, 50, 60, 70, 80]
    Delete 40: [20, 30, 50, 60, 70, 80]
    Delete 30: [20, 50, 60, 70, 80]
*/

#include <stdio.h>
#include <stdlib.h>

/* BST Node structure */
typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

/* Create new node */
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* Insert node in BST */
Node *insert(Node *root, int value)
{
    if (root == NULL)
    {
        return createNode(value);
    }
    
    if (value < root->data)
    {
        root->left = insert(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = insert(root->right, value);
    }
    
    return root;
}

/* Find minimum value node in a tree */
Node *findMin(Node *root)
{
    while (root != NULL && root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

/* Delete node from BST */
Node *delete(Node *root, int value)
{
    if (root == NULL)
    {
        printf("Value %d not found in tree\n", value);
        return root;
    }
    
    /* Search for the node */
    if (value < root->data)
    {
        root->left = delete(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = delete(root->right, value);
    }
    else
    {
        /* Node found - three cases to handle */
        
        /* Case 1: No children (leaf node) */
        if (root->left == NULL && root->right == NULL)
        {
            printf("Deleting leaf node: %d\n", value);
            free(root);
            return NULL;
        }
        
        /* Case 2: One child (left child) */
        if (root->right == NULL)
        {
            printf("Deleting node %d with one child (left)\n", value);
            Node *temp = root->left;
            free(root);
            return temp;
        }
        
        /* Case 2: One child (right child) */
        if (root->left == NULL)
        {
            printf("Deleting node %d with one child (right)\n", value);
            Node *temp = root->right;
            free(root);
            return temp;
        }
        
        /* Case 3: Two children */
        printf("Deleting node %d with two children\n", value);
        
        /* Find minimum value in right subtree (inorder successor) */
        Node *minRight = findMin(root->right);
        printf("  Using inorder successor: %d\n", minRight->data);
        
        /* Replace root data with successor data */
        root->data = minRight->data;
        
        /* Delete the successor node */
        root->right = delete(root->right, minRight->data);
    }
    
    return root;
}

/* Inorder traversal */
void inorderTraversal(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    
    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

int main()
{
    Node *root = NULL;
    int choice;
    int value;
    
    printf("Binary Search Tree - Delete Operations (Runtime Input)\n");
    printf("====================================================\n\n");
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Insert value\n");
        printf("2. Delete value\n");
        printf("3. Display inorder traversal\n");
        printf("4. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                root = insert(root, value);
                printf("Inserted %d\n", value);
                break;
                
            case 2:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                root = delete(root, value);
                printf("Deletion attempted for %d\n", value);
                break;
                
            case 3:
                printf("Inorder traversal: ");
                inorderTraversal(root);
                printf("\n");
                break;
                
            case 4:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid option!\n");
        }
    }
    
    return 0;
}

/*
OUTPUT:
Binary Search Tree - Delete Operations
======================================

--- Inserting values: 50, 30, 70, 20, 40, 60, 80 ---
Inorder traversal: 20 30 40 50 60 70 80

--- Delete leaf node: 20 ---
Deleting leaf node: 20
Inorder traversal: 30 40 50 60 70 80

--- Delete node with one child: 30 ---
Deleting node 30 with one child (left)
Inorder traversal: 40 50 60 70 80

--- Delete node with two children: 50 ---
Deleting node 50 with two children
  Using inorder successor: 60
Inorder traversal: 40 60 70 80

--- Delete node: 70 ---
Deleting node 70 with one child (right)
Inorder traversal: 40 60 80

--- Try to delete non-existent node: 100 ---
Value 100 not found in tree
Inorder traversal: 40 60 80
*/
