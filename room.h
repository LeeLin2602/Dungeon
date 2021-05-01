#pragma once
#include<string>
#include<obj.h>

#define U 0
#define D 1
#define L 2
#define R 3

using namespace std;

class room {
public:
	string name;
	int index;
	room* adj[4];
	object* obj;
	room() {
		obj = nullptr;
		for(int i = 0; i < 4; i++) adj[i] = nullptr;
	}
};