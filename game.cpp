#include<game.h>

void* keyboard_detect(void *events){
    while(1)((queue<pii>*)events)->push(make_pair(0, (int)getchar()));
    pthread_exit(NULL);
}

vector<string> readFile(string path, string path2){
    fstream fileIO;
    fileIO.open(path, ios::in);
    if(!fileIO and path2.size()) fileIO.open(path2);
    vector<string> res; string pl;
    while(getline(fileIO, pl)) res.push_back(pl);
    return res;
}


void game::pause(){
    clear_event(events);
    while(events.empty()) usleep(50); 
    clear_event(events);
}

void game::init(){
    try { Grapher.init();} catch (...) { throw gameError((gameErrorType)ncursesError); }
    initMap();
    if(pthread_create(&keyboard_detector, NULL, keyboard_detect, &events)) throw gameError((gameErrorType)kbdtError);
    while(1){
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if(w.ws_row >= scr_row and w.ws_col >= scr_col) break;
        mvprintw(5,5,"Terminal size: (%d, %d)", w.ws_row, w.ws_col);
        mvprintw(6,5,"Minumum size: (%d, %d)", scr_row, scr_col);
        refresh(); usleep(50);
    }
    resizeterm(scr_row, scr_col);
    hello();
}

void game::hello(){
    erase(); Grapher.print_vector(1, 1, readFile("./Data/images/a.ansi"));
    mvprintw(4, 4, "My wife is cute <3"), refresh(), pause();
    clear_event(events), erase(), box(stdscr, '|', '-');
    vector<string> DungeonLOGO = readFile("./Data/images/dungeon.ansi");
    Grapher.print_vector(4, 24, DungeonLOGO);
    string name = "";
    while(1){
        mvprintw(20, 36, "Please tell me your name? ");
        if(!events.empty()){
            auto [typ, key] = events.front(); events.pop();
            if(typ == 0 and key == 127 and !name.empty()) name = name.substr(0, name.length() - 1);
            else if(!name.empty() and (key == 13 or key == 10)) break;
            else if(name.length() < 16 and ((key == ' ' or key == '_' or key == '.') or ('a' <= key and key <= 'z') or ('A' <= key and key <= 'Z'))) name += (char)key;
            else beep();
            for(int i = 0; i < name.length(); i++) mvprintw(20, 62 + i, "%c", name[i]);
        }
        for(int i = name.length(); i < 16; i++) mvprintw(20, 62 + i, "_");
        refresh(), usleep(50);
    }
    player = new warrior(name, 1000, 10, 10, 0); player->mod("last", -1);
    erase(); box(stdscr, '|', '-');
    Grapher.print_vector(4, 24, DungeonLOGO), Grapher.print_string(20,36, "Hello, ", name, ", "); 
    mvprintw(21,36, "Welcome to Dungeon!"), mvprintw(23,36, "(press any key to continue)");
    refresh(), pause();
}

void game::draw(int info, int prompt, int mainp, int doViewBag, int doRefresh){
    resizeterm(scr_row, scr_col);
    erase(), g_split(), g_status();
    if(prompt) g_prompt();
    if(info) g_info();
    if(mainp) g_main();
    if(!mainp and doViewBag >= 0) g_viewBag(doViewBag);
    Grapher.print_string(36, 2, "", message);
    if(doRefresh) refresh();
}

void game::print_prompt(string x){
    message = x;
}

void game::show_info(vector<string> content){
    Grapher.print_vector(14, 2, content, 1);
}

inline void game::clear_event(queue<pii> &events){
    while(!events.empty()) events.pop();
}


inline void game::g_split(){
    box(stdscr, '|', '-'), mvprintw(1, 3, "Dungeon Designed By cychen");
    for(int i = 1; i < scr_row - 3; i++) mvprintw(i, stc - 1, "|");
    for(int i = 1; i < stc - 1; i++) mvprintw(2, i, "-");
    for(int i = 1; i < stc - 1; i++) mvprintw(12, i, "-");
    for(int i = 1; i < stc - 1; i++) mvprintw(21, i, "-");
    for(int i = 1; i < stc - 1; i++) mvprintw(33, i, "-");
    for(int i = 1; i < scr_col - 1; i++) mvprintw(35, i, "-");
}

inline void game::g_status(){
    mvprintw(3, 2, "Status: "), Grapher.print_string(5, 6, "Player: ", player->name);
    mvprintw(7, 6, "HP : %d", player->get("hp")), mvprintw(8, 6, "ATK: %d", player->get("atk"));
    mvprintw(9, 6, "DFS: %d", player->get("dfs")), mvprintw(10, 6, "Money: %d", player->get("money"));
}

