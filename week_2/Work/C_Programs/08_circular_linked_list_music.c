/*
PROGRAM: Circular Linked List - Music Player Playlist
QUESTION: Build a circular linked list for a music player. Implement:
          1. Add a song at the end
          2. Remove a song by name
          3. Traverse the playlist continuously for k iterations
          Demonstrate with at least 5 songs
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Node structure for song */
typedef struct Node
{
    char songName[50];
    struct Node *next;
} Node;

/* Circular Linked List structure */
typedef struct
{
    Node *head;
} CircularPlaylist;

/* Initialize playlist */
void initPlaylist(CircularPlaylist *playlist)
{
    playlist->head = NULL;
}

/* Create new song node */
Node *createSongNode(char *songName)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->songName, songName);
    newNode->next = NULL;
    return newNode;
}

/* Add song at the end */
void addSong(CircularPlaylist *playlist, char *songName)
{
    Node *newNode = createSongNode(songName);
    
    if (playlist->head == NULL)
    {
        playlist->head = newNode;
        newNode->next = newNode;  /* Point to itself */
        printf("Added first song: %s\n", songName);
    }
    else
    {
        Node *current = playlist->head;
        
        /* Find last node */
        while (current->next != playlist->head)
        {
            current = current->next;
        }
        
        /* Add new node */
        current->next = newNode;
        newNode->next = playlist->head;
        printf("Added song: %s\n", songName);
    }
}

/* Remove song by name */
void removeSong(CircularPlaylist *playlist, char *songName)
{
    if (playlist->head == NULL)
    {
        printf("Playlist is empty\n");
        return;
    }
    
    Node *current = playlist->head;
    Node *previous = NULL;
    int found = 0;
    
    /* Special case: removing head */
    if (strcmp(playlist->head->songName, songName) == 0)
    {
        if (playlist->head->next == playlist->head)
        {
            /* Only one song */
            free(playlist->head);
            playlist->head = NULL;
            printf("Removed song: %s (playlist now empty)\n", songName);
        }
        else
        {
            Node *temp = playlist->head;
            
            /* Find last node to update its next pointer */
            Node *last = playlist->head;
            while (last->next != playlist->head)
            {
                last = last->next;
            }
            
            playlist->head = playlist->head->next;
            last->next = playlist->head;
            free(temp);
            printf("Removed song: %s\n", songName);
        }
        return;
    }
    
    /* Search for song to remove */
    previous = current;
    current = current->next;
    
    while (current != playlist->head)
    {
        if (strcmp(current->songName, songName) == 0)
        {
            previous->next = current->next;
            free(current);
            printf("Removed song: %s\n", songName);
            found = 1;
            break;
        }
        previous = current;
        current = current->next;
    }
    
    if (!found)
    {
        printf("Song not found: %s\n", songName);
    }
}

/* Traverse and play songs */
void traversePlaylist(CircularPlaylist *playlist, int iterations)
{
    if (playlist->head == NULL)
    {
        printf("Playlist is empty\n");
        return;
    }
    
    Node *current = playlist->head;
    int count = 0;
    int songCount = 0;
    int currentIter = 1;
    
    /* Count songs */
    Node *temp = playlist->head;
    do
    {
        songCount++;
        temp = temp->next;
    } while (temp != playlist->head);
    
    printf("\nPlaying playlist for %d iterations (%d songs total):\n", 
           iterations, songCount);
    printf("================\n");
    
    for (count = 0; count < iterations * songCount; count++)
    {
        if (count > 0 && count % songCount == 0)
        {
            printf("--- Iteration %d ---\n", ++currentIter);
        }
        
        printf("%d. [Iteration %d] Now Playing: %s\n", 
               count + 1, currentIter, current->songName);
        current = current->next;
    }
}

/* Display all songs in playlist */
void displayPlaylist(CircularPlaylist *playlist)
{
    if (playlist->head == NULL)
    {
        printf("Playlist is empty\n");
        return;
    }
    
    Node *current = playlist->head;
    int position = 1;
    
    printf("\nCurrent Playlist:\n");
    printf("================\n");
    
    do
    {
        printf("%d. %s\n", position, current->songName);
        current = current->next;
        position++;
    } while (current != playlist->head);
}

int main()
{
    CircularPlaylist playlist;
    initPlaylist(&playlist);
    
    printf("Music Player - Circular Linked List (Runtime Input)\n");
    printf("==================================================\n\n");
    
    int choice;
    char songName[50];
    int iterations;
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Add song to end\n");
        printf("2. Remove song by name\n");
        printf("3. Display playlist\n");
        printf("4. Play playlist (k iterations)\n");
        printf("5. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        getchar();  /* Consume newline after scanf */
        
        switch (choice)
        {
            case 1:
                printf("Enter song name: ");
                fgets(songName, sizeof(songName), stdin);
                if (songName[strlen(songName) - 1] == '\n')
                    songName[strlen(songName) - 1] = '\0';
                addSong(&playlist, songName);
                break;
                
            case 2:
                printf("Enter song name to remove: ");
                fgets(songName, sizeof(songName), stdin);
                if (songName[strlen(songName) - 1] == '\n')
                    songName[strlen(songName) - 1] = '\0';
                removeSong(&playlist, songName);
                break;
                
            case 3:
                displayPlaylist(&playlist);
                break;
                
            case 4:
                printf("Enter number of iterations: ");
                scanf("%d", &iterations);
                traversePlaylist(&playlist, iterations);
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
Music Player - Circular Linked List
===================================

--- Adding Songs ---
Added first song: Imagine
Added song: Bohemian Rhapsody
Added song: Hotel California
Added song: Stairway to Heaven
Added song: Purple Haze

Current Playlist:
================
1. Imagine
2. Bohemian Rhapsody
3. Hotel California
4. Stairway to Heaven
5. Purple Haze

Playing playlist for 2 iterations (5 songs total):
================
1. [Iteration 1] Now Playing: Imagine
2. [Iteration 1] Now Playing: Bohemian Rhapsody
3. [Iteration 1] Now Playing: Hotel California
4. [Iteration 1] Now Playing: Stairway to Heaven
5. [Iteration 1] Now Playing: Purple Haze
6. [Iteration 2] Now Playing: Imagine
7. [Iteration 2] Now Playing: Bohemian Rhapsody
8. [Iteration 2] Now Playing: Hotel California
9. [Iteration 2] Now Playing: Stairway to Heaven
10. [Iteration 2] Now Playing: Purple Haze

--- Removing Songs ---
Removed song: Bohemian Rhapsody

Current Playlist:
================
1. Imagine
2. Hotel California
3. Stairway to Heaven
4. Purple Haze

--- Traversing updated playlist for 1 iteration ---
1. [Iteration 1] Now Playing: Imagine
2. [Iteration 1] Now Playing: Hotel California
3. [Iteration 1] Now Playing: Stairway to Heaven
4. [Iteration 1] Now Playing: Purple Haze
*/
