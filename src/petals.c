/*
 * ============================================================================
 *
 *       Filename:  petals.c
 *
 *    Description:  Petals Around The Rose is a puzzle game wherein the 
 *                  objective is to decipher the rules of the game. A player
 *                  (who already knows the rules) throws a handful of 
 *                  (typically) 5 dice and states an answer. The objective for
 *                  observers is to figure out the means by which the first 
 *                  player reached their answer.
 *
 *                  In Petals Around The Rose, the hint is in the title of the
 *                  game. Three faces on a six sided dice contain a dot in the
 *                  middle - 1, 3 and 5. The "petals" therefore are the dots
 *                  which surround the "rose" at the center. The answer is 
 *                  found by counting the number of petals which surround a 
 *                  rose in each roll of the dice.  Obviously, even though the 
 *                  number 1 is a rose, it has no petals and thus equates to 0. 
 *                  So the only rolls which really matter are 3 and 5 which 
 *                  equate to 2 and 4 petals respectively.
 *
 *                  This program could be extended to contain a number of 
 *                  different functions which would produce different answers
 *                  based on different properties of the rolled dice. The
 *                  game would then select a random rule set when starting so
 *                  that there is at least some reason to play more than once.
 *
 *        Version:  1.0
 *        Created:  15/07/17 14:16:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gary Munnelly (gm), munnellg@tcd.ie
 *        Company:  Adapt Centre, Trinity College Dublin
 *
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define NUM_DICE 5   /* number of dice we should roll */
#define ROLL    'y'  /* any user input other than this signals quit */

#define DICE_PER_ROW 5 /* number of dice to be rendered per console row */

/* helper macro for extracting indexed die from Cup */
#define READ_DIE( c, i ) ((int)((((c) >> 3*(i))%6) + 1))

/* Cup capable of holding up to 21 six-sided dice (3 bits per die) */
typedef uint64_t Cup;


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  print_rules
 *  Description:  Greets the player and tells them how to play
 * ============================================================================
 */
void
print_rules() {
    static char *rules = 
    "===================================================================\n\n"
    "Welcome to Petals Around the Rose. In this game you will be shown\n"
    "the result of %d die rolls. You will then be shown an \"answer\".\n"
    "Your goal is to figure out why the answer is correct for the given\n"
    "configuration of die rolls.\n\nGood luck!!!\n\n"
    "===================================================================\n\n";

    printf(rules, NUM_DICE);
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  render
 *  Description:  Visually renders the dice in the console for the player.
 *                The whole point of petals around the rose is pattern
 *                recognition, so it makes sense to actually put in a bit of
 *                effort to display the dice, rather than just their values
 * ============================================================================
 */
void
render ( Cup c, int n) {
    int i, j, d;

    /* loop to control number of dice per row */
    for( j=0; j<n; j+=DICE_PER_ROW) {
        printf("\n");
        
        /* top line above dice */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            printf(" -------     ");
        }
        printf("\n");
        
        /* print first row of dots for each dice */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            d = READ_DIE(c, i);
            printf("| %c   %c |    ", 
                (d==1||d==3)? ' ' : '*', 
                (d<3)? ' ' : '*'
            );
        }
        printf("\n");
        
        /* print second row of dots for each dice */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            d = READ_DIE(c, i);
            printf("| %c %c %c |    ", 
                (d==6)? '*' : ' ', 
                (d%2)? '*' : ' ', 
                (d==6)? '*' : ' '
            );
        }
        printf("\n");
        
        /* print third row of dots for each dice */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            d = READ_DIE(c, i);
            printf("| %c   %c |    ", 
                (d>2)? '*' : ' ', 
                (d==1||d==3)? ' ' : '*'
            );
        }
        printf("\n");
        
        /* bottom line beneath dice */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            printf(" -------     ");
        }
        printf("\n");
        
        /* generally not a bad idea to just print the dice values too */
        for( i=j; i<j+DICE_PER_ROW && i<n; i++ ) {
            d = READ_DIE(c, i);
            printf("%5d        ", d );
        }
        
        /* big line break to distinguish between rows */
        printf("\n\n");
    }   
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  answer
 *  Description:  Compute the answer for petals around the rose given the 
 *                input dice configuration. This is basically just a case 
 *                of computing (count(3)*2 + count(5)*4)
 * ============================================================================
 */
int
answer ( Cup c, int n ) {
    int d, i, s;
    
    /* initialize sum */
    s = 0;

    /* answer is sum of three rolls times two and five rolls times four. */
    for( i=0; i<n; i++ ) {
        d = READ_DIE( c, i );
        s += (d-1)*(d%2);
    }

    /* return the answer */
    return s;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  play_petals
 *  Description:  Play a single round of the game "petals around the rose"
 * ============================================================================
 */
void
play_petals() {
    Cup c;
    
    /* roll all dice */
    c = rand();

    /* display dice for our users */
    render( c, NUM_DICE );

    /* wait for player to request the answer */
    printf("Press Enter to reveal answer ");
    while (fgetc(stdin) != '\n' && !feof(stdin)); 

    /* print the answer and pitch the big question */
    printf("The answer is: %d -- but why???\n\n", answer(c, NUM_DICE));
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  main
 *  Description:  Program startup
 * ============================================================================
 */
int
main( int argc, char *argv[] ) {
    char q = ROLL;

    /* initialize the random number generator */
    srand(time(NULL));
    print_rules();

    /* wait for user to start the game */
    printf("Press Enter to begin");
    while (fgetc(stdin) != '\n' && !feof(stdin)); 

    /* play until user quits */
    while( q == ROLL ) {
        /* play the game */
        play_petals();  
        
        /* give player a chance to go again */
        printf("Play again? y/n: ");
        q = fgetc(stdin);
        
        /* clear input buffer */
        while (fgetc(stdin) != '\n' && !feof(stdin));
    }

    /* all done */
    return EXIT_SUCCESS;
}
