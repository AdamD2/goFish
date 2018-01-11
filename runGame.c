/* Name: Go Fish Game Run
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

void main_loop(WINDOW* title, WINDOW* game, WINDOW* stats, int parent_y,
               int parent_x, int game_x, int title_space, char* title_string);
static void turnPrompt (void);
static void printHands (Game g, WINDOW* game, int parent_y, int game_x);
static void printStats (Game g, WINDOW* stats);
static void setUpDeck (Game g);
int letterToCard (char cardLetter); 
char numberToCard (int cardNumber); 

int main (int argc, char *argv[]) {
    initscr();      // Initialize curses
    keypad(stdscr, TRUE);   // Enable keyboard mapping
    nonl();         // No NL->CR/NL on output
    cbreak();       // Take input one at a time
    nodelay(stdscr, TRUE);  // Do not pause on getch()
    curs_set(0);    // Make cursor invisible
    echo();         // Do not echo input
    if (has_colors()) { // Set up colors
        start_color();
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_BLUE,    COLOR_BLACK);
        init_pair(4, COLOR_BLACK,   COLOR_GREEN);
    }

    // Window based variables
    int parent_x, parent_y;
    getmaxyx(stdscr, parent_y, parent_x);
    int game_x = parent_x-20;

    // Title
    char* title_string = "Go Fish";
    int title_space = (parent_x - 2 - strlen(title_string)) / 2; //Center

    // Create windows for the title, game and stats
    WINDOW *title = newwin(3, parent_x, 0, 0);
    WINDOW *game = newwin(parent_y-3, game_x, 3, 0);
    WINDOW *stats = newwin(parent_y-3, parent_x-game_x, 3, game_x);

    // Initial printing
    add_boxes();
    mvwprintw(title, 1, title_space, title_string);
    refresh_windows();

    // Main loop
    main_loop(title, game, stats, parent_y, parent_x, game_x, title_space,
              title_string);

    // Finish
    delete_windows();
    endwin();
    return EXIT_SUCCESS;
}

void main_loop(WINDOW* title, WINDOW* game, WINDOW* stats, int parent_y,
               int parent_x, int game_x, int title_space, char* title_string) {
    int new_y, new_x;
    int gameOver = FALSE;
    int turnOver = FALSE;

    // Create the game ADT and action structure
    Game g = newGame ();
    action a;
    char tempCard;
    
    // Initialise a seed for random number generation
    srand (time (NULL));

    // Initialise the deck and player hands
    setUpDeck (g);

    while (!gameOver) {
        // Handle resizing
        getmaxyx(stdscr, new_y, new_x);
        if (new_y != parent_y || new_x != parent_x) {
            parent_y = new_y;
            parent_x = new_x;

            game_x = parent_x-20;
            wresize(title, 3, new_x);
            title_space = (new_x-2-strlen(title_string))/2;
            wresize(game, new_y-3, game_x);
            wresize(stats, new_y-3, new_x-game_x);
            mvwin(stats, 3, game_x);

            clear_windows();
            mvwprintw(title, 1, title_space, title_string);
            refresh_windows();
        }

        // Print statistics and hands
        printStats (g, stats);
        printHands (g, game, parent_y, game_x);
        refresh_windows();

        turnOver = FALSE;

        while (!turnOver) {
            // Get action from user input or AI 
            if (getWhoseTurn (g) == PLAYER_1) {
                // TODO ask player which player and card to ask for
                /*printf ("Which player would you like to ask? \n");
                scanf ("%d", &a.player);
                printf ("Which card would you like to take? \n");
                scanf (" %c", &tempCard);*/

                a.card = letterToCard (tempCard);
            } else {
                a = decideAction (g);
                tempCard = numberToCard (a.card);

                // TODO create a message asking for a card based on ai input
                /*printf ("Which player would you like to ask? \n");
                printf ("%d\n", a.player);
                printf ("Which card would you like to take? \n");
                printf ("%c\n", tempCard);*/
            }

            if (a.player < PLAYER_1 || a.player > PLAYER_4 ||
                a.card < ACE || a.card > KING || a.player == getWhoseTurn (g)) {
                //printf ("Invalid input. Draw a card. ");
                popOffDeck (g);
                turnOver = TRUE;
            } else {
                // Check if the current player and opponent have that card
                if (checkPlayer (g, a) && checkOpponent (g, a)) {
                    // Give the current player the card and draw
                    //printf ("\nI have that card, here you go.\n");
                    takeFromPlayer (g, a);
                    turnOver = FALSE;
                } else {
                    //printf ("\nGo fish!\n");
                    popOffDeck (g);
                    turnOver = TRUE;
                }
            }
            
            printHands (g, game, parent_y, game_x);
        }

        findSets (g);

        nextTurn (g);
        turnPrompt ();

        if (allCardsUsed (g)) {
            gameOver = TRUE;
        }

        // Refresh each window
        add_boxes();
        refresh_windows();
    }
   
    calculateWinner (g);
    disposeGame (g);
}

static void turnPrompt (void) {
    // Pause until the player presses space
    while (TRUE) {
        int c = getch();

        if (c == ' ') break;
    }
}

static void printHands (Game g, WINDOW* game, int parent_y, int game_x) {
    // TODO Print out all player hands around the game board with question 
    // marks for AI players
    mvwprintw (game, parent_y-1, game_x-3-handLength(g), " ");
    printHand (g);
}

static void printStats (Game g, WINDOW* stats) {
    mvwprintw(stats, 1, 1, "Player: %d", getWhoseTurn(g));
    mvwprintw(stats, 2, 1, "Round: %d", getRoundNumber(g));
    mvwprintw(stats, 3, 1, "Player 1 Pairs: %d", getPairs(g, PLAYER_1));
    mvwprintw(stats, 4, 1, "Player 2 Pairs: %d", getPairs(g, PLAYER_2));
    mvwprintw(stats, 5, 1, "Player 3 Pairs: %d", getPairs(g, PLAYER_3));
    mvwprintw(stats, 6, 1, "Player 4 Pairs: %d", getPairs(g, PLAYER_4));
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
