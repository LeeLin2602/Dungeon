#pragma once
#include<string>
#include<vector>
#include<sstream>

using namespace std;

inline int Hash(string x){
	int p = 1e8 + 7, q = 67, ans = 0;
	for(char y: x) ans = ((long long)ans * q + y - 'A') % p;
	return ans;
}

class object {
public:
	string name;
	vector<string> image;
	int valid, type;
	virtual void trigger(void *Game) = 0;
	virtual vector<string> render();
	virtual string save();
	virtual void read(string input);
};

class monster: public object {
public:
	int hp, atk, dfs, reward, script;
	monster(){}
	monster(string n, int h, int a, int d, int r, int sc): hp(h), atk(a), dfs(d), reward(r), script(sc) { name = n, type = 1, valid = 1;}
	void trigger(void *Game);
	vector<string> render();
private:
	int calculateDamage(int pa, int pd);
};

class props: public object {
private:
	vector<string> commands;
public:
	int prop_type, index;
	props(){}
	props(string n, vector<string> cmd, int pt, int idx){ name = n, commands = cmd, valid = 1, type = 2, prop_type = pt, index = idx;}
	void trigger(void *Game);
	void use(void *Game);
};

class npc: public object { 
public:
	string script;
	vector<string> commodity;
	vector<int> number;
	npc(){}
	npc(string n, string sc, vector<string> cm, vector<int> nb){ name = n, script = sc, commodity = cm, number = nb, valid = 1, type = 3;}
	vector<string> render();
	void trigger(void *Game);
	string save();
	void read(string input);
};