inline void game::g_prompt(){
    mvprintw(22, 2, "Prompt: "), mvprintw(24, 6, "(W, A, S, D) Move");
    mvprintw(25, 6, "(E) View backpack"), mvprintw(29, 6, "(Q) Save/Load/Quit");
    mvprintw(30, 6, "(R) Retreat"), mvprintw(31, 6, "(SPACE) Interactive");
    Grapher.print_string(34, 2, "Cur. Location: ", cur->name);
}

inline void game::g_info(){
    if(cur->obj != nullptr and cur->obj->valid) show_info(cur->obj->render());
    if(cur->obj == nullptr or !cur->obj->valid) mvprintw(14, 2, "Empty Room");
}

inline void game::g_main(){
    for(int i = 0; i <= 17; i++){
        for(int j = 1; j <= 5; j++) mvprintw(j, stc + i, "\\");
        for(int j = 28; j <= scr_row - 4; j++) mvprintw(j, stc + i, "\\");
    }
    for(int i = 0; i <= 15; i++){
        for(int j = 1; j <= 5; j++) mvprintw(j, scr_col - i - 2, "\\");
        for(int j = 28; j <= scr_row - 4; j++) mvprintw(j, scr_col - i - 2, "\\");
    }
    if(cur->adj[L]){
        for(int i = 0; i <= 17; i++) mvprintw(5, stc + i, "^"), mvprintw(27, stc + i, "v");
    } else {
        for(int i = 5; i <= 27; i++) {
            mvprintw(i, stc + 17, "|");
            for(int j = 0; j < 17; j++) mvprintw(i, stc + j, "\\");
        }
    }
    if(cur->adj[U]){
        for(int i = 1; i <= 4; i++) mvprintw(i, stc + 17, "|"), mvprintw(i, scr_col - 18, "|");
    } else {
        for(int i = stc + 18; i < 111; i++) {
            mvprintw(5, i, "^");
            for(int j = 1; j < 5; j++) mvprintw(j, i, "\\");
        }
    }
    if(cur->adj[R]){
        for(int i = 1; i <= 17; i++) mvprintw(5, scr_col - i - 1, "^"), mvprintw(27, scr_col - i - 1, "v");
    } else {
        for(int i = 5; i <= 27; i++) {
            mvprintw(i, scr_col - 18, "|");
            for(int j = 1; j < 17; j++) mvprintw(i, scr_col - j - 1, "\\");
        }
    }
    if(cur->adj[D]){
        for(int i = 3; i <= 9; i++) mvprintw(scr_row - i - 1, stc + 17, "|"), mvprintw(scr_row - i - 1, scr_col - 18, "|");
    } else {
        for(int i = stc + 18; i < 111; i++) {
            mvprintw(27, i, "v");
            for(int j = 2; j < 9; j++) mvprintw(scr_row - j - 2, i, "\\");
        }
    }
    if(cur->obj != nullptr and cur->obj->valid and cur->obj->image.size() > 0) Grapher.print_vector(6, stc + 21, cur->obj->image);
}

inline void game::g_viewBag(int cur){
    for(int i = cur - cur % 20; i < player->bag.size() and i < cur - cur % 20 + 20; i ++) Grapher.print_string(10 + i, 64, i == cur?"> ":"  ", player->bag[i].name, (player->get("eq" + to_string(player->bag[i].prop_type)) == Hash(player->bag[i].name))?" (Equiped)":"");
}



string game::save(){
    string result; result += player->name + "\n"; 
    result += to_string(player->val.size()) + "\n"; for(auto [k, v]: player->val) result += k + " " + to_string(v) + "\n";
    result += to_string(player->bag.size()) + "\n"; for(auto k: player->bag) result += to_string(k.index) + "\n";
    result += to_string(gameMap.size()) + "\n";     for(auto &r: gameMap) result += (r.obj)?(r.obj->save() + "\n"):"0\n";
    result += to_string(cur->index) + "\n";
    return result;
}

void game::load(string input){
    stringstream s(input); s >> player->name; int n; player->val.clear(), player->bag.clear(); s >> n;
    for(int i = 0; i < n; i++){
        string k; int v; s >> k >> v;
        player->val[k] = v;
    } s >> n;
    for(int i = 0; i < n; i++){
        int id; s >> id;
        props*item = new props();
        memcpy(item, gameProps[id], sizeof(props));
        player->bag.push_back(*item);
    } s >> n; string x; getline(s, x);
    for(int i = 0; i < n; i++){
        string x; getline(s, x);
        if(gameMap[i].obj) gameMap[i].obj->read(x);
    } s >> n;
    cur = &gameMap[n];
}

void game::quit(){
    system("stty cooked"), endwin(), exit(0);
}
