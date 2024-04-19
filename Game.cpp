#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Game.h"
#include <ctime> // for time()

using namespace std;



// set some attributes as default value

Game::Game(int _width, int _height)
    : width(_width), height(_height), // play screen
	  squares(_height, vector<CellType>(_width, CELL_EMPTY)), // cell coordinates
      snake(*this, Position(_width/2, _height/2)),  // init snake positin in middle of play screen
      currentDirection(Direction::RIGHT),
      status(GAME_RUNNING),
      score(0)
{
	// add new cherry in game initiation
	addCherry();
}

Game::~Game()
{
    //dtor
}



/***
 * When snake moves to a position,
 * if position belongs to BOARD or SNAKE body, status is GAME_OVER
 * if position is having CHERRY :
 * 			- score schoule be increased
 * 			- snake should eat cherry
 * 			- a new cherry should be randomly added
 * otherwise, this position should be assigned as cell of snake
 *
 * Args:
 * 		pos (Position): position where the snake will move
 *
 * Returns:
 * 		// none
 *
***/

void Game::snakeMoveTo(Position pos)
{
	getCellType(pos);
	if(getCellType(pos)==CELL_OFF_BOARD||getCellType(pos)==CELL_SNAKE)
    {
        status=GAME_OVER;
        return;
    }

    if(getCellType(pos)==CELL_CHERRY)
    {
        score=score+1;
        snake.eatCherry();
        setCellType(pos,CELL_SNAKE);
        do
        {
            srand(time(0));
            Position posrandom=(rand()%width,rand()%height);
            if(getCellType(posrandom)==CELL_EMPTY)
            {
                cherryPosition=posrandom;
                setCellType(posrandom,CELL_CHERRY);
                getCherryPosition();
                break;
            }

        }
        while(true);
    }
    else
    {
        setCellType(pos,CELL_SNAKE);
    }
}


/***
 * When all snake body leave a cell, set it as CELL_EMPTY
 * Args:
 * 		position (Position): position where snake will leave
 * Returns:
 * 		// none
 *
 ***/
void Game::snakeLeave(Position position)
{
	setCellType(position, CELL_EMPTY);
}



void Game::processUserInput(Direction direction)
{
    inputQueue.push(direction);
}


/***
 * check whether the snake can move to the intended direction with the currect direction.
 * If current direction is UP or DOWN, the next direction should not be UP or DOWN
 * if current diection is LEFT or RIGHT, the next direction should not be LEFT or RIGHT
 *
 * Args:
 * 		current (Direction): current direction of the snake
 * 		next (Direction): the intened direction that snake will move
 * Returns:
 * 		bool: whether the snake can ben changed the direction
 *
 ***/
bool Game::canChange(Direction current, Direction next) const {
	if (current == UP || current == DOWN)
		return (next!=UP&&next!=DOWN);
	return (next!=LEFT&&next!=RIGHT);
}


/***

 * Iterover the input queue from keyboard.
 * For each input direction, check whether snake can move or not.
 * If the snake can move,  set currentDiection as this input direction.
 * Otherwise, go to the next input direction from input queue
 *
 * Args:
 * 		// none
 * Returns:
 * 		// none
 *
 ***/

void Game::nextStep()
{
	while (!inputQueue.empty()) {
		// get the input direction from input queue
        Direction next= inputQueue.front() ;
        // remove the front of input queue
        inputQueue.pop();
		// check if snake can move to the next direction, set current direction as next
        if (canChange(currentDirection, next))
            {
        	currentDirection=next;
        	break;
            }
    }

    snake.move(currentDirection);
}


/***
 * When snake have already eaten a cherry, please add new cherry inside the play screen with random position
 *
 * Args:
 * 		// none
 * Returns:
 * 		// none
 *
 ***/

void Game::addCherry()
{
    srand(time(0));
    do {
		// init a random position inside the play screen (width, height)
		// Suggestion: use rand() function
        Position randompos(rand()%width,rand()%height);
		// check if the randomPos is EMPTY
        if (getCellType(randompos) == CELL_EMPTY) {

        	// assign the cherry position as randomPos, and set randomPos type as CELL_CHERRY
        	//getCherryPosition(randomPos);
            cherryPosition=randompos;
        	setCellType(randompos,CELL_CHERRY);
       		break;
        }
    } while (true);
}


/***
 * set cell of a position as intended type.
 *
 * Args:
 * 		pos (Position): a chosen position
 * 		cellType (CellType): cell type of pos
 * Returns:
 * 		// none
 *
 ***/
void Game::setCellType(Position pos, CellType cellType)
{
	// if position is inside the play screen (width, height), set to the cellType.
	if(getCellType(pos)==CELL_OFF_BOARD)
    {
        return;
    }
    else
    {

        squares[pos.y][pos.x]=cellType;
    }
	// Otherwise, do nothing
	// Suggestion: use pos.isInsideBox(...) in Position class
	//
}



CellType Game::getCellType(Position pos) const
{
	return pos.isInsideBox(0, 0, width, height) ? squares[pos.y][pos.x] : CELL_OFF_BOARD;
}

vector<Position> Game::getSnakePositions() const
{
    return snake.getPositions();
}


GameStatus Game::getGameStatus(){
	return status;
}


int Game::getWidth(){
	return width;
}


int Game::getHeight(){
	return height;
}


Snake Game::getSnake(){
	return snake;
}

