# pingponggame
C/S ping pong game

1. The architecture of the software
The basic video pong game contains three parts, i.e. ball, court and paddle. For its upgraded version we need to add socket technique in order to achieve communication between two players (server and client). Here we first introduce the basic three parts of this net pong game, then describe how to achieve the different player play the game with each other via socket programming. This project contains screen management, signal and timer management, and the socket communication management techniques.
There are four basic elements of the court, i.e. the paddle, boundary, ball, and display information. While since this net pong game is designed for two players, so it has two separated parts, the left half of the court (e.g. controlled by server) and the right half of the court (e.g. controlled by client), however the techniques are almost the same.
 
The figure is the flow chart of the program, and the following are the detail describe of different functions in this program.
COURT:
First we use curses to draw the boundary and net, and ball to move it among the boundary. The important functions are set_up(), initscr(), noecho(), move(), addstr() and refresh(), as shown in Figure 3. However the display information will be changed based on the progress of the game, this will be detailed descript in the socket part.
PADDLE:
For the paddle part, we generate the paddle for both server and client based on the PAD_SYMBOL. We first define a paddle structure, by a top y-coordinate, a bottom y-coordinate and an x-coordinate. The x-coordinate is fixed. The top and bottom y-coordinates are controlled by user input.
During this part, we need to focus on how paddle move on the boundary and how it contact with ball, thus we designed three function to programming three different situation, the paddle_init()function check if the paddle has reached the limits of its path, and if not, to adjust the data structure and then adjust the screen representation of the paddle. The paddle_contact () function is not only has (x,y) position of the ball but also have the status of server or client. This function tells if the ball at the given position of given server or client is in contact with the paddle. The calling function can then bounce the ball appropriately. These functions about paddle control are contained in ‘paddle.c’ file.
BALL:
Then there comes the ball part, the ball is defined in a structure, including its position and speed, moving directory, the ball move bouncing off the walls and the paddles. When the ball reaches the net, it vanishes from that court and appears through the net on the other player's screen. That player needs to bounce it back. If that player misses the ball, the opponent hears the news and gets one score. One round has three opportunities, i.e. two out of three strategies.

With the help of socket technique, the two players can be anywhere on the Internet, each playing half the game against an unseen opponent. This net-pong game is the upgrade version of the basic pong game (i.e. console pong game). There are totally two main parts of this program, server part and client part.
SERVER
 (1) set_up() to initialize display
	a. Using curses to draw the boundaries of table
	b. Initialize the display information, the score and message display.
	c. Initialize the paddle use the paddle_init(symbol) method in “paddle.c”. Initialize the paddle based on the symbol of server or client, the different is the ball_index. And then move the paddle up and down.
(2) Initialize the server – – new_serve_socket = initServe() in “bserver.c” file
	a. ask kernel for a socket and set the timeout of sever
	b. bind address to socket. (host & port)
	c. listen and allow incoming calls write on socket
(3) Receive message – – receive()
	receive message from client
	(receive() in “bserver.c” file)
(4) Check status----check the status of the message received from client
  A. SCORE:msg=”playing......” means there is a decision situation so the ball-1; then re-serve() a new ball.
For the serve() method – – the score obtained by the server part so the resend ball is from the server part.
  B. default: nothing happened, ball bounce off and play all the time. Then server_serve(); the ball receive from the  client and move in server court the same as the client without missing.
  C. ball_move()--using curses to control the moving of ball to make sure the ball is moving correctly. However, we should pay attention to send and receive ball on the net part. For the net part we should pay attention to the index.
Use bounce_or_lose() and some indicates to determine the direction and the move of the ball.
  D. set_ticket()-- use the timeval to control the move of ball, set the timeval use the system method setitimer().
  Sendball() in the bserver.c ––  send message to client; copy information to client.
  Three statues of the ball: WIN/LOSE—end; SCORE—score and serve(); default—playing all the time.
(5) pthread_create() and pthread_join()	
Used to process the input with function input_thread() and to process the input from the keyboard.
CLIENT:
(1) set_up();
     Basicly the same as the server. Initialize and draw the table and display information.
    Then initialize paddle with paddle_init(CLIENT_FLAG);
(2) serve()
     Initialize the ball, and make it moving on the screen. Use signal control ball_move().
     ball_move(): restrict the position with boundaries; need to make choice when the ball touch net connect to sever and when the ball comes from sever it will receive ball.
     Check status: WIN/LOSE--break; SCORE--serve(); default—client_serve();
     bounce_or_loose()--check the situation to move to the next round or have winner to quit.
(3) initClient(); – initialize the client part
	a. get a socket
	b. connect to server – some information about server
(4) receive message from server – int receive()
(5) send info to server – int sendball() method
(6) close_socket() -- close socket to stop the communication between server and client.

2. Help file of the game

Step1: Log into ubuntu system,use ctrl+alt+t open two terminals.
Step2: Use “cd ...” command direct to the server folder and direct to the client folder.
Step3: In the server folder run the “make” command,generate the server run file “bounce2ds”, in the client folder do the same things, generate “bounce2dc”. After generate, use “make clean” command to clean the template files generate during the “make” command.
Step4: Run server use “./bounce2ds” command, run client use “./bounce2dc” command.
Step5: Start play the game, the ball will start on client end, and transfer to the server end. Use 'j/J' and 'k/K' to move the paddle up and down.
Step6: There are three balls, when 3 balls over, game over, the winner wins, both stop game, press 'Q' to exit game.

3. The future improvement

When the ball is in other side, the paddle can't be moved. For I'm not use the select() or the poll() system call to allow program to watch for input from the keyboard as well as from the socket. In the future, I can try to use the select() or the poll() system call to allow the user to move the paddle even when the ball is in the other court.

