/*
 * Program: EECS 348 Assignment 2
 *
 * All lines may not be commented on exactly but they will have a comprehensive section above them that will cover either
 * the single line or the entire block like the struct below it has a multi-line comment above it to describe it, on line
 * comments will also be more simple and formated different than header type comments
 *
 * Description:
 * This program manages a CEO's email inbox using a MaxHeap
 * Emails are prioritized first by sender category and then by date
 * The highest-priority email is always stored at the root of the heap
 *
 * Sender priority:
 * 1. Boss
 * 2. Subordinate
 * 3. Peer
 * 4. ImportantPerson
 * 5. OtherPerson
 *
 * Input:
 * A text file containing EMAIL, NEXT, READ, and COUNT commands
 *
 * Output:
 * Displays the highest-priority email when NEXT is encountered
 * and displays the number of unread emails when COUNT is encountered
 *
 * Collaborators:
 * None
 *
 * Other Sources:
 * ChatGPT
 *
 * Author:
 * Logan Sigg
 *
 * Creation Date:
 * September 15, 2026
 *
 * Revision Date:
 * September 17, 2026
 *
 * Revisions:
 * Initial implementation of the CEO email priority queue
 *
 * Code Source:
 * The implementation below was generated with assistance from ChatGPT
 * and should be reviewed/modified by the author before submission
 */

/* Calling in all the required libraries for the project */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

/* Maximum length allowed for a subject line */
#define MAX_SUBJECT 256

/* Maximum length allowed for a sender category */
#define MAX_SENDER 50

/* Maximum number of emails that can be stored */
#define MAX_EMAILS 1000

/*
 * Email structure stores the sender category, subject, date, and a numeric version
 * of the date used when comparing emails
 */
typedef struct
{
    char sender[MAX_SENDER];
    char subject[MAX_SUBJECT];
    char date[11];
    int dateValue;
} Email;

/*
 * MaxHeap structure, the array acts as the list-based implementation of the heap
 * size and stores the current number of unread emails
 */
typedef struct
{
    Email list[MAX_EMAILS];
    int size;
} MaxHeap;

/*
 * senderPriority
 * Converts a sender category into a numeric priority
 * Larger numbers represent higher priority
 */
int senderPriority(const char *sender)
{
    if (strcmp(sender, "Boss") == 0) /*Checks for the Boss and sets its priority to the highest */
    {
        return 5; /*Largest size on the heap*/
    }

    if (strcmp(sender, "Subordinate") == 0) /*Does the same for the subordiante but sets to second*/
    {
        return 4;
    }

    if (strcmp(sender, "Peer") == 0)/*Then third for the peer*/
    {
        return 3;
    }

    if (strcmp(sender, "ImportantPerson") == 0) /*Then second lowest for Important Person*/
    {
        return 2;
    }

    return 1; /*And finally lowest priority for the someone not one of the four above*/
}

/*
 * convertDate
 * Converts a date in MM-DD-YYYY format into YYYYMMDD
 * This makes dates easy to compare numerically
 */
int convertDate(const char *date)
{
    int month; /* create three variables to be assigned values*/
    int day;
    int year;

    sscanf(date, "%d-%d-%d", &month, &day, &year); /*assign the values to the variables*/

    return year * 10000 + month * 100 + day; /*turns it into YYYYMMDD*/
}

/*
 * higherPriority
 * Returns 1 if email a should be read before email b
 * Sender category is checked first
 * If both emails have the same sender category, the newest date wins
 */
int higherPriority(Email a, Email b)
{
    int priorityA = senderPriority(a.sender); /*Takes in a sender postion*/
    int priorityB = senderPriority(b.sender); /*Takes in another sender postion*/

    if (priorityA > priorityB) /*Compares the returned values of the sender postion (boss == 5, otherperson == 1)*/
    {
        return 1; /*confirms a is higher priority than b*/
    }

    if (priorityA < priorityB) /*Compares the returned values again*/
    {
        return 0; /*returns 0 if b is greater than a*/
    }

    return a.dateValue > b.dateValue; /*If both same date trumps all*/
}

/*
 * swapEmails
 * Swaps two Email structures
 */
void swapEmails(Email *a, Email *b)
{
    Email temp = *a; /*create temp value of a pointer to a to not delete value a*/
    *a = *b; /*set the old a pointer to b*/
    *b = temp; /*set the temp a value to b*/
}

