#pragma once
#include<string>
#include<map>
#include<set>
#include<obj.h>

using namespace std;

class warrior {
protected:
	map<string, int> val;
public:
	vector<props> bag;
	string name;
	warrior(string n, int h, int a, int d, int mn){ name = n, val["hp"] = h, val["atk"] = a, val["dfs"] = d, val["money"] = mn; }
	void add(string key, int v){ val[key] += v; }
	void mod(string key, int v){ val[key] = v; }
	int get(string key){ return val[key]; }
	int&getref(string key){ return val[key]; }

	friend class game;
};