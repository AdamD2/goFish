/* Name: Go Fish AI Player
 * Creators: Adam Douglas, James Wardman
 * Date: 23-06-2016
 */

#include "Game.h"

action decideAction (Game g) {
    action nextAction;
    int me = getWhoseTurn (g);
    list playerHand = getDeck (g);
    int len = listLength (playerHand);

    // Choose a random card from the player's deck to ask for
    int cardNumber = rand () % len;
    link curr = playerHand->head;
    for (int i = 0; i < cardNumber; i++) {
        curr = curr->next;
    }
    nextAction.card = curr->value;

    // Choose a random player that isn't the current player to ask
    do {
        nextAction.player = rand() % NUM_PLAYERS + 1;
    } while (nextAction.player == me);

    return nextAction;
}
