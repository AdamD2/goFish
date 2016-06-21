/* Name: Go Fish Header File
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

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

typedef struct _node *link;    // a link points to a node

typedef struct _node {
    int value;
    int suit;
    link next;
} node;

// a list is just represented by a pointer to a struct containing a 
// a pointer to the first node in it 
typedef struct _list{
    link head;
} *list; 

typedef struct _game *Game;

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

// Checks if the deck is empty and the game is over
int isDeckEmpty (Game g);

// Pops the card off the top of the deck into the current player's hand
void popOffDeck (Game g);

// Increment the turn variables
void nextTurn (Game g);

// Print the current player's hand
void printHand (Game g);

// Getter functions
int getWhoseTurn (Game g);
int getRoundNumber (Game g);
int getPairs (Game g);

// Search through the current player's deck and search for pairs,
// removing them and incrementing the pair count
void findPairs (Game g);

// Check if the opponent has the card specified and returns TRUE or FALSE
int checkOpponent (Game g, action a);
// Check if the current player has the card specified
int checkPlayer (Game g, action a);
// Pops the card off the player's deck to the current player's hand
void takeFromPlayer (Game g, action a);

// Calculate and print the winner of a game
void calculateWinner (Game g);
