/* Name: Go Fish Game Run
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

static void printTitle (char *title);
static void turnPrompt (void);

int main (int argc, char *argv[]) {
    int gameOver = FALSE;
    int turnOver = FALSE;

    printTitle ("Welcome To Go Fish");
    
    // Create the game ADT and action structure
    Game g = newGame ();
    action a;
    
    // Initialise a seed for random number generation
    srand (time (NULL));

    // Initialise the deck and player hands
    initialiseDeck (g);
    shuffleDeck (g);
    dealDeck (g);

    while (!gameOver) {
        // Print statistics
        printf ("Round %d, player %d's turn.\n", getRoundNumber (g),
                getWhoseTurn (g));
        printf ("---------------------------\n\n");
        printf ("Your hand is: \n");
        printHand (g);

        turnOver = FALSE;

        while (!turnOver) {
            // Get action from user input
            printf ("Which player would you like to ask? \n");
            scanf ("%d", &a.player);
            printf ("Which card would you like to take? \n");
            scanf ("%d", &a.card);

            if (a.player < PLAYER_1 || a.player > PLAYER_2 ||
                a.card < ACE || a.card > KING || a.player == getWhoseTurn (g)) {
                printf ("Invalid input. Draw a card. ");
                popOffDeck (g);
                turnOver = TRUE;
            } else {
                // Check if the current player and opponent have that card
                if (checkPlayer (g, a) && checkOpponent (g, a)) {
                    // Give the current player the card and draw
                    printf ("Take from player\n");
                    takeFromPlayer (g, a);
                    turnOver = FALSE;
                } else {
                    printf ("Take from deck\n");
                    popOffDeck (g);
                    turnOver = TRUE;
                }
            }

            printf ("Your new hand is: \n");
            printHand (g);
        }

//      findPairs (g);

        nextTurn (g);
        turnPrompt ();

        if (isDeckEmpty (g)) {
            gameOver = TRUE;
        }
    }
   
    calculateWinner (g);
    disposeGame (g);

    return EXIT_SUCCESS;
}

static void printTitle (char *title) {
    printf ("====================\n");
    printf ("%s\n", title);
    printf ("====================\n\n\n");
}

static void turnPrompt (void) {
    printf ("Press enter to go to the next player's turn...");
    getchar ();
    system ("clear");
}
