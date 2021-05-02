#include<iostream>
#include<iomanip>
#include<game.h>

#define fst first
#define scd second
using namespace std;

typedef pair<int, int> pii;
enum MODE{ NormalMode, LeavingMode, QuitingMode, SavingMode, LoadingMode, ViewingBagMode, BreakingMode };
vector<string>  Leaving = {"Save/Load/Quit:", "", "(S) Save", "(L) Load", "(Q) Quit", "(C) Continue"},
                Quiting = {"Do you really want to leave?", "", "(Y) Leave", "(N) Cancel", "", ""},
                Saving = {"Save to?", "", "", "", "", "(C) Cancel"}, Loading = {"Load from?", "", "", "", "", "(C) Cancel"},
                ViewingBag = {"View backpack:", "", "(W) Scroll up", "(S) Scroll down", "(SPACE) Use/Equip/Unload", "(Q) Quit"};

int main() {
    game Game(38, 128);
    try { Game.init(); } 
    catch (const gameError& e){
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

    MODE INPUT_MODE = NormalMode;

    while(1) { 
        int &last = Game.player->val["last"];
        if(INPUT_MODE == NormalMode){
            Game.draw(1, 1, 1);
            while(!Game.events.empty()){
                Game.print_prompt("");
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0){
                    if(Game.cur->obj == nullptr or !Game.cur->obj->valid or Game.cur->obj->type != 1){
                        if(tolower(key) == 'w' and Game.cur->adj[U]) Game.cur = Game.cur->adj[U], last = D;
                        if(tolower(key) == 's' and Game.cur->adj[D]) Game.cur = Game.cur->adj[D], last = U;
                        if(tolower(key) == 'a' and Game.cur->adj[L]) Game.cur = Game.cur->adj[L], last = R;
                        if(tolower(key) == 'd' and Game.cur->adj[R]) Game.cur = Game.cur->adj[R], last = L;
                    } else if(tolower(key) == 'd' or tolower(key) == 'w' or tolower(key) == 's' or tolower(key) == 'a') {
                        Game.print_prompt("There is monster in this room. You cannot pass by. Press (R) to retreat.");
                    }
                    if(key == ' '){
                        if(Game.cur->obj != nullptr and Game.cur->obj->valid) Game.cur->obj->trigger(&Game);
                        else Game.print_prompt("Nothing to interactive here.");
                    }
                    if(tolower(key) == 'q') INPUT_MODE = LeavingMode;
                    if(tolower(key) == 'e') INPUT_MODE = ViewingBagMode;
                    if(tolower(key) == 'r' and last != -1) Game.cur = Game.cur->adj[last], last = -1, Game.print_prompt("You returned to last room.");
                }
            }
            if(Game.player->get("isWin") == 1) Game.print_prompt("Congruatulation! You win. (Press any key to quit)"), INPUT_MODE = BreakingMode;
            if(Game.player->get("hp") <= 0) Game.print_prompt("You are defeated! (Press any key to quit)"), INPUT_MODE = BreakingMode;
        } else if(INPUT_MODE == LeavingMode){
            Game.draw(0, 1, 1, -1, 0); Game.show_info(Leaving); refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(tolower(key) == 's') INPUT_MODE = SavingMode;
                if(tolower(key) == 'l') INPUT_MODE = LoadingMode;
                if(tolower(key) == 'q') INPUT_MODE = QuitingMode;
                if(tolower(key) == 'c') INPUT_MODE = NormalMode;
            }
        } else if(INPUT_MODE == QuitingMode){
            Game.draw(0, 1, 1, -1, 0); Game.show_info(Quiting); refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and tolower(key) == 'y') INPUT_MODE = BreakingMode;
                if(typ == 0 and tolower(key) == 'n') INPUT_MODE = NormalMode;
            }
        } else if(INPUT_MODE == SavingMode){
            if(Saving[1] == ""){
                for(int i = 1; i <= 4; i++){
                    fstream rec; string res;
                    rec.open("./Data/saves/" + to_string(i) + "/time", ios::in);
                    if(!rec) res = "Empty Record";
                    else { getline(rec, res); res = res!=""?res:"Empty Record"; }
                    Saving[i] = "(" + to_string(i) +  ") " + res;
                    rec.close();
                }
            }
            Game.draw(0, 1, 1, -1, 0); Game.show_info(Saving); refresh();

            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and '1' <= key and key <= '4') {
                    fstream rec;
                    string path = "./Data/saves/"; path += (char)key;
                    string record = Game.save();
                    rec.open(path + "/rec", ios::out);
                    rec.write(record.c_str(), record.size());
                    rec.close();
                    
                    auto t = std::time(nullptr); auto tm = *localtime(&t);
                    ostringstream oss; oss << put_time(&tm, "%Y-%m-%d %H:%M");
                    auto nowtime = oss.str();

                    rec.open(path + "/time", ios::out);
                    rec.write(nowtime.c_str(), nowtime.size());
                    rec.close();
                    Saving[1] = "", INPUT_MODE = NormalMode;
                    Game.print_prompt("Save record successfully.");
                    break;
                }
                if(typ == 0 and tolower(key) == 'c') INPUT_MODE = NormalMode, Saving[1] = "";
            }
        } else if(INPUT_MODE == LoadingMode){
            if(Loading[1] == ""){
                for(int i = 1; i <= 4; i++){
                    fstream rec;
                    string res;
                    rec.open("./Data/saves/" + to_string(i) + "/time", ios::in);
                    if(!rec)res = "Empty Record";
                    else { getline(rec, res);res = res!=""?res:"Empty Record";}
                    Loading[i] = "(" + to_string(i) +  ") " + res;
                    rec.close();
                }
            }    
            Game.draw(0, 1, 1, -1, 0), Game.show_info(Loading); refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0 and '1' <= key and key <= '4') {
                    fstream rec;
                    string path = "./Data/saves/"; path += (char)key; path += "/rec";
                    string record = Game.save();
                    rec.open(path, ios::in);
                    string res, perLine;
                    if(!rec){
                        Game.print_prompt("Unable to Load record.");
                        INPUT_MODE = NormalMode, Loading[1] = "";
                        break;
                    } else {
                        while(getline(rec, perLine)) res += perLine + "\n";
                        Game.load(res), Game.print_prompt("Load record successfully");
                        rec.close();
                    }
                    Loading[1] = "", INPUT_MODE = NormalMode;
                    break;
                }
                if(typ == 0 and tolower(key) == 'c') INPUT_MODE = NormalMode, Loading[1] = "";
            }        
        } else if(INPUT_MODE == ViewingBagMode){
            static int cur = 0;
            Game.draw(0, 1, 0, cur, 0), Game.show_info(ViewingBag);
            refresh();
            while(!Game.events.empty()){
                auto [typ, key] = Game.events.front(); Game.events.pop();
                if(typ == 0){
                         if(tolower(key) == 'w') cur = max(cur - 1, 0);
                    else if(tolower(key) == 's') cur = min(cur + 1, (int)Game.player->bag.size() - 1);
                    else if(tolower(key) == 'q') INPUT_MODE = NormalMode;
                    else if(key == ' ' and Game.player->bag.size() != 0) { Game.player->bag[cur].use(&Game); if(!Game.player->bag[cur].valid) Game.player->bag.erase(Game.player->bag.begin() + cur); cur = min(cur, (int)Game.player->bag.size() - 1);}
                    else beep(); 
                }
            }
        } else if(INPUT_MODE == BreakingMode) break;
        usleep(50000);
    }
    Game.draw(1, 1, 1);
    Game.pause();
    Game.quit();
}

