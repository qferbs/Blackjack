/* Blackjack
Allows the user to play hands of blackjack with the computer until one of them
runs out of cherries or the player chooses to stop.
Author: Quinn Ferbers, Jacob Burggraf */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* definitions */
#define DECK_SIZE 53
#define SUIT_SIZE 13
#define BUST 21
#define HAND_SIZE 15
#define CHERRY_COUNT 10
#define COMP_PLAYER_MAX_SCORE 15

/* function prototypes */
void displayGame(int[], int[], int, int);
int getPlayerChoice();
int anotherRound();
void clean();
void initDeck(int[]);
void newHand(int[]);
void shuffleDeck(int[]);
int drawCard(int[]);
void drawToHand(int[], int[]);
int handTotal(const int[]);
void displayHands(const int[], const int[]);
void printCard(int);
char getSuit(int);
void displayCherries(int, int);
void displayScore(int[], int[]);

/* debug function prototypes */
void printArr(int[], int);


int main()
{
    /* cherries and hand array for player and computer; deck array */
    int cherriesP = CHERRY_COUNT;
    int cherriesC = CHERRY_COUNT;
    int deck[DECK_SIZE];
    int handP[HAND_SIZE];
    int handC[HAND_SIZE];

    /* random number generator seeding */
    srand(time(0));

    /* deck initialization */
    initDeck(deck);
    shuffleDeck(deck);

    /* loops rounds until someone runs out of cherries or player chooses to stop */
    do
    {
        newHand(handP);
        newHand(handC);


        /* initial draws made (second initial player draw in player draw loop below */
        drawToHand(handP, deck);
        drawToHand(handC, deck);
        drawToHand(handC, deck);


        /* player draw loop: asks to draw until player says no or goes bust */
        do
        {
            system("cls");
            drawToHand(handP, deck);
            displayGame(handP, handC, cherriesP, cherriesC);
        }
        while(handTotal(handP) != 0 && getPlayerChoice());


        /* computer draw loop: draws for computer until over it's max 'safe' score, has more score than the player, or goes bust */
        while(handTotal(handC) <= COMP_PLAYER_MAX_SCORE && handTotal(handC) < handTotal(handP) && handTotal(handC) != 0)
        {
            system("cls");
            drawToHand(handC, deck);
            displayGame(handP, handC, cherriesP, cherriesC);
        }


        /* checks who won, displays it, and handles cherries */
        if(handTotal(handP) > handTotal(handC))
        {
            system("cls");
            cherriesP++;
            cherriesC--;
            displayGame(handP, handC, cherriesP, cherriesC);
            printf("Player wins!\n");
        }
        else
        {
            system("cls");
            cherriesP--;
            cherriesC++;
            displayGame(handP, handC, cherriesP, cherriesC);
            printf("Computer wins!\n");
        }
    }
    while(cherriesP != 0 && cherriesC != 0 && anotherRound() == 1);


    /*  final game message with who won */
    if(cherriesP == 0)
        printf("\nThe computer has won the game!\nThanks for playing!\n");
    else if(cherriesC == 0)
        printf("\nThe player has won the game!\nThanks for playing!\n");
    else
        printf("\nThanks for playing!\n");

    return 0;
}

/* prints all game information to screen */
void displayGame(int handP[], int handC[], int cherriesP, int cherriesC)
{
    printf("-- BlackJack --\n");

    displayCherries(cherriesP, cherriesC);

    printf("Your Hand   Computer's Hand\n"
           "---------------------------------\n");
    displayHands(handP, handC);
    displayScore(handP, handC);

    printf("---------------------------------\n");
}

/* returns player input on whether to draw or not */
int getPlayerChoice()
{
    char choice;
    do
    {
        printf("draw again? (y/n): ");
        choice = getchar();

        if(choice != '\n')
            clean();
    }
    while(choice != 'y' && choice != 'n');

    if(choice == 'y')
        return 1;
    else if (choice == 'n')
        return 0;
    else
    {
        printf("error in getPlayerChoice(): input incorrectly handled\n");
        return -1;
    }

}

/* returns player input on if to play another round */
int anotherRound()
{
    char choice;
    do
    {
        printf("play another round? (y/n): ");
        choice = getchar();

        if(choice != '\n')
            clean();
    }
    while(choice != 'y' && choice != 'n');

    if(choice == 'y')
        return 1;
    else
        return 0;
}


