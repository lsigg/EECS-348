/*
 * Program: EECS 348 Assignment 2
 *
 * Description:
 * Manages a CEO's email inbox using a MaxHeap priority queue
 * Emails are prioritized by sender category and then by newest date
 *
 * Input:
 * A text file containing EMAIL, NEXT, READ, and COUNT commands
 *
 * Output:
 * Displays email information and unread email counts
 *
 * Other Sources:
 * ChatGPT
 *
 * Author: YOUR NAME
 * Creation Date: September 17, 2026
 * Revision Date: September 17, 2026
 *
 * Revisions:
 * - Added dynamic heap resizing
 * - Added safer EMAIL parsing
 * - Added date validation
 * - Added memory allocation error handling
 * - Improved organization and maintainability
 */

/* Calling in all the required libraries for the project */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENDER 50 /* Maximum length allowed for a sender category */
#define MAX_SUBJECT 256 /* Maximum length allowed for a subject line */
#define MAX_LINE 512 /* Maximum length allowed for one input line */
#define INITIAL_CAPACITY 10 /* Starting size of the dynamic heap */

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
 * MaxHeap structure stores emails in a dynamic array
 * size stores the current number of unread emails
 */
typedef struct
{
    Email *list;
    int size;
    int capacity;
} MaxHeap;


/*
 * Returns the numeric priority of a sender
 * Higher numbers mean higher priority
 */
int senderPriority(const char *sender)
{
    if (strcmp(sender, "Boss") == 0) /*Checks for Boss and sets the highest priority*/
        return 5;

    if (strcmp(sender, "Subordinate") == 0) /*Sets Subordinate to second priority*/
        return 4;

    if (strcmp(sender, "Peer") == 0) /*Sets Peer to third priority*/
        return 3;

    if (strcmp(sender, "ImportantPerson") == 0) /*Sets ImportantPerson to fourth priority*/
        return 2;

    if (strcmp(sender, "OtherPerson") == 0) /*Sets OtherPerson to the lowest priority*/
        return 1;

    return 0; /*Returns zero for an invalid sender*/
}


/*
 * Converts MM-DD-YYYY into YYYYMMDD
 *
 * Returns -1 if the date is invalid
 */
int convertDate(const char *date)
{
    int month; /*Creates variables for the date*/
    int day;
    int year;
    char extra; /*Checks for extra characters*/

    /*
     * The extra %c detects unwanted characters after the date
     */
    if (sscanf(date, "%d-%d-%d%c",
               &month, &day, &year, &extra) != 3)
    {
        return -1; /*Returns an error for a bad date format*/
    }

    /*
     * Basic date validation
     */
    if (month < 1 || month > 12)
        return -1; /*Returns an error for an invalid month*/

    if (day < 1 || day > 31)
        return -1; /*Returns an error for an invalid day*/

    if (year < 0)
        return -1; /*Returns an error for an invalid year*/

    return year * 10000 + month * 100 + day; /*Turns the date into YYYYMMDD*/
}


/*
 * Determines whether email a has higher priority than email b
 */
int higherPriority(const Email *a, const Email *b)
{
    int priorityA = senderPriority(a->sender); /*Gets sender a priority*/
    int priorityB = senderPriority(b->sender); /*Gets sender b priority*/

    /*
     * First compare sender categories
     */
    if (priorityA != priorityB)
    {
        return priorityA > priorityB; /*Returns true when a has higher priority*/
    }

    /*
     * If the senders have equal priority,
     * the newer email has higher priority
     */
    return a->dateValue > b->dateValue; /*Uses the newest date when priorities match*/
}


/*
 * Swaps two emails
 */
void swapEmails(Email *a, Email *b)
{
    Email temp = *a; /*Saves email a*/
    *a = *b; /*Moves b into a*/
    *b = temp; /*Moves saved a into b*/
}


/*
 * Initializes the MaxHeap
 *
 * Returns 1 on success and 0 if memory allocation fails
 */
int initializeHeap(MaxHeap *heap)
{
    heap->size = 0; /*Sets the heap size to zero*/
    heap->capacity = INITIAL_CAPACITY; /*Sets the starting capacity*/

    heap->list = malloc( /*Creates memory for the email list*/
        heap->capacity * sizeof(Email)
    );

    if (heap->list == NULL)
    {
        return 0; /*Returns failure when memory could not be created*/
    }

    return 1; /*Returns success*/
}


