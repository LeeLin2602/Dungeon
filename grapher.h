#pragma once
#include<ncurses.h>

class grapher {
public:
	void init(){
		initscr();
        curs_set(0);
        cbreak();
        nonl();
        intrflush(stdscr,FALSE);
        keypad(stdscr,TRUE);
        scrollok(stdscr, FALSE);
        noecho();
	}
	inline void print_string(int i, int j, const char *y, string &x, const char *z = "", int _spc = 1){
	    mvprintw(i, j, y);
	    for(int k = 0; k < x.length(); k++) mvprintw(i, j + strlen(y) + k, "%c", ((x[k]=='_')&&_spc)?' ':x[k]);
	    mvprintw(i, j + strlen(y) + x.length(), z);
	}
	inline void print_vector(int i, int j, vector<string> lines, int _spc = 0){
		for(string line: lines) print_string(i++, j, "", line, "", _spc);
	}
};