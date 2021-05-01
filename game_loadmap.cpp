#include<game.h>

void game::initMap(){
    fstream gameMapFile, gameMonsterFile, gameItemFile, gameNPCFile; int n;
    gameNPCFile.open("./Data/npcs", ios::in), gameItemFile.open("./Data/items", ios::in), gameMonsterFile.open("./Data/monsters", ios::in), gameMapFile.open("./Data/map", ios::in);
    
    if(!gameNPCFile || !gameItemFile || !gameMonsterFile || !gameMapFile) throw gameError((gameErrorType)gameDataError);

    gameNPCFile >> n;
    for(int i = 0; i < n; i++){
        string name, script; int m; gameNPCFile >> name >> script >> m;
        vector<string> cmd(m); vector<int> nb; string pl; 
        for(int j = 0; j < m; j++){ int x; gameNPCFile >> x; nb.push_back(x); } getline(gameNPCFile, pl);
        for(int j = 0; j < m; j++) getline(gameNPCFile, cmd[j]);
        gameNPC.push_back(new npc(name, script, cmd, nb));
        gameNPC[i]->image = readFile("./Data/images/npcs/" + to_string(i + 1) + ".ansi", "./Data/images/npcs/0.ansi");
    }

    gameItemFile >> n;
    for(int i = 0; i < n; i++){
        string name; int ptp, m; gameItemFile >> name >> ptp >> m;
        vector<string> cmd(m); string pl; getline(gameItemFile, pl);
        for(int j = 0; j < m; j++) getline(gameItemFile, cmd[j]);
        gameProps.push_back(new props(name, cmd, ptp, i));
        gameProps[i]->image = readFile("./Data/images/items/" + to_string(i + 1) + ".ansi", "./Data/images/items/0.ansi");
    }

    gameMonsterFile >> n;
    for(int i = 0; i < n; i++){
        string n; int h, a, d, r; gameMonsterFile >> n >> h >> a >> d >> r;
        gameMonsters.push_back(new monster(n, h, a, d, r));
        gameMonsters[i]->image = readFile("./Data/images/" + to_string(i + 1) + ".ansi", "./Data/images/0.ansi");
    }

    gameMapFile >> n;
    gameMap.resize(n);
    cur = &gameMap[0];
    for(int i = 0; i < n; i++){
        string n; int u, d, l, r, tp;
        gameMapFile >> n >> u >> d >> l >> r >> tp;
        u --, r --, d --, l --, gameMap[i].name = n, gameMap[i].index = i;

        if(u >= 0) gameMap[i].adj[U] = &gameMap[u], gameMap[u].adj[D] = &gameMap[i];
        if(d >= 0) gameMap[i].adj[D] = &gameMap[d], gameMap[d].adj[U] = &gameMap[i];
        if(l >= 0) gameMap[i].adj[L] = &gameMap[l], gameMap[l].adj[R] = &gameMap[i];
        if(r >= 0) gameMap[i].adj[R] = &gameMap[r], gameMap[r].adj[L] = &gameMap[i];

        if(tp == 1){
            int mtp; gameMapFile >> mtp;
            gameMap[i].obj = new monster();
            memcpy(gameMap[i].obj, gameMonsters[mtp - 1],  sizeof(monster));
        } else if(tp == 2){
            int itp; gameMapFile >> itp;
            gameMap[i].obj = new props();
            memcpy(gameMap[i].obj, gameProps[itp - 1], sizeof(props));
        } else if(tp == 3){
            int ntp; gameMapFile >> ntp;
            gameMap[i].obj = new npc();
            memcpy(gameMap[i].obj, gameNPC[ntp - 1], sizeof(npc));
        }
    }

    gameMapFile.close(), gameMonsterFile.close(), gameItemFile.close(), gameNPCFile.close();
}