/*
Question : Use a stack to check balanced parentheses/brackets in an expression.
*/

#include "stdio.h"
#include "stdlib.h"

struct Node {
	char data;
	struct Node *next;
} *top = NULL;

void push(char ch) {

	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode -> data = ch;
	newNode -> next = top;
	
	top = newNode;
}

char pop() {

	if(top == NULL) { 
		return '\0';
	} else {
		struct Node *temp;
		char ch;
		
		temp = top;
		ch = top -> data;
		
		top = top -> next;
		
		free(temp);
		return ch;
	}
}

int isMatching(char open, char close) {
	if(open == '(' && close == ')') return 1;
	if(open == '{' && close == '}') return 1;
	if(open == '[' && close == ']') return 1;
	return 0;
}

int isBalance(char str[]) {
	char ch;
	for(int i = 0; str[i] != '\0'; i++) {
	
		if(str[i] == '(' || str[i] == '[' || str[i] == '{') {
			push(str[i]);
		}
		else if(str[i] == ')' || str[i] == ']' || str[i] == '}') {
			// Stack is empty
			if(top == NULL) return 0;
			
			ch = pop();
			
			if(!isMatching(ch ,str[i])) {
				return 0;
			}
			
		}
	}
	if(top == NULL) return 1;
	
	return 0;
}

int main() {
	char str[100];
	s1 : printf("Enter the expression : ");
	scanf("%s", str);
	
	if(isBalance(str)) {
		printf("It is balance Parenthesis\n");
	}
	else {
		printf("It is not the balance parenthesis\n");
	}
	
	printf("If you want check again press 1 or else 0.\n");
	int num;
	scanf("%d", &num); 	
	if(num == 1) goto s1;
	
	return 0;
}

/*
Output :

(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD/HCL_week_2/C_Program$ ./StackParenthesis
Enter the expression : [([{}])]
It is balance Parenthesis
If you want check again press 1 or else 0.
1
Enter the expression : [[][])({}
It is not the balance parenthesis
If you want check again press 1 or else 0.
0

*/
