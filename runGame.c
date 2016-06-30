/* Name: Go Fish Game Run
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

static void printTitle (char *title);
static void turnPrompt (void);
static void printHandDecision (Game g);
static void setUpDeck (Game g);
int letterToCard (char cardLetter); 
char numberToCard (int cardNumber); 

int main (int argc, char *argv[]) {
    int gameOver = FALSE;
    int turnOver = FALSE;

    system ("clear");
    printTitle ("Welcome To Go Fish");
    
    // Create the game ADT and action structure
    Game g = newGame ();
    action a;
    char tempCard;
    
    // Initialise a seed for random number generation
    srand (time (NULL));

    // Initialise the deck and player hands
    setUpDeck (g);

    while (!gameOver) {
        // Print statistics
        printf ("Round %d, player %d's turn.\n", getRoundNumber (g),
                getWhoseTurn (g));
        printf ("---------------------------\n\n");

        turnOver = FALSE;
        printHandDecision (g);

        while (!turnOver) {
            // Get action from user input or AI 
            if (getWhoseTurn (g) == PLAYER_1) {
                printf ("Which player would you like to ask? \n");
                scanf ("%d", &a.player);
                printf ("Which card would you like to take? \n");
                scanf (" %c", &tempCard);

                a.card = letterToCard (tempCard);
            } else {
                a = decideAction (g);
                tempCard = numberToCard (a.card);

                printf ("Which player would you like to ask? \n");
                printf ("%d\n", a.player);
                printf ("Which card would you like to take? \n");
                printf ("%c\n", tempCard);
            }

            if (a.player < PLAYER_1 || a.player > PLAYER_4 ||
                a.card < ACE || a.card > KING || a.player == getWhoseTurn (g)) {
                printf ("Invalid input. Draw a card. ");
                popOffDeck (g);
                turnOver = TRUE;
            } else {
                // Check if the current player and opponent have that card
                if (checkPlayer (g, a) && checkOpponent (g, a)) {
                    // Give the current player the card and draw
                    printf ("\nI have that card, here you go.\n");
                    takeFromPlayer (g, a);
                    turnOver = FALSE;
                } else {
                    printf ("\nGo fish!\n");
                    popOffDeck (g);
                    turnOver = TRUE;
                }
            }
            
            printHandDecision (g);
        }

        findSets (g);

        nextTurn (g);
        turnPrompt ();

        if (allCardsUsed (g)) {
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
    printf ("Press space and enter to go to the next player's turn...");

    // Create a delay so that the previous enter doesn't trigger the
    // next turn
    while (getchar () != ' ') {
    }

    system ("clear");
}

static void printHandDecision (Game g) {
    // Print out the player's hand if they are player 1, since they 
    // are an AI otherwise
    printf ("Your hand is: \n");
    printHand (g);
}

static void setUpDeck (Game g) {
    initialiseDeck (g);
    shuffleDeck (g);
    dealDeck (g);
}

int letterToCard (char cardLetter) {
    int cardNumber;

    switch (cardLetter) {
        case 'A': cardNumber = ACE;
        break;
        case 'J': cardNumber = JACK;
        break;
        case 'Q': cardNumber = QUEEN;
        break;
        case 'K': cardNumber = KING;
        break;
        default: cardNumber = (int)cardLetter - ASCII_0;
    }

    return cardNumber;
}

char numberToCard (int cardNumber) {
    char cardLetter;

    switch (cardNumber) {
        case ACE: cardLetter = 'A';
        break;
        case 10: cardLetter = 'X';
        break;
        case JACK: cardLetter = 'J';
        break;
        case QUEEN: cardLetter = 'Q';
        break;
        case KING: cardLetter = 'K';
        break;
        default: cardLetter = (char)(cardNumber + ASCII_0);
    }

    return cardLetter;
}
