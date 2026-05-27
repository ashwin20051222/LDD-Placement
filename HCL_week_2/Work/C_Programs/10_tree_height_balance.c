/*
PROGRAM: Binary Tree Height and Balance Check
QUESTION: Compute the height of a binary tree and determine if it is 
          height-balanced (difference between left and right subtree 
          heights <= 1 for every node).
*/

#include <stdio.h>
#include <stdlib.h>

/* Tree node structure */
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

/* Calculate height of tree */
int getHeight(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

/* Check if tree is balanced */
int isBalanced(Node *root)
{
    if (root == NULL)
    {
        return 1;
    }
    
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    
    /* Check if current node is balanced */
    if (leftHeight - rightHeight > 1 || rightHeight - leftHeight > 1)
    {
        return 0;
    }
    
    /* Check if left and right subtrees are balanced */
    return isBalanced(root->left) && isBalanced(root->right);
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
    printf("Binary Tree - Height and Balance Check (Runtime Input)\n");
    printf("====================================================\n\n");
    
    int choice;
    int value;
    Node *root = NULL;
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Insert value to tree\n");
        printf("2. Display tree structure\n");
        printf("3. Check height\n");
        printf("4. Check if balanced\n");
        printf("5. Display full analysis\n");
        printf("6. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                if (root == NULL)
                {
                    root = createNode(value);
                }
                else
                {
                    if (value < root->data)
                    {
                        if (root->left == NULL)
                            root->left = createNode(value);
                        else
                            root->left->left = createNode(value);
                    }
                    else
                    {
                        if (root->right == NULL)
                            root->right = createNode(value);
                        else
                            root->right->right = createNode(value);
                    }
                }
                printf("Inserted: %d\n", value);
                break;
                
            case 2:
                if (root == NULL)
                    printf("Tree is empty!\n");
                else
                {
                    printf("\nTree structure:\n");
                    displayTree(root, 0);
                }
                break;
                
            case 3:
                if (root == NULL)
                    printf("Tree is empty!\n");
                else
                {
                    int height = getHeight(root);
                    printf("Height: %d\n", height);
                }
                break;
                
            case 4:
                if (root == NULL)
                    printf("Tree is empty!\n");
                else
                {
                    int balanced = isBalanced(root);
                    printf("Is Balanced: %s\n", balanced ? "YES" : "NO");
                }
                break;
                
            case 5:
                if (root == NULL)
                    printf("Tree is empty!\n");
                else
                {
                    printf("\nFull Analysis:\n");
                    displayTree(root, 0);
                    printf("Height: %d\n", getHeight(root));
                    printf("Is Balanced: %s\n", isBalanced(root) ? "YES" : "NO");
                }
                break;
                
            case 6:
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
Binary Tree - Height and Balance Check
======================================

--- Balanced Tree ---
Tree structure:
                17
            15
        10
            7
        5
            3

Height: 3
Is Balanced: YES

--- Unbalanced Tree ---
Tree structure:
            15
        10
            1
        3
        5

Height: 4
Is Balanced: NO
*/