/* cleans input stream */
void clean()
{
    while(getchar() != '\n');
}

/* initializes inputted deck array with numbers 0 - 51 as card identifiers, and a final number for number of cards left */
void initDeck(int deck[])
{
    for(int i = 0; i < DECK_SIZE - 1; i++)
        deck[i] = i + 1;
    deck[DECK_SIZE -1] = DECK_SIZE - 1;
}

/* clears the inputted hand array of values (Note: last value in hand array is number of cards in hand) */
void newHand(int hand[])
{
    for(int i = 0; i < HAND_SIZE; i++)
        hand[i] = 0;
}

/* shuffles inputted deck array */
void shuffleDeck(int deck[])
{
    int r;
    int temp;

    deck[DECK_SIZE - 1] = DECK_SIZE - 1;
    for(int i = 0; i < DECK_SIZE - 1; i++)
    {
        r = (rand() % (DECK_SIZE - 1 - i)) + i;

        temp = deck[r];
        deck[r] = deck[i];
        deck[i] = temp;
    }
}

/* returns next card in deck array, reduces cards left entry by 1, and shuffles if deck is currently empty */
int drawCard(int deck[])
{
    if(deck[DECK_SIZE - 1] == 0)
        shuffleDeck(deck);

    deck[DECK_SIZE - 1] -= 1;
    return deck[DECK_SIZE - 2 - deck[DECK_SIZE - 1]];
}

/* places a drawn card from inputted deck into inputted hand array and increases number of cards in hand entry */
void drawToHand(int hand[], int deck[])
{
    hand[hand[HAND_SIZE - 1]] = drawCard(deck);
    hand[HAND_SIZE - 1]++;
}

/* totals up inputted hand array and returns total value of the cards or 0 if bust*/
int handTotal(const int hand[])
{
    int total = 0;
    int i = 0;
    int cardVal;

    while(hand[i] != 0)
    {
        /* 1 = Ace, 2-10 = cards 2 - 10, 11,12,0 = J,Q,K */
        cardVal = hand[i]%SUIT_SIZE;

        if(cardVal == 0 || cardVal >= 11)
            cardVal = 10;

        total += cardVal;
        i++;
    }

    if(total > BUST)
        total = 0;

    return total;
}

/* prints both inputted hand arrays to screen */
void displayHands(const int hand1[], const int hand2[])
{
    int i = 0;

    while(hand1[i] != 0 || hand2[i] != 0)
    {
        if(hand1[i] != 0)
            printCard(hand1[i]);

        printf("\t\t");

        if(hand2[i] != 0)
            printCard(hand2[i]);

        printf("\n");

        i++;
    }
}

/* prints inputted card to screen as (value)(suit)*/
void printCard(int card)
{
    int cardNum = card%SUIT_SIZE;

    switch(cardNum)
    {
    case 0:
        printf(" K");
        break;
    case 12:
        printf(" Q");
        break;
    case 11:
        printf(" J");
        break;
    case 1:
        printf(" A");
        break;
    default:
        printf("%2d", cardNum);
        break;
    }

    printf("%c", getSuit(card));
}

/* returns character of suit for specific card value */
char getSuit(int card)
{
    char suit;
    /* hearts, spades, diamonds, clubs */
    if(card <= SUIT_SIZE)
        suit = 'H';
    else if(card > SUIT_SIZE && card <= SUIT_SIZE*2)
        suit = 'S';
    else if(card > SUIT_SIZE*2 && card <= SUIT_SIZE*3)
        suit = 'D';
    else if(card > SUIT_SIZE*3 && card <= SUIT_SIZE*4)
        suit = 'C';
    else
    {
        printf("Error in getSuit(): not a valid card.\n");
        suit = 0;
    }

    return suit;
}

/* prints cherry values to screen */
void displayCherries(int cherriesP, int cherriesC)
{
    printf("Player: %d (cherries)\tComputer: %d (cherries)\n", cherriesP, cherriesC);
}

/* prints scores to screen */
void displayScore(int handP[], int handC[])
{
    printf("Score: %d\tScore: %d\n", handTotal(handP), handTotal(handC));
}


/* utilities */
/* prints array to screen */
void printArr(int array[],int length)
{
    for(int i = 0; i < length; i++)
        printf("%d ", array[i]);
    printf("\n");
}


