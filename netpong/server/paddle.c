#include "bounce.h"
#include <curses.h>

struct paddle the_paddle;

int i, j;

/*init the paddle*/
void paddle_init(int s_or_c)
{
    the_paddle.symbol = PAD_SYMBOL;
    //server paddle init
	if(s_or_c == SERVER_FLAG)
	{
	    the_paddle.pad_col = 9;
	    the_paddle.pad_top = 10;                                            
	    the_paddle.pad_bot = 15;
	}
	//client paddle init
	if(s_or_c == CLIENT_FLAG)
	{
	    the_paddle.pad_col = 71;
	    the_paddle.pad_top = 10;
	    the_paddle.pad_bot = 15;
	}
	i = the_paddle.pad_top;
	for(i ; i<= the_paddle.pad_bot ; i++)
	{
	    mvaddch(i, the_paddle.pad_col, the_paddle.symbol);
	}
}

void paddle_up()
{
    if(the_paddle.pad_top >= 6)
	{
	    mvaddch(the_paddle.pad_bot, the_paddle.pad_col, BLANK);
		the_paddle.pad_top--;
		the_paddle.pad_bot--;
		i = the_paddle.pad_top;
		for(i ; i<=the_paddle.pad_bot ; i++)
		{
		    mvaddch(i, the_paddle.pad_col, the_paddle.symbol);
		}
	}
}

void paddle_down()
{
    if(the_paddle.pad_bot < 20)
	{
	    mvaddch(the_paddle.pad_top, the_paddle.pad_col, BLANK);
		the_paddle.pad_top++;
		the_paddle.pad_bot++;
		i = the_paddle.pad_top;
		for(i ; i<=the_paddle.pad_bot ; i++)
		{
		    mvaddch(i, the_paddle.pad_col, the_paddle.symbol);
		}
	}
}

int paddle_contact(y, x, s_or_c)
{
    if(x == 70 && s_or_c == CLIENT_FLAG)
	{
	    j = the_paddle.pad_top;
		for(j ; j<=the_paddle.pad_bot ; j++)
		{
		    if(y == j)
			{
			    return 0;
			}
		}
	}else if(x == 10 && s_or_c == SERVER_FLAG)
	{
	    j = the_paddle.pad_top;
        for(j ; j<=the_paddle.pad_bot ; j++)
        {
            if(y == j)
            {
                return 0;
            }
        }

	}
	return -1;
}
