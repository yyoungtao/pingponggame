#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include "bounce.h"
#include <stdlib.h>


struct ppball the_ball;

void set_up();
void serve();
void wrap_up();

char horizonal[] = "-----------------------------------------------------------------------";
char vertical[] = "|";
char net[] = "+";
int balls_left = 3;

int main()
{
    int c;
	set_up();
	serve();

	while(balls_left > 0 && (c = getchar()) != 'Q')
	{
	    if(c == 'f') the_ball.x_ttm--;
   		else if(c == 's') the_ball.x_ttm++;
		else if(c == 'F') the_ball.x_ttm--;
		else if(c == 'S') the_ball.x_ttm++;
        else if(c == 'j') paddle_down();
		else if(c == 'k') paddle_up();
	}
	wrap_up();
}

void set_up()
{
	int i; 

	initscr();
    noecho();
    crmode();

    move(4, 9);
	addstr(horizonal);
	move(21, 9);
	addstr(horizonal);
    for(i=5 ; i<21 ; i++)
	{
	    move(i, 9);
		addstr(vertical);
	}
    
    paddle_init();
    refresh();
}

void serve()
{
    void ball_move(int);
	 
     /* Random values for initial position and speed */
     //int y_pos_r = ( rand() % (19 - 6 + 1) + 6 );
    // int y_pos_r = get_rand(6, 19);
    // int x_pos_r = get_rand(31, 58);
  
    // int y_ttm_r = get_rand(4, 16);
    // int x_ttm_r = get_rand(4, 16);
		   
     the_ball.y_pos = Y_INIT;
     the_ball.x_pos = X_INIT;
     the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
     the_ball.x_ttg = the_ball.x_ttm = X_TTM;
     the_ball.y_dir = 1  ;
     the_ball.x_dir = 1  ;
     the_ball.symbol = DFL_SYMBOL;
														        
     signal( SIGINT , SIG_IGN );
     mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol  );
     refresh();
														 
     signal( SIGALRM, ball_move );
     set_ticker( 1000 / TICKS_PER_SEC ); /* send millisecs per tick */
}

void wrap_up()
{
    set_ticker(0);
	endwin();
}

void ball_move(int signum)
{
    int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);
	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;
	
	if(the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1)
	{
	    the_ball.y_pos += the_ball.y_dir;/*move*/
		the_ball.y_ttg = the_ball.y_ttm; /*reset*/
		moved = 1;
	}

	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1 )
	{
	    the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}

	if(moved)
	{
	    mvaddch(y_cur, x_cur, BLANK);
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		if(bounce_or_lose(&the_ball) == -1)
		{
		    if(balls_left > 0)
			{
		  	    mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
				sleep(1);
		     	serve();
			}else{
			    move(20, 20);
                addstr("you lose!");
                move(21, 20);
                addstr("Press 'Q' to exit!");
			}
		}
		move(LINES-1, COLS-1);
		refresh();
	}
   	signal(SIGALRM, ball_move);
}

void paddle_move()
{
    
}

int bounce_or_lose(struct ppball *bp)
{
    int return_val = 0;
	
	if(bp->y_pos == TOP_ROW)
	{
	    bp->y_dir = 1;
		return_val = 1;
	}else if(bp->y_pos == BOT_ROW)
	{
	    bp->y_dir = -1;
		return_val = 1;
	}

	if(bp->x_pos == LEFT_EDGE)
	{
	    bp->x_dir = 1;
	    return_val = 1;
	}else if(bp->x_pos == RIGHT_EDGE)
    {
	    if(paddle_contact(bp->y_pos, bp->x_pos) == 0)
		{
		    bp->x_dir = -1;
			the_ball.y_ttm = Y_TTM;
			the_ball.x_ttm = X_TTM;
			return_val = 1;
		}else{
		    balls_left--;
			return_val = -1;
		}
    }
	return return_val;
}

int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
	n_sec = n_msecs/1000;/*seconds */
	n_usecs = (n_msecs % 1000)*1000L;/*milli second*/

	new_timeset.it_interval.tv_sec = n_sec;   /*set reload*/
	new_timeset.it_interval.tv_usec = n_usecs;/*new ticker value*/
	new_timeset.it_value.tv_sec = n_sec;      /*store this*/
	new_timeset.it_value.tv_usec = n_usecs;   /*and this*/

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
