#include<ncurses.h>
#include<vector>
#include<utility>
#include<game.h>
#include<iostream>
#include<iomanip>

#define fst first
#define scd second

using namespace std;

typedef pair<int, int> pii;

int main() {

    game Game(38, 128);

    try {
        Game.init();
    } catch (const gameError& e){
        switch(e.errorType){
            case (gameErrorType)kbdtError:
                cout << "Unable to detect keyboard events" << endl;
                break;
            case (gameErrorType)ncursesError:
                cout << "Unable to initiate ncurses" << endl;
                break;
            case (gameErrorType)gameDataError:
                cout << "Unable to access files under Data" << endl;
                break;
        }
        exit(e.errorType);
    }

    Game.player->mod("last", -1);
    int INPUT_MODE = 0; // normal

    vector<string> Leaving = {"Save/Load/Quit:", "", "(S) Save", "(L) Load", "(Q) Quit", "(C) Continue"};
    vector<string> Quiting = {"Do you really want to leave?", "", "(Y) Leave", "(N) Cancel", "", ""};
    vector<string> Saving = {"Save to?", "", "", "", "", "(C) Cancel"};
    vector<string> Reading = {"Load from?", "", "", "", "", "(C) Cancel"};
    vector<string> ViewingBag = {"View backpack:", "", "(W) Scroll up", "(S) Scroll down", "(SPACE) Use/Equip/Unload", "(Q) Quit"};
    

    while(1){
        int &last = Game.player->val["last"];
        if(INPUT_MODE == 0){
            Game.draw(1);
            while(!Game.events.empty()){
                Game.print_prompt("");
                auto [typ, key] = Game.events.front(); Game.events.pop();
                // cout << Game.cur->obj << endl;
                if(Game.cur->obj == nullptr or !Game.cur->obj->valid or Game.cur->obj->type != 1){
                    if(typ == 0 and tolower(key) == 'w' and Game.cur->adj[U]) Game.cur = Game.cur->adj[U], last = D;
                    if(typ == 0 and tolower(key) == 's' and Game.cur->adj[D]) Game.cur = Game.cur->adj[D], last = U;
                    if(typ == 0 and tolower(key) == 'a' and Game.cur->adj[L]) Game.cur = Game.cur->adj[L], last = R;
                    if(typ == 0 and tolower(key) == 'd' and Game.cur->adj[R]) Game.cur = Game.cur->adj[R], last = L;
                } else {
                    Game.print_prompt("There is monster in this room. You cannot pass by. Press (R) to retreat.");
                }
                if(typ == 0 and tolower(key) == 'e') INPUT_MODE = 5;
                if(typ == 0 and tolower(key) == 'r' and last != -1) Game.cur = Game.cur->adj[last], last = -1, Game.print_prompt("You returned to last room.");
                if(typ == 0 and key == ' ' and Game.cur->obj != nullptr and Game.cur->obj->valid) {
                    Game.cur->obj->trigger(&Game);
                } else if(typ == 0 and key == ' ') {
                    Game.print_prompt("Nothing to interactive here.");
                }
                if(typ == 0 and tolower(key) == 'q') INPUT_MODE = 1;
            }
            // std::stringstream ss; ss << Game.cur->obj;
            // string x; ss >> x; Game.print_prompt(x); 
            if(Game.player->get("hp") <= 0) Game.print_prompt("You are defeated! ");
        } else if(INPUT_MODE == 1){
            Game.show_info(Leaving); refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and tolower(key) == 's') INPUT_MODE = 3;
                if(typ == 0 and tolower(key) == 'l') INPUT_MODE = 4;
                if(typ == 0 and tolower(key) == 'q') INPUT_MODE = 2;
                if(typ == 0 and tolower(key) == 'c') {
                    INPUT_MODE = 0;
                    break;
                }
            }
        } else if(INPUT_MODE == 2){
            Game.show_info(Quiting); refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and tolower(key) == 'y') INPUT_MODE = 10;
                if(typ == 0 and tolower(key) == 'n') {
                    INPUT_MODE = 0;
                    break;
                }
            }
        } else if(INPUT_MODE == 3){
            if(Saving[1] == ""){
                for(int i = 1; i <= 4; i++){
                    fstream rec, add;
                    string res;
                    rec.open("./Data/saves/" + to_string(i) + "/time", ios::in);
                    if(!rec){
                        add.open("./Data/saves/" + to_string(i) + "/time", ios::out);
                        add.close();
                        res = "Empty Record";
                    } else {
                        getline(rec, res);
                        res = res!=""?res:"Empty Record";
                    }
                    Saving[i] = "(" + to_string(i) +  ") " + res;
                    rec.close();
                }
            }
            Game.show_info(Saving); refresh();
            Game.save();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and '1' <= key and key <= '4') {
                    fstream rec;
                    string path = "./Data/saves/";
                    path += (char)key;
                    string record = Game.save();
                    rec.open(path + "/rec", ios::out);
                    rec.write(record.c_str(), record.size());
                    rec.close();
                    
                    auto t = std::time(nullptr);
                    auto tm = *std::localtime(&t);

                    std::ostringstream oss;
                    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
                    auto nowtime = oss.str();

                    rec.open(path + "/time", ios::out);
                    rec.write(nowtime.c_str(), nowtime.size());
                    rec.close();
                    Saving[1] = "";
                    INPUT_MODE = 0;
                    Game.print_prompt("Save record successfully.");
                    break;
                }
                if(typ == 0 and tolower(key) == 'c') {
                    INPUT_MODE = 0;
                    Saving[1] = "";
                    break;
                }
            }
        } else if(INPUT_MODE == 4){
            if(Reading[1] == ""){
                for(int i = 1; i <= 4; i++){
                    fstream rec, add;
                    string res;
                    rec.open("./Data/saves/" + to_string(i) + "/time", ios::in);
                    if(!rec){
                        add.open("./Data/saves/" + to_string(i) + "/time", ios::out);
                        add.close();
                        res = "Empty Record";
                    } else {
                        getline(rec, res);
                        res = res!=""?res:"Empty Record";
                    }
                    Reading[i] = "(" + to_string(i) +  ") " + res;
                    rec.close();
                }
            }    
            Game.show_info(Reading); refresh();
            Game.save();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and '1' <= key and key <= '4') {
                    fstream rec;
                    string path = "./Data/saves/";
                    path += (char)key;
                    path += "/rec";
                    string record = Game.save();
                    rec.open(path, ios::in);
                    string res, perLine;
                    if(!rec){
                        Game.print_prompt("Unable to Load record.");
                        INPUT_MODE = 0;
                        Reading[1] = "";
                        break;
                    } else {
                        while(getline(rec, perLine)) res += perLine + "\n";
                        Game.read(res);
                        Game.print_prompt("Load record successfully");
                        rec.close();
                    }
                    Reading[1] = "";
                    INPUT_MODE = 0;
                    break;
                }
                if(typ == 0 and tolower(key) == 'c') {
                    INPUT_MODE = 0;
                    Reading[1] = "";
                    break;
                }
            }        
        } else if(INPUT_MODE == 5){
            static int cur = 0;
            // Game.draw(0);
            Game.viewBag(cur);
            Game.show_info(ViewingBag); refresh();

            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                // Game.print_prompt(to_string((int)Game.player->bag.size()) + " " + to_string(cur) + " " + (char)key);
                if(typ == 0 and tolower(key) == 'w') cur = max(cur - 1, 0);
                else if(typ == 0 and tolower(key) == 's') cur = min(cur + 1, (int)Game.player->bag.size() - 1);
                else if(typ == 0 and key == ' ' and Game.player->bag.size() != 0) {
                    Game.player->bag[cur].use(&Game);
                    if(!Game.player->bag[cur].valid) Game.player->bag.erase(Game.player->bag.begin() + cur);
                } else if(typ == 0 and tolower(key) == 'q') {
                    INPUT_MODE = 0;
                    break;
                } else beep(); 
            }
        } else if(INPUT_MODE == 10){
            break;
        }
        
        usleep(50);
    }
    // erase();

    // printw("%d", Game.logs.size());
    Game.quit();
}