/*
 * initializeHeap
 * Initializes an empty MaxHeap
 */
void initializeHeap(MaxHeap *heap)
{
    heap->size = 0;
}

/*
 * insertEmail
 * Adds an email to the end of the list and then moves it upward
 * until the MaxHeap property is restored
 * This operation takes O(log n) time.
 */
void insertEmail(MaxHeap *heap, Email email)
{
    int index; /*create necessary variables*/
    int parent;

    if (heap->size >= MAX_EMAILS) /*check if the heap is already full of emails*/
    {
        return; /*send it back tills its not full*/
    }

    index = heap->size; /*If its not full set the index size equal to the heaps size*/

    heap->list[index] = email; /*insert the email to the bottom or lowest priority of the heap*/

    heap->size++; /*increase by one to account for the addition of a email*/

    while (index > 0) /*loop till index is less then 0*/
    {
        parent = (index - 1) / 2; /*reach the parent node of the heap ex. parent node of 15 is 7*/

        if (higherPriority(heap->list[index], heap->list[parent])) /*Check for the priority of the freshly inserted email*/
        { /*and compare it the parent node priority to make sure it either needs to move to the parents node or stay put*/
            swapEmails(&heap->list[index], &heap->list[parent]); /*If it needs to move then swap the nodes*/

            index = parent; /*Move up the new emails node*/
        }
        else
        {
            break; /*If not end the loop and keep it where its at*/
        }
    }
}

/*
 * heapifyDown
 * Moves an email downward through the heap after the root
 * email has been removed
 * This restores the MaxHeap property
 */
void heapifyDown(MaxHeap *heap, int index)
{
    int left; /*Create required variables for the function*/
    int right;
    int largest;

    while (1) /*Create a loop until what we need is fulfilled*/
    {
        left = 2 * index + 1; /*Equations to move the nodes down the heap priority if needed*/
        right = 2 * index + 2;/*left, right nodes, and the very bottom*/
        largest = index;

        if (left < heap->size &&
            higherPriority(heap->list[left], heap->list[largest])) /*Check the size of the left node and the priority*/
        {
            largest = left;/*if less then the size of the heap and its priority is less left is now set to largest*/
        }

        if (right < heap->size &&
            higherPriority(heap->list[right], heap->list[largest])) /*Mirror of the comments above except its for the right*/
        {
            largest = right;
        }

        if (largest == index) /*Check if the largest is already equal to the index*/
        {
            break; /*If true end the while loop*/
        }

        swapEmails(&heap->list[index], &heap->list[largest]); /*Then swap the the email positions*/

        index = largest; /*Index is now the largest*/
    }
}

/*
 * readEmail
 * Removes the highest-priority email from the heap
 * The last email is moved to the root and then heapifyDown
 * restores the MaxHeap
 * READ does not display the removed email
 */
void readEmail(MaxHeap *heap)
{
    if (heap->size == 0) /*Make sure the base case of the emails isnt true (no emails at all)*/
    {
        return; /*Return heap if true*/
    }

    heap->size--; /*"Reading" the highest priority email*/

    if (heap->size > 0) /*Check if there is a heap with emails*/
    {
        heap->list[0] = heap->list[heap->size]; /*Set the top of the list equal to the empty root position*/

        heapifyDown(heap, 0); /*Then move the email down*/
    }
}

/*
 * nextEmail
 * Displays the highest-priority email without removing it
 * Because it does not change the heap, two NEXT commands in a row
 * display the same email
 */
void nextEmail(MaxHeap *heap)
{
    if (heap->size == 0) /*Check the size of the heap to make sure its not zero*/
    {
        return; /*retunr if so*/
    }

    printf("Next email:\n"); /*Get the required information for a new email*/
    printf("Sender: %s\n", heap->list[0].sender); 
    printf("Subject: %s\n", heap->list[0].subject);
    printf("Date: %s\n", heap->list[0].date);
}

/*
 * displayCount
 * Displays the current number of unread emails
 */
void displayCount(MaxHeap *heap)
{
    printf("There are %d emails to read.\n", heap->size);
}

/*
 * removeNewline
 * Removes a newline character from the end of a string
 */
void removeNewline(char *string)
{
    string[strcspn(string, "\r\n")] = '\0'; 
}

