#pragma once
#include<ncurses.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<sys/ioctl.h>

#include<queue>
#include<fstream>
#include<sstream>
#include<string>
#include<utility>
#include<exception>

#include<room.h>
#include<obj.h>
#include<stdlib.h>
#include<warrior.h>

using namespace std;

typedef pair<int, int> pii;

enum gameErrorType{
    kbdtError = 1,
    ncursesError = 2,
    gameDataError = 3
};

struct gameError: public exception {
    gameErrorType errorType;
    gameError(gameErrorType e): errorType(e){}
};

void* keyboard_detect(void *events);
void clear_event(queue<pii> &events);

class game {
private:
    pthread_t keyboard_detector;
    int scr_row, scr_col;
    string message;
    vector<room> gameMap;
    vector<monster*> gameMonsters;
    vector<props*> gameProps;
    vector<npc*> gameNPC;
public:
    queue<pii> events;
    room* cur;
    string logs;
    warrior* player;
    game(int r, int c);
    void init();
    void draw(int screen);
    void viewBag(int cur);
    void quit();
    void pause();
    string save();
    void read(string input);
    void show_info(vector<string> content);
    void print_prompt(string x);
private:
    void clear_event(queue<pii> &events);
    void hello();
    void initMap();
};
