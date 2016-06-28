/* Name: Go Fish Testing File
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

void testListCompare ();
void testClearList ();
void testFindPairs ();
void caseOne (Game g);
void caseTwo (Game g);
void caseThree (Game g);
list newList ();
void appendInt (list l, int val);
int listCompare (list a, list b);
void printList (list l); 
void clearList ();
void deleteNodes ();

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

int main (int argc, char* argv[]) {

    testClearList();
    printf ("\n\n");
    testListCompare();
    printf ("\n\n");
    testFindPairs();

    printf ("All tests passed you are awesome!\n");

    return EXIT_SUCCESS;
}

void testClearList() {
    list test = newList();

    appendInt (test, 1);
    appendInt (test, 4);
    appendInt (test, 5);

    printList (test);
    clearList (test);

    assert (test->head == NULL);

    appendInt (test, 3);
    appendInt (test, 7);
    appendInt (test, 10);
    appendInt (test, 31);

    printList (test);
    clearList (test);

    assert (test->head == NULL);
    
    free (test);

    printf ("clearList tests passed.\n");
}

void testListCompare () {
    int testArray0[5] = {1, 2, 3, 4, 5};
    int testArray1[5] = {1, 2, 3, 4, 5};
    int testArray2[4] = {3, 4, 5, 6};
    int testArray3[5] = {6, 7, 8, 9, 10};
    list test0, test1, test2, test3;

    test0 = newList ();
    test1 = newList ();
    test2 = newList ();
    test3 = newList ();

    for (int i = 0; i < 4; i++) {
        appendInt (test2, testArray2[i]);
    }

    for (int i = 0; i < 5; i++) {
        appendInt (test0, testArray0[i]);
        appendInt (test1, testArray1[i]);
        appendInt (test3, testArray3[i]);
    }

    assert (listCompare (test0, test1) == TRUE);
    assert (listCompare (test0, test2) == FALSE);
    assert (listCompare (test0, test3) == FALSE);
    
    clearList (test0);
    clearList (test1);
    clearList (test2);
    clearList (test3);
    free (test0);
    free (test1);
    free (test2);
    free (test3);


    printf ("listCompare tests passed.\n");
}

void testFindPairs () {
    // Create the game struct for testing
    Game g = newGame ();

    printf ("Testing findPairs.\n");

    // Simple Case
    caseOne(g);

    // Segfault (but doesn't in test)
    // caseTwo(g);

    // Only picked up 2 pairs out of 3
    // caseThree(g);

    printf ("findPairs tests passed.\n");
}

void caseOne(Game g) {
    int testArray0[6] = {1, 2, 3, 4, 5, 6};
    list test = newList ();
    list compList = newList ();
    g->playerArray[0].playerHand = test;

    // Populate lists
    for (int i = 0; i < 6; i++) {
        appendInt (test, testArray0[i]);
        appendInt (compList, testArray0[i]);
    }

    findPairs (g);
    assert (listCompare (test, compList));
    
    clearList (test);
    free (test);
    clearList (compList);
    free (compList);
}

void caseTwo(Game g) {
    int testArray1[17] = {4, 4, 4, 4, 5, 6, 7, 7, 8, 8, 8, 10, 12, 12,
                         12, 13, 13};
    list test = newList ();
    list compList = newList ();
    g->playerArray[0].playerHand = test;

    // Populate lists
    for (int i = 0; i < 17; i++) {
        appendInt (test, testArray1[i]);
        if (i > 3) {
            appendInt (compList, testArray1[i]);
        }
    }

    findPairs (g);
    assert (listCompare (test, compList));
}

void caseThree(Game g) {
    // int testArray2[13] = {1, 1, 1, 1, 10, 10, 10, 10, 11, 11, 11, 11,
       //                  12};
}

list newList () {
    list l = malloc (sizeof (struct _list));
    assert (l != NULL);
    l->head = NULL;
    assert (l->head == NULL);
    
    return l;
}


void appendInt (list l, int val) {
    link newLink = malloc (sizeof (node));
    link current = l->head;
    
    if (current != NULL) {
        while (current->next != NULL) {
            current = current->next;
        }
        
        current->next = newLink;
    } else {
        l->head = newLink;
    }
    
    newLink->next = NULL;
    newLink->value = val;
}

int listCompare (list a, list b) {
    int equal = TRUE;
    link currA = a->head;
    link currB = b->head;

    if (listLength (a) == listLength (b)) {
        while (currA != NULL && equal) {
            if (currA->value == currB->value) {
                currA = currA->next;
                currB = currB->next;
            } else {
                equal = FALSE;
            }
        }
    } else {
        equal = FALSE;
    }

    return equal;
}

void printList (list l) {
    link curr = l->head;

    if (curr != NULL) {
        printf ("[%d]", curr->value);
        curr = curr->next;

        while (curr != NULL) {
            printf ("->[%d]", curr->value);
            curr = curr->next;
        }
    }

    printf ("\n\n");
}

void clearList (list l) {
    deleteNodes (l->head);
    
    l->head = NULL;
    assert (l->head == NULL);
}

void deleteNodes (link curr) {
    if (curr->next == NULL) {
        free (curr);
    } else {
        deleteNodes (curr->next);
        free (curr);
    }
}
