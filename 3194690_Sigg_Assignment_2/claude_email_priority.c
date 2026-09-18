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
 * Claude
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Maximum sizes used in the program */
#define MAX_SUBJECT_LEN   256   /*Maximum characters allowed in a subject*/
#define MAX_CATEGORY_LEN  32    /*Maximum characters allowed in a category*/
#define MAX_LINE_LEN       512  /*Maximum characters allowed in one line*/
#define INITIAL_CAPACITY   16   /*Starting size of the heap array*/

/*
 * Email structure stores the category, subject, date, and priority information
 * used to compare emails
 */
typedef struct {
    char category[MAX_CATEGORY_LEN];
    char subject[MAX_SUBJECT_LEN];
    char dateStr[16];
    int  month;
    int  day;
    int  year;
    int  catPriority;
} Email;

/*
 * MaxHeap structure stores emails in a dynamic array
 * size is the number of emails and capacity is the array size
 */
typedef struct {
    Email *data;
    int size;
    int capacity;
} MaxHeap;

/*
 * categoryPriority
 * Converts a sender category into a numeric priority
 * Larger numbers represent higher priority
 */
int categoryPriority(const char *category) {
    if (strcmp(category, "Boss") == 0) return 5; /*Boss is the highest priority*/
    if (strcmp(category, "Subordinate") == 0) return 4; /*Subordinate is second*/
    if (strcmp(category, "Peer") == 0) return 3; /*Peer is third*/
    if (strcmp(category, "ImportantPerson") == 0) return 2; /*ImportantPerson is fourth*/
    if (strcmp(category, "OtherPerson") == 0) return 1; /*OtherPerson is lowest*/
    return 0; /*Returns lowest priority for an unknown category*/
}

/*
 * emailIsHigherPriority
 * Checks if email a should be read before email b
 * Sender priority is checked first and newer dates win ties
 */
int emailIsHigherPriority(const Email *a, const Email *b) {
    /*Compare sender category priority first*/
    if (a->catPriority != b->catPriority) {
        return a->catPriority > b->catPriority; /*Returns true when a has the higher category*/
    }
    /*If categories match compare year, month, and then day*/
    if (a->year != b->year) return a->year > b->year; /*Checks the newest year*/
    if (a->month != b->month) return a->month > b->month; /*Checks the newest month*/
    if (a->day != b->day) return a->day > b->day; /*Checks the newest day*/
    return 0; /*Returns false when both emails have the same priority*/
}

/*
 * createHeap
 * Creates an empty MaxHeap
 */
MaxHeap *createHeap(void) {
    MaxHeap *heap = (MaxHeap *)malloc(sizeof(MaxHeap)); /*Creates memory for the heap*/
    if (heap == NULL) {
        fprintf(stderr, "Error: could not allocate heap.\n"); /*Prints an error if memory fails*/
        exit(EXIT_FAILURE); /*Stops the program*/
    }
    heap->data = (Email *)malloc(sizeof(Email) * INITIAL_CAPACITY); /*Creates the email array*/
    if (heap->data == NULL) {
        fprintf(stderr, "Error: could not allocate heap storage.\n"); /*Prints an error if memory fails*/
        exit(EXIT_FAILURE); /*Stops the program*/
    }
    heap->size = 0; /*Sets the heap size to zero*/
    heap->capacity = INITIAL_CAPACITY; /*Sets the starting array size*/
    return heap; /*Returns the new heap*/
}

/*
 * freeHeap
 * Frees the memory used by the heap
 */
void freeHeap(MaxHeap *heap) {
    if (heap != NULL) { /*Makes sure the heap exists*/
        free(heap->data); /*Frees the email array*/
        free(heap); /*Frees the heap*/
    }
}

/*
 * resizeHeapIfNeeded
 * Doubles the heap array when it is full
 */
void resizeHeapIfNeeded(MaxHeap *heap) {
    if (heap->size >= heap->capacity) { /*Checks if the heap array is full*/
        int newCapacity = heap->capacity * 2; /*Doubles the array size*/
        Email *newData = (Email *)realloc(heap->data, sizeof(Email) * newCapacity); /*Makes a larger array*/
        if (newData == NULL) {
            fprintf(stderr, "Error: could not resize heap storage.\n"); /*Prints an error if resizing fails*/
            exit(EXIT_FAILURE); /*Stops the program*/
        }
        heap->data = newData; /*Uses the larger array*/
        heap->capacity = newCapacity; /*Saves the new capacity*/
    }
}

/*
 * swapEmails
 * Swaps two emails in the heap
 */
void swapEmails(Email *a, Email *b) {
    Email temp = *a; /*Saves email a temporarily*/
    *a = *b; /*Moves email b into a*/
    *b = temp; /*Moves the saved email into b*/
}

