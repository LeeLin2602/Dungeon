#pragma once
#include<ncurses.h>
#include<cmath>
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
	inline void draw_line(int x1, int x2, int y1, int y2, int db = 1){
		if(x1 > x2) swap(x1, x2);
		// if(y1 > y2) swap(y1, y2);
		double d = (double)(y2 - y1) / (x2 - x1);
		double y = y1;
		for(int i = x1; i <= x2; i++, y += d){
			if(d > 0) {if(db) mvprintw((int)floor(y), i, "/"); mvprintw((int)ceil(y), i, "\\");}
			else {if(db) mvprintw((int)floor(y), i, "\\"); mvprintw((int)ceil(y), i, "/");}
		}
	}
};