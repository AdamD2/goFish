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
void findSets (Game g);
void setToZero (Game g); 
void removeZero (Game g);
void append (list l, link val);
list newList ();
void appendInt (list l, int val);
int listCompare (list a, list b);
void printList (list l); 
void clearList ();
void deleteNodes ();
//BUBBLE SORT STUFF
void bubbleSort (list l);
void swap (link elt);

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

    // First test
    appendInt (test, 1);
    appendInt (test, 4);
    appendInt (test, 5);

    printList (test);
    clearList (test);

    assert (test->head == NULL);

    // Second test
    appendInt (test, 3);
    appendInt (test, 7);
    appendInt (test, 10);
    appendInt (test, 31);

    printList (test);
    clearList (test);

    assert (test->head == NULL);

    /* First test using a partial delete
    appendInt (test, 1);
    appendInt (test, 1);
    appendInt (test, 2);
    appendInt (test, 3);
    appendInt (test, 4);
    appendInt (test, 5);
    
    clearBelow (test, 2);*/

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

    printf ("Testing findSets.\n");

    // Simple Case
    caseOne (g);

    // Segfault (but doesn't in test)
    caseTwo (g);

    // Only picked up 2 pairs out of 3
    caseThree (g);

    disposeGame (g);

    printf ("findSets tests passed.\n");
}

void caseOne(Game g) {
    // Simple Case
    int testArray0[6] = {1, 2, 3, 4, 5, 6};
    list test = g->playerArray[0].playerHand;
    list compList = newList ();

    // Populate lists
    for (int i = 0; i < 6; i++) {
        appendInt (test, testArray0[i]);
        appendInt (compList, testArray0[i]);
    }

    findSets (g);
    assert (listCompare (test, compList));
    printf ("Case one worked!\n");
    
    clearList (test);
    clearList (compList);
    free (compList);
}

void caseTwo(Game g) {
    // Segfault 
    // EDIT: Doesn't cause segfault in this test
    // EDIT 2: It does now
    int testArray1[17] = {4, 4, 4, 4, 5, 6, 7, 7, 8, 8, 8, 10, 12, 12,
                         12, 13, 13};
    list test = g->playerArray[0].playerHand;
    list compList = newList ();

    // Populate lists
    for (int i = 0; i < 17; i++) {
        appendInt (test, testArray1[i]);
        if (i > 3) {
            appendInt (compList, testArray1[i]);
        }
    }

    // Tests lists
    printHand (g);
    findSets (g);
    printHand (g);
    assert (listCompare (test, compList));
    printf ("Case two worked!\n");

    clearList (test);
    clearList (compList);
    free (compList);
}

void caseThree(Game g) {
    // Only removed 2/3 pairs
    int testArray2[13] = {1, 1, 1, 1, 10, 10, 10, 10, 11, 11, 11, 11,
                          12};
    list test = g->playerArray[0].playerHand;
    list compList = newList ();

    // Populate lists
    for (int i = 0; i < 13; i++) {
        appendInt (test, testArray2[i]);
    }
    appendInt (compList, 12);

    printHand (g);
    findSets (g);
    printHand (g);

    assert (listCompare (test, compList));

    printf ("Case three worked!\n");
}

list newList () {
    list l = malloc (sizeof (struct _list));
    assert (l != NULL);
    l->head = NULL;
    assert (l->head == NULL);
    
    return l;
}

/*void findSets (Game g) {
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

    for (int i = 0; i < listLength (hand) - 3; i++) {
            if (currCard->value == currCard->next->next->next->value) {
                currCard->value = 0;
                currCard->next->value = 0;
                currCard->next->next->value = 0;
                currCard->next->next->next->value = 0;
                currCard = currCard->next->next->next;
                i += 3;
            }
            currCard = currCard->next;
        }
}*/

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