/*
 * heapifyUp
 * Moves a new email up the heap until it is in the correct position
 */
void heapifyUp(MaxHeap *heap, int idx) {
    while (idx > 0) { /*Keeps going until the email reaches the root*/
        int parent = (idx - 1) / 2; /*Finds the parent position*/
        /*Check if the current email has a higher priority than its parent*/
        if (emailIsHigherPriority(&heap->data[idx], &heap->data[parent])) {
            swapEmails(&heap->data[idx], &heap->data[parent]); /*Swaps the two emails*/
            idx = parent; /*Moves to the parent position*/
        } else {
            break; /*Stop when the email is in the correct place*/
        }
    }
}

/*
 * heapifyDown
 * Moves an email down the heap after the root is removed
 */
void heapifyDown(MaxHeap *heap, int idx) {
    while (1) { /*Loops until the email is in the correct position*/
        int left = 2 * idx + 1; /*Finds the left child*/
        int right = 2 * idx + 2; /*Finds the right child*/
        int largest = idx; /*Starts by treating the current email as largest*/

        /*Check the left child priority*/
        if (left < heap->size && emailIsHigherPriority(&heap->data[left], &heap->data[largest])) {
            largest = left; /*Left child is now the largest*/
        }
        /*Check the right child priority*/
        if (right < heap->size && emailIsHigherPriority(&heap->data[right], &heap->data[largest])) {
            largest = right; /*Right child is now the largest*/
        }
        /*Stop if the current email is already the largest*/
        if (largest == idx) {
            break; /*Ends the loop*/
        }
        swapEmails(&heap->data[idx], &heap->data[largest]); /*Moves the larger email up*/
        idx = largest; /*Moves down to continue checking*/
    }
}

/*
 * heapInsert
 * Adds a new email to the heap and moves it into the correct position
 */
void heapInsert(MaxHeap *heap, Email email) {
    resizeHeapIfNeeded(heap); /*Makes room if the heap is full*/
    heap->data[heap->size] = email;   /*Add the email to the end of the heap*/
    heapifyUp(heap, heap->size);      /*Move it up if it has a higher priority*/
    heap->size++; /*Adds one to the number of emails*/
}

/*
 * heapPeekMax
 * Returns the highest-priority email without removing it
 */
Email *heapPeekMax(MaxHeap *heap) {
    if (heap->size == 0) { /*Checks if the heap is empty*/
        return NULL; /*Returns nothing when there are no emails*/
    }
    return &heap->data[0]; /*Returns the root email*/
}

/*
 * heapExtractMax
 * Removes the highest-priority email from the heap
 */
int heapExtractMax(MaxHeap *heap) {
    if (heap->size == 0) { /*Checks if there are emails to remove*/
        return 0; /*Return if there are no emails*/
    }
    /*Move the last email to the root and move it down if needed*/
    heap->data[0] = heap->data[heap->size - 1]; /*Moves the last email to the root*/
    heap->size--; /*Removes one email from the heap*/
    heapifyDown(heap, 0); /*Restores the heap order*/
    return 1; /*Shows that an email was removed*/
}

/*
 * trim
 * Removes extra spaces and newline characters from a string
 */
void trim(char *str) {
    /*Remove spaces from the end of the string*/
    int len = (int)strlen(str); /*Gets the string length*/
    while (len > 0 && (isspace((unsigned char)str[len - 1]))) { /*Checks the last character*/
        str[len - 1] = '\0'; /*Removes the last space or newline*/
        len--; /*Moves to the character before it*/
    }
    /*Remove spaces from the beginning of the string*/
    int start = 0; /*Starts at the beginning of the string*/
    while (str[start] != '\0' && isspace((unsigned char)str[start])) { /*Finds the first non-space*/
        start++; /*Moves past the space*/
    }
    if (start > 0) { /*Only moves text when spaces were found*/
        memmove(str, str + start, strlen(str) - start + 1); /*Moves the text to the start*/
    }
}

/*
 * parseDate
 * Separates a date into month, day, and year numbers
 */
void parseDate(const char *dateStr, int *month, int *day, int *year) {
    *month = 0; *day = 0; *year = 0; /*Sets default date values*/
    sscanf(dateStr, "%d-%d-%d", month, day, year); /*Reads the date numbers*/
}

/*
 * handleEmailCommand
 * Separates an EMAIL command and adds the email to the heap
 */
