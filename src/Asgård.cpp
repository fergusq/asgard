//============================================================================
// Name        : Asgård.cpp
// Author      : Iikka Hauhio
// Version     :
// Copyright   : (c) Iikka Hauhio
// Description : Hello World in C++, Ansi-style
//============================================================================

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "asgård.h"
#include "game.h"
#include "map.h"

bool running = true;

int main() {
	  initscr();
	  keypad(stdscr, TRUE);
	  raw();
	  noecho();
	  curs_set(0);
	  start_color();

	  if (can_change_color()) {
		  init_color(COLOR_BLACK, 0, 0, 0);
	  }

	  init_pair(CP_BLUE, COLOR_BLUE, COLOR_BLACK);
	  init_pair(CP_R, COLOR_BLACK, COLOR_WHITE);
	  init_pair(CP_R_YELLOW, COLOR_YELLOW, COLOR_WHITE);
	  init_pair(CP_RED, COLOR_RED, COLOR_BLACK);
	  init_pair(CP_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	  init_pair(CP_GREEN, COLOR_GREEN, COLOR_BLACK);
	  init_pair(CP_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	  init_pair(CP_CYAN, COLOR_CYAN, COLOR_BLACK);

	  int maxX, maxY;
	  const int minWindowSizeX = 32;
	  const int minWindowSizeY = 16;

	  getmaxyx(stdscr,maxY,maxX);
	  if(maxX < minWindowSizeX || maxY < minWindowSizeY) {
	    printw("Ikkunan minimikoko on %d x %d. Ikkunasi koko on %dx%d", minWindowSizeX, minWindowSizeY, maxX, maxY);
	    getch();
	    endwin();
	    return 0;
	  }

	  AsgardGame * game = new AsgardGame;

	  do {

		  game->updateGame();

	  } while (running);

	  endwin();

	  delete game; game = 0;
	  return 0;
}
