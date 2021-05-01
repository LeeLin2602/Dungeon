#include<game.h>

void game::initMap(){

    fstream gameMapFile, gameMonsterFile, gameItemFile, gameNPCFile;

    gameNPCFile.open("./Data/npcs", ios::in);
    if(!gameNPCFile) {
        throw gameError((gameErrorType)gameDataError);
    } else {
        int n; gameNPCFile >> n;
        for(int i = 0; i < n; i++){
            string name, script; int m; gameNPCFile >> name >> script >> m;
            vector<string> cmd; vector<int> nb; string pl; 
            for(int j = 0; j < m; j++){
                int x; gameNPCFile >> x;
                nb.push_back(x);
            }

            for(int j = 0; j < m; j++){
                string a, b, c, d, e, f; gameNPCFile >> a >> b >> c >> d >> e >> f;
                pl = a + " " + b + " " + c + " " + d + " " + e + " " + f;
                cmd.push_back(pl);
            } 

            gameNPC.push_back(new npc(name, script, cmd, nb));

            fstream readImage; string perLine;
            readImage.open("./Data/images/npcs/" + to_string(i + 1) + ".ansi", ios::in);
            if(!readImage) readImage.open("./Data/images/npcs/0.ansi");
            while(getline(readImage, perLine)) gameNPC[i]->image.push_back(perLine);
            readImage.close();
        }
    }

    gameItemFile.open("./Data/items", ios::in);

    if(!gameItemFile) {
        throw gameError((gameErrorType)gameDataError);
    } else {
        int n; gameItemFile >> n;
        for(int i = 0; i < n; i++){
            string name; int ptp, m; gameItemFile >> name >> ptp >> m;
            vector<string> cmd; string pl; getline(gameItemFile, pl);
            for(int j = 0; j < m; j++){
                getline(gameItemFile, pl);
                cmd.push_back(pl);
            } 
            gameProps.push_back(new props(name, cmd, ptp, i));

            fstream readImage; string perLine;
            readImage.open("./Data/images/items/" + to_string(i + 1) + ".ansi", ios::in);
            if(!readImage) readImage.open("./Data/images/items/0.ansi");
            while(getline(readImage, perLine)) beep(), gameProps[i]->image.push_back(perLine);
            readImage.close();
        }
    }

    gameItemFile.close();

    gameMonsterFile.open("./Data/monsters", ios::in);

    if(!gameMonsterFile) {
        throw gameError((gameErrorType)gameDataError);
    } else {
        int n; gameMonsterFile >> n;

        for(int i = 0; i < n; i++){
            string n; int h, a, d, r; gameMonsterFile >> n >> h >> a >> d >> r;
            gameMonsters.push_back(new monster(n, h, a, d, r));

            fstream readImage; string perLine;
            readImage.open("./Data/images/" + to_string(i + 1) + ".ansi", ios::in);
            if(!readImage) readImage.open("./Data/images/0.ansi");
            while(getline(readImage, perLine)) gameMonsters[i]->image.push_back(perLine);
            readImage.close();
        }

    }
    gameMonsterFile.close();

    gameMapFile.open("./Data/map", ios::in);
    if(!gameMapFile) {
        throw gameError((gameErrorType)gameDataError);
    } else {
        int n; gameMapFile >> n;
        gameMap.resize(n);
        cur = &gameMap[0];
        for(int i = 0; i < n; i++){
            string n; int u, d, l, r, tp;
            gameMapFile >> n >> u >> d >> l >> r >> tp;
            u --, r --, d --, l --;
            gameMap[i].name = n;
            gameMap[i].index = i;

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
    }
    gameMonsterFile.close();

}