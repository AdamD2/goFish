/* Name: Go Fish Game Engine
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

void removeZero (Game g);
void setToZero (Game g); 
link findPrev (list l, link target);
void bubbleSort (list l);
void swap (link elt);
void append (list l, link val);

typedef struct _player {
    list playerHand;
    int pairs;
} player;

typedef struct _game {
    list deck;
    player playerArray[NUM_PLAYERS];
    int whoseTurn;
    int roundNumber;
} game;

static void moveToFront (list l, int nodeNum);

Game newGame (void) {
    // Allocate an area in the heap to the game struct and assert that
    // it does not return null
    Game g = malloc (sizeof (struct _game));
    assert (g != NULL);

    // Set all of the lists to NULL
    g->deck = malloc (sizeof (struct _list));
    g->deck->head = NULL;
    for (int i = PLAYER_1 - 1; i < PLAYER_4; i++) {
        g->playerArray[i].playerHand = malloc (sizeof (struct _node));
        g->playerArray[i].playerHand->head = NULL;
        g->playerArray[i].pairs = 0;
    }
    
    // Set the variables to zero to start the game
    g->whoseTurn = 1;
    g->roundNumber = 0;

    return g;
}


void disposeGame (Game g) {
    // Free each player's hand as they will all be empty at this point
    for (int i = PLAYER_1 - 1; i < PLAYER_4; i++) {
        free (g->playerArray[i].playerHand);
        g->playerArray[i].playerHand = NULL;
        assert (g->playerArray[i].playerHand == NULL);
    }

    // Free the deck
    free (g->deck);
    g->deck = NULL;
    assert (g->deck == NULL);

    // Free the entire game
    free (g);
    g = NULL;
    assert (g == NULL);
}

void initialiseDeck (Game g) {
    // Create a new node and point the deck list to it
    assert (g->deck->head == NULL);
    link curr = malloc (sizeof (struct _node));
    g->deck->head = curr;
    link prev;

    // For loop to populate the deck list with all of the cards
    for (int i = ACE; i <= KING; i++) {
        for (int j = HEARTS; j <= SPADES; j++) {
            curr->value = i;
            curr->suit = j;
            curr->next = malloc (sizeof (struct _node));
            curr = curr->next;
        }
    }
    
    // Find the previous node
    prev = findPrev (g->deck, curr);
    prev->next = NULL;

    // Free the last node, which has no data in it
    free (curr);
    curr = NULL;
}

void shuffleDeck (Game g) {
    int random;
    
    // Move a card from a random position in the deck to the front
    // This happens 1000 times
    for (int i = 0; i < 1000; i++) {
        random = rand () % 50 + 1;
        moveToFront (g->deck, random);
    }
}

static void moveToFront (list l, int nodeNum) {
    link curr = l->head->next;
    link prev = l->head;
    
    // Move through the list up to the node specified
    for (int i = 0; i < nodeNum-1; i++) {
        prev = prev->next;
        curr = curr->next;
    }
    
    // Move the node to the start of the list
    prev->next = curr->next;
    curr->next = l->head;
    l->head = curr;
}

void dealDeck (Game g) {
    g->whoseTurn = 1;
    
    // Loop through dealing 5 cards to each player
    while (g->whoseTurn <= 4) {
        for (int i = 0; i < 5; i++) {
            popOffDeck (g);
        }
        
        g->whoseTurn++;
    }
    
    g->whoseTurn = 1;
}

int allCardsUsed (Game g) {
    int result = TRUE;

    // Store the original value of whoseTurn
    int originalTurn = g->whoseTurn;

    // If any of the players decks aren't null, the result is false 
    g->whoseTurn = PLAYER_1; 
    while (g->whoseTurn <= PLAYER_4) {
        if (getDeck (g)->head != NULL) {
            result = FALSE;
        }

        g->whoseTurn++;
    }
    
    // Restore the original turn number
    g->whoseTurn = originalTurn;
    return result;
}

void popOffDeck (Game g) {
    list l = g->deck;
    link oldElement;

    if (g->deck->head != NULL) {
        oldElement = l->head;
        l->head = oldElement->next;

        append (g->playerArray[g->whoseTurn-1].playerHand, oldElement);
    }
}

void nextTurn (Game g) {
    g->whoseTurn++;
    
    if (g->whoseTurn > PLAYER_4) {
        g->whoseTurn = PLAYER_1;
        g->roundNumber++;
    }
}

void printHand (Game g) {
    list l = g->playerArray[PLAYER_1-1].playerHand;
    link curr = l->head;

    if (curr != NULL) {
        printf ("[%d]", curr->value);
        curr = curr->next;

        while (curr != NULL) {
            printf (" [%d]", curr->value);
            curr = curr->next;
        }
    }

    printf ("\n\n");
}

int getWhoseTurn (Game g) {
    return g->whoseTurn;
}

int getRoundNumber (Game g) {
    return g->roundNumber;
}

int getPairs (Game g) {
    return g->playerArray[g->whoseTurn - 1].pairs;
}

list getDeck (Game g) {
    return g->playerArray[g->whoseTurn - 1].playerHand;
}

int checkOpponent (Game g, action a) {
    link curr = g->playerArray[a.player-1].playerHand->head;
    int result = FALSE;
    
    while (curr != NULL && result == FALSE) {
        if (curr->value == a.card) {
            result = TRUE;
        }

        curr = curr->next;
    }
    
    return result;
}

int checkPlayer (Game g, action a) {
    link curr = g->playerArray[g->whoseTurn - 1].playerHand->head;
    int result = FALSE;

    while (curr != NULL && result == FALSE) {
        if (curr->value == a.card) {
            result = TRUE;
        }

        curr = curr->next;
    }

    return result;
}

void findSets (Game g) {
    list hand = g->playerArray[g->whoseTurn-1].playerHand;

    bubbleSort (hand);

    if (listLength (hand) >= PAIR_SIZE) {
        setToZero (g);
    }

    removeZero (g);
}

void setToZero (Game g) {
    list hand = g->playerArray[g->whoseTurn-1].playerHand;
    link currCard = hand->head;

    // Set each card that is part of a set to 0
    for (int i = 0; i < listLength (hand) - 3; i++) {
            if (currCard->value == currCard->next->next->next->value) {
                g->playerArray[g->whoseTurn - 1].pairs++;
                printf ("Player %d has mad a set of %d\n",
                        g->whoseTurn, currCard->value);
                printf ("Player %d now has %d set(s)\n", g->whoseTurn,
                        g->playerArray[g->whoseTurn - 1].pairs);
                currCard->value = 0;
                currCard->next->value = 0;
                currCard->next->next->value = 0;
                currCard->next->next->next->value = 0;
                currCard = currCard->next->next->next;
                i += 3;
            }
            currCard = currCard->next;
        }
}

void removeZero (Game g) {
    list hand = g->playerArray[g->whoseTurn-1].playerHand;
    link curr = hand->head;
    link prev = NULL;
    link deadNode;

    // Remove and free every occurence of zero in the player's hand
    while (curr != NULL) {
        if (curr->value == 0) {
            deadNode = curr;
            curr = curr->next;
            if (prev == NULL) {
                hand->head = curr;
            } else {
                prev->next = curr;
            }
            free (deadNode);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

link findPrev (list l, link target) {
    link curr = l->head;
    link prev = NULL;
    int found = FALSE;

    if (curr == NULL) {
    } else {
        while (curr != NULL && !found) {
            if (curr == target) {
                found = TRUE;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }

    return prev;
}

void bubbleSort (list l) {
    link current = l->head;
    int listLen = listLength (l);
    int count = 0;
    
    while (count < listLen) {
        current = l->head;
        
        while (current->next != NULL) {
            if (current->value > current->next->value) {
                swap (current);
            }
        
            current = current->next;
        }
        
        count++;
    }
}

int listLength (list l) {
    link current = l->head;
    int count = 0;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count;
}


void swap (link elt) {
    int temp;
    
    temp = elt->value;
    elt->value = elt->next->value;
    elt->next->value = temp;
}

void append (list l, link val) {
    link curr = l->head;
    
    if (curr != NULL) {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        
        curr->next = val;
    } else {
        l->head = val;
    }
    
    val->next = NULL;
}

void takeFromPlayer (Game g, action a) {
    list player = g->playerArray[g->whoseTurn-1].playerHand;
    list opponent = g->playerArray[a.player-1].playerHand;
    link prev = NULL;
    link curr = opponent->head;
    link backup;

    while (curr != NULL) {
        if (curr->value == a.card) {
            backup = curr->next;

            append (player, curr);

            //Store node for removal (Not sure if this is needed)
            curr = backup;

            //Remove card and patch (relink) opponent's hand
            if (prev == NULL) {
                opponent->head = curr;
            } else {
                prev->next = curr;
            }

        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void calculateWinner (Game g) {
    int player = PLAYER_1;

    int max = getPairs (g);
    int winner = g->whoseTurn;

    int originalWhoseTurn = g->whoseTurn;
    g->whoseTurn = player;

    printf ("\n\nGame Over \n\n\n");

    while (player <= PLAYER_4) {
        printf ("Player %d has %d pairs.\n", player, getPairs (g));
        
        if (getPairs (g) > max) {
            winner = g->whoseTurn;
            max = getPairs (g);
        }

        player++;
        g->whoseTurn = player;
    }

    g->whoseTurn = originalWhoseTurn;

    printf ("\nStatistics:\n");
    printf ("Rounds - %d\n", getRoundNumber (g));
    printf ("Last Turn - %d\n", getWhoseTurn (g));
    printf ("Winner - Player %d!\n", winner);
    printf ("\nThanks for playing.\n");
}
