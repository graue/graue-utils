/* Note Frequency Calculator by Catatonic Porpoise <graue@oceanbase.org>

   This assumes the capital letters A to G and the digits
   0 to 9 are adjacent in the character set.

   Modified July 22, 2009, hot diggity. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define A4FREQ 440

void invalid(void);

int main(int argc, char *argv[]) {
    int noteval[] = {0, 2, -9, -7, -5, -4, -2};
    int note = 69, c;
    double freq;
    char notename[3];

    if (argc >= 2 && strlen(argv[1]) >= 3) {
        notename[0] = argv[1][0];
        notename[1] = argv[1][1];
        notename[2] = argv[1][2];
    } else {
        notename[0] = getchar();
        notename[1] = getchar();
        notename[2] = getchar();
    }

    c = toupper(notename[0]) - 'A';
    if(c<0 || c>6) invalid();

    note += noteval[c];

    switch(notename[1]) {
        case '-': /* Natural note: do nothing */
            break;
        case '#': /* Sharp: increase note by 1 */
            note++;
            break;
        case 'b': /* Flat: decrease note by 1 */
            note--;
            break;
        default: /* Something weird */
            invalid();
    }

    c = notename[2] - '0';
    if(c<0 || c>9) invalid();
    note += (c - 4) * 12;

    freq = 440 * pow(2.0, (note - 69) / 12.0);
    printf("%f\n",freq);
    return EXIT_SUCCESS;
}

void invalid(void) {
    puts("Invalid input");
    exit(EXIT_FAILURE);
}
