#include <stdio.h>

/*
 * Program: NFL Score Combinations
 * Purpose: Reads an NFL score from the user and prints every possible
 * scoring combination that can produce that total. The program repeats
 * until the user enters 0 or 1.
 */

/* Prints every combination of NFL scoring plays that adds up to score. */
void scorigami(int score) {
    printf("Possible scores for %d:\n", score);
     
    /* Try every possible count for each type of scoring play. */
    for(int td2pt = 0; td2pt <= score / 8; td2pt++) {
        for(int tdFG = 0; tdFG <= score / 7; tdFG++) {
            for(int td = 0; td <= score / 6; td++) {
                for(int fg = 0; fg <= score / 3; fg++) {
                    for(int s = 0; s <= score / 2; s++) {

                        /* Calculate the score made by the current combination. */
                        int total = td2pt * 8 + tdFG * 7 + td * 6 + fg * 3 + s * 2;

                        /* Print only the combinations that match the user's score. */
                        if(total == score) {
                            printf("td2pt: %d, tdFG: %d, td: %d, fg: %d, s: %d\n", td2pt, tdFG, td, fg, s);
                        }
                    }
                }
            }
        }
    }
}

int main(void) {
    int score;

    /* Keep asking for scores until the user chooses to quit. */
    while(1) {
        printf("Enter NFL score (1 or 0 to quit): ");
        if (scanf("%d", &score) != 1) {
            printf("Error: Please enter a whole number score.\n");
            return 1;
        }

        /* Scores of 0 or 1 end the program. */
        if (score == 0 || score == 1) {
            break;
        }

        /* Negative scores are invalid, so ask again instead of ending. */
        if (score < 0) {
            printf("Error: Score cannot be negative.\n");
            continue;
        }

        /* Show all valid scoring combinations for the entered score. */
        scorigami(score);
    }

    return 0;
}