void handleEmailCommand(MaxHeap *heap, char *args) {
    /*Split the command into category, subject, and date*/
    char *category = strtok(args, ","); /*Gets the sender category*/
    char *subject  = strtok(NULL, ","); /*Gets the email subject*/
    char *date     = strtok(NULL, ","); /*Gets the email date*/

    if (category == NULL || subject == NULL || date == NULL) { /*Checks for missing email information*/
        fprintf(stderr, "Warning: malformed EMAIL command ignored.\n"); /*Prints a warning*/
        return; /*Stops the bad command*/
    }

    trim(category); /*Removes extra spaces from the category*/
    trim(subject); /*Removes extra spaces from the subject*/
    trim(date); /*Removes extra spaces from the date*/

    Email email; /*Creates a new email*/
    strncpy(email.category, category, MAX_CATEGORY_LEN - 1); /*Copies the category*/
    email.category[MAX_CATEGORY_LEN - 1] = '\0'; /*Makes sure the category ends correctly*/
    strncpy(email.subject, subject, MAX_SUBJECT_LEN - 1); /*Copies the subject*/
    email.subject[MAX_SUBJECT_LEN - 1] = '\0'; /*Makes sure the subject ends correctly*/
    strncpy(email.dateStr, date, sizeof(email.dateStr) - 1); /*Copies the date*/
    email.dateStr[sizeof(email.dateStr) - 1] = '\0'; /*Makes sure the date ends correctly*/

    parseDate(email.dateStr, &email.month, &email.day, &email.year); /*Separates the date for comparisons*/
    email.catPriority = categoryPriority(email.category); /*Sets the sender priority*/

    heapInsert(heap, email); /*Adds the complete email to the heap*/
}

/*
 * handleNextCommand
 * Displays the highest-priority email without removing it
 */
void handleNextCommand(MaxHeap *heap) {
    Email *top = heapPeekMax(heap); /*Gets the root email*/
    if (top == NULL) { /*Checks if there are no emails*/
        printf("No emails to read.\n"); /*Tells the user the heap is empty*/
        return; /*Stops the function*/
    }
    printf("Next email:\n"); /*Prints the next email label*/
    printf("Sender: %s\n", top->category); /*Prints the sender*/
    printf("Subject: %s\n", top->subject); /*Prints the subject*/
    printf("Date: %s\n", top->dateStr); /*Prints the date*/
}

/*
 * handleReadCommand
 * Removes the highest-priority email from the heap
 */
void handleReadCommand(MaxHeap *heap) {
    heapExtractMax(heap); /*Remove the top email*/
}

/*
 * handleCountCommand
 * Displays the number of emails in the heap
 */
void handleCountCommand(MaxHeap *heap) {
    printf("There are %d emails to read.\n", heap->size); /*Prints the number of emails*/
}

/*
 * processLine
 * Checks a command line and calls the correct function
 */
void processLine(MaxHeap *heap, char *line) {
    trim(line); /*Removes spaces and newlines from the command*/
    if (line[0] == '\0') { /*Checks for an empty line*/
        return; /*Skip blank lines*/
    }

    if (strncmp(line, "EMAIL ", 6) == 0) { /*Checks for an EMAIL command*/
        handleEmailCommand(heap, line + 6); /*Adds the email after the word EMAIL*/
    } else if (strcmp(line, "NEXT") == 0) { /*Checks for NEXT*/
        handleNextCommand(heap); /*Displays the top email*/
    } else if (strcmp(line, "READ") == 0) { /*Checks for READ*/
        handleReadCommand(heap); /*Removes the top email*/
    } else if (strcmp(line, "COUNT") == 0) { /*Checks for COUNT*/
        handleCountCommand(heap); /*Displays the number of emails*/
    } else {
        fprintf(stderr, "Warning: unrecognized command \"%s\" ignored.\n", line); /*Prints a warning for a bad command*/
    }
}

/*
 * main
 * Reads commands from an input file or the terminal
 */
int main(int argc, char *argv[]) {
    FILE *input = stdin; /*Uses the terminal as input by default*/

    /*Use the input file if one was given*/
    if (argc > 1) { /*Checks if an input filename was entered*/
        input = fopen(argv[1], "r"); /*Opens the input file for reading*/
        if (input == NULL) { /*Checks if the file could not open*/
            fprintf(stderr, "Error: could not open file \"%s\"\n", argv[1]); /*Prints the error*/
            return EXIT_FAILURE; /*Ends the program with an error*/
        }
    }

    MaxHeap *heap = createHeap(); /*Creates the email heap*/
    char line[MAX_LINE_LEN]; /*Stores one input line at a time*/

    /*Read and process one line at a time*/
    while (fgets(line, sizeof(line), input) != NULL) { /*Keeps reading until the file ends*/
        processLine(heap, line); /*Handles the current command*/
    }

    if (input != stdin) { /*Only closes input when it is a file*/
        fclose(input); /*Closes the input file*/
    }
    freeHeap(heap); /*Frees the memory used by the heap*/

    return EXIT_SUCCESS; /*Ends the program successfully*/
}
