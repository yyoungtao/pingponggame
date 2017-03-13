#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include "bounce.h"
#include <stdlib.h>
#include <pthread.h>

struct ppball the_ball;
struct sppbtp the_sppbtp;

void set_up();
void serve();
void client_serve();
void wrap_up();
void ball_move(int);
//char horizonal[] = "-------------------------------------------------------------";
char horizonal[] = "*************************************************************";
char vertical[] = ".";
char net[] = "+";
int balls_left = 3;
int sock_cli;
char s_score = '0';
char c_score = '0';
char* msg;
int main()
{
    int c;

	set_up();
	serve();
	sock_cli = initClient();

	while(balls_left >= 0 && (c = getchar()) != 'Q')
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
	//init display info
	move(22, 9);
	addstr("Me:");
	move(22, 14);
	addstr("0");
	move(22, 17);
	addstr("Server:");
	move(22, 26);
	addstr("0");
	move(22, 29);
	addstr("Waiting for server...");
	paddle_init(CLIENT_FLAG);
	refresh();
}

void serve()
{
//    void ball_move(int);
	 
     /* Random values for initial position and speed */
     //int y_pos_r = ( rand() % (19 - 6 + 1) + 6 );
    // int y_pos_r = get_rand(6, 19);
    // int x_pos_r = get_rand(31, 58);
  
    // int y_ttm_r = get_rand(4, 16);
    // int x_ttm_r = get_rand(4, 16);
    balls_left--;		   
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

void client_serve()
{
   /* the_ball.y_pos = pb.y_pos;
    the_ball.x_pos = pb.x_pos;
    the_ball.y_ttg = the_ball.y_ttm = pb.y_ttm;
    the_ball.x_ttg = the_ball.x_ttm = pb.x_ttm;
    the_ball.y_dir = pb.y_dir;
    the_ball.x_dir = pb.x_dir;
    the_ball.symbol = DFL_SYMBOL;*/
    the_ball.x_pos = 10;
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
		mvaddch(MSG_Y, ME_X, c_score);
		mvaddch(MSG_Y, OPPO_X, s_score);
		move(MSG_Y, MSG_X);
		addstr(msg);
		//touch net connect to client send ball
		if(the_ball.x_pos == LEFT_EDGE)
		{
            mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
			//send ball
			the_sppbtp.info_type = DEFAULT;
            the_sppbtp.trans_ball   = the_ball;
            the_sppbtp.balls_left   = balls_left;
            the_sppbtp.server_score = s_score;
            the_sppbtp.client_score = c_score;
            the_sppbtp.message      = "Playing...";
			msg = "wait for server..........";
			move(MSG_Y, MSG_X);
            addstr(msg);
			refresh();
			sendball(sock_cli, &the_sppbtp);
			//receive ball
			receive(sock_cli, &the_sppbtp);
            the_ball = the_sppbtp.trans_ball;
            balls_left = the_sppbtp.balls_left;

            //check status
			switch(the_sppbtp.info_type)
            {
                case WIN  :
                c_score += 1;
			    msg = "you win, Press'Q' to exit";
			    close(sock_cli);
				break;
                case SCORE:
                c_score += 1;
				msg = "Playing...               ";
                serve();
				break;
                default:
				msg = "Playing...               ";
                client_serve();
				break;
            }
		}
		if(bounce_or_lose(&the_ball) == -1)
		{
		    if(balls_left > 0)
			{
		  	    mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
//				balls_left--;
                the_sppbtp.info_type    = SCORE;
                the_sppbtp.balls_left   = balls_left;
				s_score += 1;
                the_sppbtp.server_score = s_score;
                the_sppbtp.client_score = c_score;
                the_sppbtp.message      = "Playing...";
				msg = "waiting for server       ";
                move(MSG_Y, MSG_X);
				addstr(msg);
                               mvaddch(MSG_Y, OPPO_X, s_score);
				refresh();
                //send info
                sendball(sock_cli, &the_sppbtp);
                //receive info
                receive(sock_cli, &the_sppbtp);
                the_ball = the_sppbtp.trans_ball;
                balls_left = the_sppbtp.balls_left;
		     	//check status
                switch(the_sppbtp.info_type)
                {
                    case WIN  :
                    c_score += 1;
					msg = "you win, Press'Q' to exit";
					close(sock_cli);
					break;
                    case SCORE:
					c_score += 1;
					msg = "Playing...               ";
                    serve();
					break;
                    default:
                    msg = "Playing...               ";
                    client_serve();
					break;
                }
			}else{
			    mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
                the_sppbtp.info_type    = WIN;
                the_sppbtp.balls_left   = balls_left;
				s_score += 1;
                the_sppbtp.server_score = s_score;
                the_sppbtp.client_score = c_score;
                the_sppbtp.message      = "you win...Press'Q'to exit";
                if(s_score > c_score){
		    msg = "you lose, Press'Q'to exit";
		}else{
		    msg = "you win, Press'Q'to exit";
		}
				//send info
                sendball(sock_cli, &the_sppbtp);
				close(sock_cli);
			}
		}
		move(LINES-1, COLS-1);
		refresh();
	}
   	signal(SIGALRM, ball_move);
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

	if(bp->x_pos == RIGHT_EDGE)
    {
	    if(paddle_contact(bp->y_pos, bp->x_pos, CLIENT_FLAG) == 0)
		{
		    bp->x_dir = -1;
			the_ball.y_ttm = Y_TTM;
			the_ball.x_ttm = X_TTM;
			return_val = 1;
		}else{
//		    balls_left--;
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