/*
 * Doubles the heap capacity when it becomes full
 *
 * Returns 1 on success and 0 if memory allocation fails
 */
int resizeHeap(MaxHeap *heap)
{
    int newCapacity = heap->capacity * 2; /*Doubles the heap capacity*/

    Email *newList = realloc( /*Creates a larger email list*/
        heap->list,
        newCapacity * sizeof(Email)
    );

    /*
     * Do not change the original pointer if realloc fails
     */
    if (newList == NULL)
    {
        return 0; /*Returns failure and keeps the old list*/
    }

    heap->list = newList; /*Uses the resized list*/
    heap->capacity = newCapacity; /*Saves the new capacity*/

    return 1; /*Returns success*/
}


/*
 * Inserts an email into the MaxHeap
 *
 * Returns 1 on success and 0 on failure
 */
int insertEmail(MaxHeap *heap, Email email)
{
    int index; /*Creates needed variables*/
    int parent;

    /*
     * Expand the heap if it is full
     */
    if (heap->size == heap->capacity) /*Checks if the heap is full*/
    {
        if (!resizeHeap(heap)) /*Tries to make more room*/
        {
            return 0; /*Stops when resizing fails*/
        }
    }

    /*
     * Insert the email at the end
     */
    index = heap->size; /*Sets index to the bottom of the heap*/
    heap->list[index] = email; /*Adds the new email*/
    heap->size++; /*Adds one to the email count*/

    /*
     * Move the email upward until the
     * MaxHeap property is restored
     */
    while (index > 0) /*Moves the email up until it is correct*/
    {
        parent = (index - 1) / 2; /*Finds the parent node*/

        if (higherPriority(
                &heap->list[index],
                &heap->list[parent]))
        {
            swapEmails( /*Swaps the email with its parent*/
                &heap->list[index],
                &heap->list[parent]
            );

            index = parent; /*Moves up the heap*/
        }
        else
        {
            break; /*Stops when the email is in the correct place*/
        }
    }

    return 1; /*Shows the email was added*/
}


/*
 * Moves an email downward until the
 * MaxHeap property is restored
 */
void heapifyDown(MaxHeap *heap, int index)
{
    while (1) /*Keeps checking until the heap is in order*/
    {
        int left = 2 * index + 1; /*Finds the left child*/
        int right = 2 * index + 2; /*Finds the right child*/
        int largest = index; /*Starts with the current email as largest*/

        /*
         * Check the left child
         */
        if (left < heap->size &&
            higherPriority(
                &heap->list[left],
                &heap->list[largest]))
        {
            largest = left; /*Left child is now largest*/
        }

        /*
         * Check the right child
         */
        if (right < heap->size &&
            higherPriority(
                &heap->list[right],
                &heap->list[largest]))
        {
            largest = right; /*Right child is now largest*/
        }

        /*
         * If the current email is already the largest,
         * the heap is valid
         */
        if (largest == index)
        {
            break; /*Stops when the heap is in order*/
        }

        swapEmails( /*Moves the larger email upward*/
            &heap->list[index],
            &heap->list[largest]
        );

        index = largest; /*Continues checking down the heap*/
    }
}


/*
 * Displays the highest-priority email
 *
 * NEXT does not remove the email
 */
void nextEmail(const MaxHeap *heap)
{
    if (heap->size == 0) /*Checks if the heap is empty*/
    {
        return; /*Stops when there are no emails*/
    }

    printf("Next email:\n"); /*Prints the next email label*/
    printf("Sender: %s\n", heap->list[0].sender); /*Prints the sender*/
    printf("Subject: %s\n", heap->list[0].subject); /*Prints the subject*/
    printf("Date: %s\n", heap->list[0].date); /*Prints the date*/
}


/*
 * Removes the highest-priority email
 *
 * READ does not display the email
 */
void readEmail(MaxHeap *heap)
{
    if (heap->size == 0)
    {
        return;
    }

    /*
     * Reduce the heap size
     */
    heap->size--; /*"Reads" the highest-priority email*/

    /*
     * If emails remain, move the last email
     * to the root and restore the heap
     */
    if (heap->size > 0)
    {
        heap->list[0] = heap->list[heap->size]; /*Moves the last email to the root*/

        heapifyDown(heap, 0); /*Restores the heap order*/
    }
}


/*
 * Displays the number of unread emails
 */
