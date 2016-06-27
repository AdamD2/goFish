/* Name: Go Fish Testing File
 * Creators: Adam Douglas, James Wardman
 * Date: 17-06-2016
 */

#include "Game.h"

void testFindPairs ();
list newList ();
void appendInt (list l, int val);

int main (int argc, char* argv[]) {
    


    testFindPairs();


    printf ("All tests passed you are awesome!\n");

    return EXIT_SUCCESS;
}

void testFindPairs () {
    int testArray1[6] = {1, 2, 3, 4, 5, 6};
    int testArray2[17] = {4, 4, 4, 4, 5, 6, 7, 7, 8, 8, 8, 10, 12, 12,
                         12, 13, 13};
    
    Game g = newGame ();

    list test = newList();
    g->playerArray[0].playerHand = test;


    printf ("%d\n", testArray2[0]);

    for (int i = 0; i < 6; i++) {
        appendInt (test, testArray1[i]);
    }

    assert (test->head->value == 1);
    //findPairs (g);

    printf ("findPairs works\n");
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
