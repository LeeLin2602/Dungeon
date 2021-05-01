#pragma once

#include<ncurses.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/ioctl.h>

#include<queue>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<utility>
#include<exception>

#include<room.h>
#include<obj.h>
#include<warrior.h>
#include<grapher.h>

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

vector<string> readFile(string path, string path2 = "");

class game {
private:
    pthread_t keyboard_detector;
    int scr_row, scr_col;
    const int stc = 32;

    string message;
    grapher Grapher;
    vector<room> gameMap;
    vector<monster*> gameMonsters;
    vector<props*> gameProps;
    vector<npc*> gameNPC;
public:
    queue<pii> events;
    room* cur;
    warrior* player;

    game(int r, int c): scr_row(r), scr_col(c) {}

    // FLOW CONTROL
    void init();
    void quit();
    void pause();

    // RECORD
    string save();
    void load(string input);

    // GRAPHIC
    void draw(int info = 1, int prompt = 1, int main = 1, int doViewBag = -1, int doRefresh = 1);
    void print_prompt(string x);
    void show_info(vector<string> content);

private:

    inline void g_split();
    inline void g_status();
    inline void g_info();
    inline void g_prompt();
    inline void g_main();
    inline void g_viewBag(int cur);
    inline void clear_event(queue<pii> &events);
    void hello();
    void initMap();
};
