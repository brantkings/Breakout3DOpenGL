BLOCKOUT 3D, 
	by João Guilherme Reis Caldeira Brant

--- INTRO ---

Blackout is a very simple computer game. Basically the player controls a squared paddle playing 'Wall' using a ball that is reflected by all solids and walls (except the bootom one). When the ball touches (and it's reflect) by a part of the wall, that is eliminated, and the player goes on until the entire wall disappears. When that happens, another wall appears and the ball keeps it's velocity. The game ends when the player loses all his lives, which is lost when the ball reaches the bottom part.

In the end, the score is shown. Score is given by how many walls and parts of the walls were destroyed.

The game always begin on the pause mode. With the paddle in the center of it's movement line (the paddle only moves in one dimension) and the ball just above it.

For the development of this game, it was used OpenGL, GLUT for window management and the Brad Fish's GLFont library to render text. The library was changed in order to support blank spaces.

This was made as a project for the Computer Graphics course in UFMG, Brazil.

--- TO COMPILE AND PLAY ---

To compile and play just use the Make command in this folder, and then run the breakout executable that is produced.

Unhappily, this was tested only on Linux systems. 

In a Linux system:
	$make
	$./breakout

Please have the make program installed on the machine. In the other side, just compile all the .cpp files manually using g++ with the -lglut and -lGLU library directives, in this order.

--- CONTROLS ---

	Move the mouse cursor to move. As specified, when the mouse is on the right of the screen, the paddle moves right and vice-versa. The exact position of the collision influences in the direction of the ball.
	-Q key to exit the game.
	-R key to reset the game.
	-Left mouse key to pause/unpause.
	-Right mouse key pauses the game, and outputs all the game's present variables. Another right mouse click will show it for the next frame.

--- IMPLEMENTATION DECISIONS ---

	-> As explained before, the reflection on the hitted ball depends on the position of the collision.
	-> Walls or 'levels' are generated procedurally in a seed system in constant-sized matrices. First, generated a random number of seed bricks in the wall. Then, each seed in then expanded (creating 4 others seeds on it's right, left, up, and down corner, as the bricks are rectangular as a cell on the matrix), until another random final number of bricks is reached.
	-> There is a queue of walls that are all renderized on the same time. You can see the other walls on the queue further away (on a bigger z distance from the camera). When a wall is destroyed, the next in the queue takes it's place in a 3D effect, and another wall is generated in the end of the queue.
	-> When a wall is destroyed, a random congratulation text is displayed.
	-> The score system is as follows:
		-> There is a HUD of the score, at all times.
		-> Each brick has a value which is added to the score when it is destroyed.
		-> If you can destroy more that one brick without hitting the ball again, a combo begins. There is a counter for each brick destroyed in a combo. Then, the score of each brick is as follows:
			
			(Brick value) + 0.5 * (Brick Value) * (Combo counter)
			
		-> Obviously, when the ball hits the paddle, the counter goes back to 0.
		-> When a brick is destroyed, it's score is shown, just like old awesome arcade games.
		-> The bricks have special powers according to their color:
			YELLOW bricks are regular ones, they don't do anything.
			RED bricks make the ball's speed double until it is hit again.
			BLUE bricks score more points.
			GREEN bricks gives an extra life.
			BROWN bricks takes two hits to be destroyed.
			PURPLE bricks poisons the ball, which when it is hit by the paddle poisons it, which makes it four times slower than normal. It is cured when it hits the ball again.
			-> A non-YELLOW block is created when a seed is expanded, in the wall creation algorithm.
		-> When you lose all your lives, a Game Over label appears. You can restart the game if you press R.
		-> There is a HUD (Heads-up Display), which contains:
			- The score.
			- The remaining lives, represented by balls.
			- Two lines in the middle of the screen, helping the player with the mouse position to move the paddle.
		-> There is an animated 3D abstract background.
		-> The game is procedural and infinite. There is no end. You should just try to make the best score possible.
		-> Hit detection is made by rectangle overlay detection.
		-> The ball can be reflected on the X axis as well, if it hits a brick on it's side.

--- REFERENCES ---
GlFont by Brad Fish
brad.fish@gmail.com
