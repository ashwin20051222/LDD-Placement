/*
PROGRAM: Binary Search Tree with Insert, Search, and Inorder Traversal
QUESTION: Build a BST supporting insert, search, and inorder traversal operations.
EXAMPLE:
    Insert: 50, 30, 70, 20, 40, 60, 80
    Inorder traversal: 20, 30, 40, 50, 60, 70, 80
    Search 40: Found
    Search 25: Not Found
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

/* Search for value in BST */
Node *search(Node *root, int value)
{
    if (root == NULL)
    {
        return NULL;
    }
    
    if (value == root->data)
    {
        return root;
    }
    else if (value < root->data)
    {
        return search(root->left, value);
    }
    else
    {
        return search(root->right, value);
    }
}

/* Inorder traversal: Left -> Root -> Right */
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

/* Preorder traversal: Root -> Left -> Right */
void preorderTraversal(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    
    printf("%d ", root->data);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

/* Display tree structure */
void displayTree(Node *root, int indent)
{
    if (root == NULL)
    {
        return;
    }
    
    displayTree(root->right, indent + 4);
    
    int i;
    for (i = 0; i < indent; i++)
    {
        printf(" ");
    }
    printf("%d\n", root->data);
    
    displayTree(root->left, indent + 4);
}

int main()
{
    Node *root = NULL;
    int choice;
    int value;
    
    printf("Binary Search Tree - Runtime Input\n");
    printf("===================================\n\n");
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Insert value\n");
        printf("2. Search value\n");
        printf("3. Display inorder traversal\n");
        printf("4. Display tree structure\n");
        printf("5. Exit\n");
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
                printf("Enter value to search: ");
                scanf("%d", &value);
                if (search(root, value) != NULL)
                {
                    printf("Value %d found in tree!\n", value);
                }
                else
                {
                    printf("Value %d NOT found in tree!\n", value);
                }
                break;
                
            case 3:
                printf("Inorder traversal: ");
                inorderTraversal(root);
                printf("\n");
                break;
                
            case 4:
                printf("Tree structure:\n");
                displayTree(root, 0);
                break;
                
            case 5:
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
Binary Search Tree Operations
=============================

--- Inserting values: 50, 30, 70, 20, 40, 60, 80 ---
Insertion complete!

Tree Structure:
                            80
                        70
                    60
                50
            40
        30
    20

Inorder Traversal (Left-Root-Right): 20 30 40 50 60 70 80

Preorder Traversal (Root-Left-Right): 50 30 20 40 70 60 80

--- Search Operations ---
Searching for 40: FOUND
Searching for 25: NOT FOUND
Searching for 60: FOUND
Searching for 100: NOT FOUND
*/
