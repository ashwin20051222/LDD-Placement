#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Patient {
	int patientID;
	char name[100];
	int severity;
	struct Patient *next;
} Patient;

Patient *head = NULL, *tail = NULL;

// Insert at end
void insertAtEnd(int patientID, char name[100], int severity) {
	Patient *newNode = (Patient *)malloc(sizeof(Patient));
	
	newNode -> patientID = patientID;
	strcpy(newNode -> name, name);
	newNode -> severity = severity;
	newNode -> next = NULL;
	
	if(head == NULL) {
		head = tail = newNode;
		return;
	}
	
	tail -> next = newNode;
	tail = newNode;
}

// Display element
void Display() {
	if(head == NULL) {
		printf("List is empty.\n");
		retrun ;
	}
	
	Patient *temp = head;
	
	while (temp != NULL)
    	{
        	printf("%d(%d)", temp->patientID, temp->severity);

        	if (temp->next != NULL){
            		printf(" -> ");
            	}

        	temp = temp->next;
    	}
    	
    	printf("\n");
	
}

// Selection Sort
void SelectionSort() {
        if(head == NULL) {
		printf("List is empty.\n");
		retrun ;
	}
	
        printf("Before Sorting :\n");
        Display();
        
        for(Patient *start = head; start != NULL;)
}
int main() {

	return 0;
}