/*
 * processEmailCommand
 * Separates an EMAIL command into:
 * sender category,
 * subject,
 * date.
 * Example:
 * EMAIL Boss,Never Mind,01-03-2025
 */
void processEmailCommand(MaxHeap *heap, char *line)
{
    Email email; /*Variable to hold a new email*/

    char *sender; /*Create all pointers to hold needed info*/
    char *subject;
    char *date;

    sender = strtok(line + 6, ","); /*Skips first 6 letters*/
    subject = strtok(NULL, ","); /*Looks for the next comma seperated pieces*/
    date = strtok(NULL, ","); /*Next comma piece*/

    if (sender == NULL || subject == NULL || date == NULL) /*Check if the line is missing anything required*/
    {
        return; /*Return if so*/
    }

    removeNewline(date); /*Removes the newline character from date if ones there*/

    strncpy(email.sender, sender, MAX_SENDER - 1); /*Copy the sender and makes sure its ends correctly*/
    email.sender[MAX_SENDER - 1] = '\0';

    strncpy(email.subject, subject, MAX_SUBJECT - 1); /*same as above but for subject*/
    email.subject[MAX_SUBJECT - 1] = '\0';

    strncpy(email.date, date, sizeof(email.date) - 1); /*same as above for date*/
    email.date[sizeof(email.date) - 1] = '\0';

    email.dateValue = convertDate(email.date); /*Changes the emails date value into a number ex 01 - 01 - 2025 == 20250101*/

    insertEmail(heap, email); /*Adds the new complete email to heap*/
}

/*
 * main
 * Opens the command file supplied on the command line
 * Each line is examined and the appropriate heap operation
 * is performed
 */
int main(int argc, char *argv[]) /*argc is amount of command-line pieces entered, argv stores them*/
{
    FILE *file; /*Creates variable to hold new file*/

    char line[512]; /*New pointer that will refer to the opened file*/
    char inputPath[512]; /*Place for the pointer to store lines from the file at a time*/

    MaxHeap heap; /*Creates a heap to reap and sort through priority of emails*/

    initializeHeap(&heap); /*Create a new heap and sets it to 0*/

    /*
     * Make sure the user supplied an input filename.
     */
    if (argc != 2) /*checks that the user gave one input filename*/
    {
        printf("Usage: %s <input file>\n", argv[0]); /*Tells user if file was entered or not*/

        return 1;
    }

    /* Open the supplied path first*/
    file = fopen(argv[1], "r");

    /*
     * If a relative filename was supplied from a different working
     * directory, also look in the directory containing the executable
     * Example: ./chatgpt_email_priority chatgpt_test1.txt
     */
    if (file == NULL && argv[1][0] != '/') /*If it cant find the file it looks somewhere else*/
    {
        char *lastSlash = strrchr(argv[0], '/'); /*finds the last / int the path to identify the folder*/

        if (lastSlash != NULL) /*Confroms if there is no / */
        {
            size_t directoryLength = (size_t)(lastSlash - argv[0] + 1); /*How many characters make up the path to that folder*/

            if (directoryLength + strlen(argv[1]) < sizeof(inputPath)) /*Check if a new path is needing to be built*/
            {
                snprintf(inputPath, sizeof(inputPath), "%.*s%s", /*Builds the path using the folder and file name*/
                         (int)directoryLength, argv[0], argv[1]);
                file = fopen(inputPath, "r"); /*Opens to test the file*/
            }
        }
    }

    /*
     * Stop if the file could not be opened.
     */
    if (file == NULL)
    {
        printf("Unable to open input file: %s\n", argv[1]);

        return 1;
    }

    /*
     * Read the test file one line at a time.
     */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        removeNewline(line);

        /*
         * EMAIL adds a new email to the MaxHeap.
         */
        if (strncmp(line, "EMAIL ", 6) == 0)
        {
            processEmailCommand(&heap, line);
        }

        /*
         * NEXT displays the root without removing it.
         */
        else if (strcmp(line, "NEXT") == 0)
        {
            nextEmail(&heap);
        }

        /*
         * READ removes the root without displaying it.
         */
        else if (strcmp(line, "READ") == 0)
        {
            readEmail(&heap);
        }

        /*
         * COUNT displays the number of unread emails.
         */
        else if (strcmp(line, "COUNT") == 0)
        {
            displayCount(&heap);
        }
    }

    /*
     * Close the input file.
     */
    fclose(file);

    return 0;
}
