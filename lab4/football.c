#include <stdio.h>

void scorigami(int score) {
    printf("Possible scores for %d:\n", score);
     
    for(int td2pt = 0; td2pt <= score / 8; td2pt++) {
        for(int tdFG = 0; tdFG <= score / 7; tdFG++) {
            for(int td = 0; td <= score / 6; td++) {
                for(int fg = 0; fg <= score / 3; fg++) {
                    for(int s = 0; s <= score / 2; s++) {

                        int total = td2pt * 8 + tdFG * 7 + td * 6 + fg * 3 + s * 2;

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

    while(1) {
        printf("Enter NFL score (1 or 0 to quit): ");
        scanf("%d", &score);

        if (score <= 1) {
            break;
        }

        scorigami(score);
    }

    return 0;
}