void displayCount(const MaxHeap *heap)
{
    printf(
        "There are %d emails to read.\n",
        heap->size
    );
}


/*
 * Removes newline characters from a string
 */
void removeNewline(char *string)
{
    string[strcspn(string, "\r\n")] = '\0'; /*Removes the newline character*/
}


/*
 * Processes an EMAIL command
 *
 * Expected format:
 * EMAIL sender,subject,MM-DD-YYYY
 */
void processEmailCommand(MaxHeap *heap, char *line)
{
    Email email; /*Variable to hold a new email*/

    char *sender;
    char *subject;
    char *date;

    /*
     * Skip "EMAIL " and separate the three fields
     */
    sender = strtok(line + 6, ","); /*Skips EMAIL and gets the sender*/
    subject = strtok(NULL, ","); /*Gets the subject*/
    date = strtok(NULL, ","); /*Gets the date*/

    /*
     * Make sure all required fields exist
     */
    if (sender == NULL ||
        subject == NULL ||
        date == NULL)
    {
        fprintf(stderr, "Invalid EMAIL command.\n");
        return;
    }

    removeNewline(date); /*Removes the newline from the date*/

    /*
     * Make sure the sender category is valid
     */
    if (senderPriority(sender) == 0)
    {
        fprintf(stderr, "Invalid sender category.\n");
        return;
    }

    /*
     * Validate and convert the date once
     */
    email.dateValue = convertDate(date); /*Changes the date into a number*/

    if (email.dateValue == -1)
    {
        fprintf(stderr, "Invalid email date.\n"); /*Tells the user the date is bad*/
        return;
    }

    /*
     * Safely copy the strings into the Email structure
     */
    snprintf(
        email.sender,
        sizeof(email.sender),
        "%s",
        sender
    );

    snprintf(
        email.subject,
        sizeof(email.subject),
        "%s",
        subject
    );

    snprintf(
        email.date,
        sizeof(email.date),
        "%s",
        date
    );

    /*
     * Insert the completed email into the heap
     */
    if (!insertEmail(heap, email)) /*Adds the complete email to the heap*/
    {
        fprintf(stderr, "Unable to add email.\n");
    }
}


/*
 * Releases dynamically allocated heap memory
 */
void freeHeap(MaxHeap *heap)
{
    free(heap->list); /*Frees the email list*/

    heap->list = NULL; /*Clears the list pointer*/
    heap->size = 0; /*Resets the heap size*/
    heap->capacity = 0; /*Resets the heap capacity*/
}


/*
 * Main program
 */
int main(int argc, char *argv[])
{
    FILE *file; /*Creates a pointer for the input file*/

    char line[MAX_LINE]; /*Stores one command line at a time*/

    MaxHeap heap; /*Creates the email heap*/

    /*
     * Make sure an input file was provided
     */
    if (argc != 2) /*Checks that one input file was entered*/
    {
        fprintf(
            stderr,
            "Usage: %s <input file>\n",
            argv[0]
        );

        return 1;
    }

    /*
     * Initialize the MaxHeap
     */
    if (!initializeHeap(&heap)) /*Creates the heap and checks for errors*/
    {
        fprintf(stderr, "Unable to allocate memory.\n");

        return 1;
    }

    /*
     * Open the input file
     */
    file = fopen(argv[1], "r"); /*Opens the input file for reading*/

    if (file == NULL)
    {
        fprintf(stderr, "Unable to open input file.\n");

        freeHeap(&heap);

        return 1;
    }

    /*
     * Process every command in the input file
     */
    while (fgets(line, sizeof(line), file) != NULL) /*Reads every command in the file*/
    {
        removeNewline(line); /*Removes the newline from the command*/

        if (strncmp(line, "EMAIL ", 6) == 0)
        {
            processEmailCommand(&heap, line); /*Adds the email command to the heap*/
        }
        else if (strcmp(line, "NEXT") == 0)
        {
            nextEmail(&heap); /*Displays the top email*/
        }
        else if (strcmp(line, "READ") == 0)
        {
            readEmail(&heap); /*Removes the top email*/
        }
        else if (strcmp(line, "COUNT") == 0)
        {
            displayCount(&heap); /*Displays the number of emails*/
        }
    }

    /*
     * Clean up all resources
     */
    fclose(file); /*Closes the input file*/
    freeHeap(&heap); /*Frees heap memory*/

    return 0;
}
