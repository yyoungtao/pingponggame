#define BLANK         ' '
#define DFL_SYMBOL    'O'
#define PAD_SYMBOL    '#'
#define TOP_ROW       5
#define BOT_ROW       20
#define LEFT_EDGE     10
#define RIGHT_EDGE    70
#define X_INIT        10
#define Y_INIT        10
#define TICKS_PER_SEC 50
#define CONNECT_SUCCESS 1
#define LOSE_CONNECT    2
#define SCORE           3
#define LOSE            4
#define WIN             5
#define DEFAULT         0
#define X_TTM 8
#define Y_TTM 8
#define ME_X  14
#define OPPO_X 26
#define MSG_X 29
#define MSG_Y 22

#define CLIENT_FLAG 0
#define SERVER_FLAG 1

/*the ping pong ball*/
struct ppball
{
    int y_pos, x_pos;
	int y_ttm, x_ttm;
	int y_ttg, x_ttg;
	int y_dir, x_dir;
	char symbol;
};
/*ping pong ball*/
struct paddle
{
   int pad_top;
   int pad_bot;
   int pad_col;
   char symbol;
};

struct sppbtp
{
    int info_type;
    char *message;
	char server_score;
	char client_score;
	int balls_left;
    struct ppball trans_ball;
};
