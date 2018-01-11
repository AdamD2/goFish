/* Name: Go Fish Header File
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <curses.h>
#include <string.h>

// Definitions
#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

#define FALSE 0
#define TRUE 1

#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 3

#define NO_ONE 0
#define PLAYER_1 1
#define PLAYER_2 2
#define PLAYER_3 3
#define PLAYER_4 4

#define NUM_PLAYERS 4

#define PAIR_SIZE 3

#define ASCII_0 48

// Macros
#define clear_windows() wclear(stdscr);\
                        wclear(title);\
                        wclear(game);\
                        wclear(stats)

#define refresh_windows() wrefresh(title);\
                          wrefresh(game);\
                          wrefresh(stats)

#define add_boxes() box(title, 0, 0);\
                    box(game, 0, 0);\
                    box(stats, 0, 0)

#define delete_windows() delwin(title);\
                         delwin(game);\
                         delwin(stats)

typedef struct _node *link;    // a link points to a node

// This is the struct for a list node, including the valeu and suit
// as well as a pointer to the next node or null
typedef struct _node {
    int value;
    int suit;
    link next;
} node;

// A list is just represented by a pointer to a struct containing a 
// a pointer to the first node in it 
typedef struct _list{
    link head;
} *list; 

// Declare the ADT for the game struct
typedef struct _game *Game;

// Declare the struct to store an action taken by the player
typedef struct _action {
    int card;
    int player;
} action;

// Create or destroy the game struct
Game newGame (void);
void disposeGame (Game g);

// Initialise the deck
void initialiseDeck (Game g);
// Shuffle the deck
void shuffleDeck (Game g);
// Deal the deck to the player's hands
void dealDeck (Game g);

// Checks if the player's hands are all empty and the game is over
int allCardsUsed (Game g);

// Pops the card off the top of the deck into the current player's hand
void popOffDeck (Game g);

// Increment the turn variables
void nextTurn (Game g);

// Print the current player's hand
void printHand (Game g);

// Getter functions
int getWhoseTurn (Game g);
int getRoundNumber (Game g);
int getPairs (Game g, int player);
list getDeck (Game g);

// Search through the current player's deck and search for pairs,
// removing them and incrementing the pair count
void findSets (Game g);

// Check if the opponent has the card specified and returns TRUE or FALSE
int checkOpponent (Game g, action a);
// Check if the current player has the card specified
int checkPlayer (Game g, action a);
// Pops the card off the player's deck to the current player's hand
void takeFromPlayer (Game g, action a);

// Calculate and print the winner of a game
void calculateWinner (Game g);

// Return the length of a given list
int listLength (list l);
// Return the string length of the player's hand
int handLength (Game g);

// Get an AI to randomly decide an action
action decideAction (Game g);
