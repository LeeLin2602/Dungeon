#pragma once
#include<string>
#include<obj.h>
#include<map>
#include<set>

using namespace std;

class warrior {

public:
	map<string, int> val;
	vector<props> bag;
	
	string name;

	warrior(string n, int h, int a, int d, int mn){
		name = n;
		val["hp"] = h;
		val["atk"] = a;
		val["dfs"] = d;
		val["money"] = mn;
	}

	void add(string key, int v){
		val[key] += v;
	}

	void mod(string key, int v){
		val[key] = v;
	}

	int get(string key){
		return val[key];
	}

};