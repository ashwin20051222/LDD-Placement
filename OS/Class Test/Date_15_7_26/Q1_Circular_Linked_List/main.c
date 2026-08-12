#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int songId;
    char title[100];
    char artist[100];
    float duration;
    struct node *next;
    struct node *prev;
} node;

node *head = NULL;
node *tail = NULL;
node *currentSong = NULL;

node *createNode(void)
{
    node *newNode = (node *)malloc(sizeof(node));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    printf("Enter Song ID : ");
    scanf("%d", &newNode->songId);

    printf("Enter Song Title : ");
    scanf(" %99[^\n]", newNode->title);

    printf("Enter Artist Name : ");
    scanf(" %99[^\n]", newNode->artist);

    printf("Enter Duration (minutes) : ");
    scanf("%f", &newNode->duration);

    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

node *findSongById(int songId)
{
    node *temp;

    if (head == NULL) {
        return NULL;
    }

    temp = head;
    do {
        if (temp->songId == songId) {
            return temp;
        }
        temp = temp->next;
    } while (temp != head);

    return NULL;
}

void addSongAtEnd(void)
{
    node *newNode = createNode();

    if (newNode == NULL) {
        return;
    }

    if (head == NULL) {
        head = newNode;
        tail = newNode;
        newNode->next = head;
        newNode->prev = tail;
        currentSong = head;
    } else {
        newNode->next = head;
        newNode->prev = tail;
        tail->next = newNode;
        head->prev = newNode;
        tail = newNode;
    }

    printf("Song added successfully\n");
}

void insertAfterSong(void)
{
    int targetSongId;
    node *targetNode;
    node *newNode;

    if (head == NULL) {
        printf("Playlist is empty\n");
        return;
    }

    printf("Enter Song ID after which to insert : ");
    scanf("%d", &targetSongId);

    targetNode = findSongById(targetSongId);
    if (targetNode == NULL) {
        printf("Song ID not found\n");
        return;
    }

    newNode = createNode();
    if (newNode == NULL) {
        return;
    }

    newNode->next = targetNode->next;
    newNode->prev = targetNode;
    targetNode->next->prev = newNode;
    targetNode->next = newNode;

    if (targetNode == tail) {
        tail = newNode;
    }

    printf("Song inserted successfully\n");
}

void deleteSong(void)
{
    int songId;
    node *targetNode;

    if (head == NULL) {
        printf("Playlist is empty\n");
        return;
    }

    printf("Enter Song ID to delete : ");
    scanf("%d", &songId);

    targetNode = findSongById(songId);
    if (targetNode == NULL) {
        printf("Song ID not found\n");
        return;
    }

    if (head == tail) {
        free(targetNode);
        head = NULL;
        tail = NULL;
        currentSong = NULL;
        printf("Song deleted successfully\n");
        return;
    }

    targetNode->prev->next = targetNode->next;
    targetNode->next->prev = targetNode->prev;

    if (targetNode == head) {
        head = targetNode->next;
    }
    if (targetNode == tail) {
        tail = targetNode->prev;
    }
    if (currentSong == targetNode) {
        currentSong = targetNode->next;
    }

    free(targetNode);
    printf("Song deleted successfully\n");
}

void searchByTitle(void)
{
    char title[100];
    node *temp;
    int found = 0;

    if (head == NULL) {
        printf("Playlist is empty\n");
        return;
    }

    printf("Enter Song Title to search : ");
    scanf(" %99[^\n]", title);

    temp = head;
    do {
        if (strcmp(temp->title, title) == 0) {
            printf("Song found: ID=%d, Title=%s, Artist=%s, Duration=%.2f min\n",
                   temp->songId, temp->title, temp->artist, temp->duration);
            found = 1;
        }
        temp = temp->next;
    } while (temp != head);

    if (!found) {
        printf("No matching song found\n");
    }
}

void displayPlaylist(void)
{
    node *temp;

    if (head == NULL) {
        printf("Playlist is empty\n");
        return;
    }

    printf("Playlist:\n");
    temp = head;
    do {
        printf("ID=%d | Title=%s | Artist=%s | Duration=%.2f min\n",
               temp->songId, temp->title, temp->artist, temp->duration);
        temp = temp->next;
    } while (temp != head);
}

void playCurrentSong(void)
{
    if (currentSong == NULL) {
        printf("No song to play. Playlist is empty\n");
        return;
    }

    printf("Now Playing -> ID=%d | Title=%s | Artist=%s | Duration=%.2f min\n",
           currentSong->songId,
           currentSong->title,
           currentSong->artist,
           currentSong->duration);
}

void playNextSong(void)
{
    if (currentSong == NULL) {
        printf("No song to play. Playlist is empty\n");
        return;
    }

    currentSong = currentSong->next;
    playCurrentSong();
}

void playPreviousSong(void)
{
    if (currentSong == NULL) {
        printf("No song to play. Playlist is empty\n");
        return;
    }

    currentSong = currentSong->prev;
    playCurrentSong();
}

void continuousPlayback(void)
{
    int stopChoice = 0;

    if (currentSong == NULL) {
        printf("No song to play. Playlist is empty\n");
        return;
    }

    while (stopChoice != 1) {
        playCurrentSong();
        currentSong = currentSong->next;
        printf("Enter 1 to stop playback, 0 to continue : ");
        scanf("%d", &stopChoice);
    }
}

void freePlaylist(void)
{
    node *temp;
    node *nextNode;

    if (head == NULL) {
        return;
    }

    temp = head;
    do {
        nextNode = temp->next;
        free(temp);
        temp = nextNode;
    } while (temp != head);

    head = NULL;
    tail = NULL;
    currentSong = NULL;
}

int main(void)
{
    int choice;

    while (1) {
        printf("\nMusic Playlist Manager\n");
        printf("1. Add Song at End\n");
        printf("2. Insert Song after Specific Song ID\n");
        printf("3. Delete Song by Song ID\n");
        printf("4. Search Song by Title\n");
        printf("5. Display Playlist\n");
        printf("6. Play Next Song\n");
        printf("7. Play Previous Song\n");
        printf("8. Continuous Playback\n");
        printf("9. Exit\n");
        printf("Enter choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSongAtEnd();
                break;
            case 2:
                insertAfterSong();
                break;
            case 3:
                deleteSong();
                break;
            case 4:
                searchByTitle();
                break;
            case 5:
                displayPlaylist();
                break;
            case 6:
                playNextSong();
                break;
            case 7:
                playPreviousSong();
                break;
            case 8:
                continuousPlayback();
                break;
            case 9:
                freePlaylist();
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}
