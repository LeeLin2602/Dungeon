#include<obj.h>
#include<game.h>

string object::save(){
	string result = name + " " + to_string(valid) + " " + to_string(type);
	return result;
}

void object::read(string input){
	stringstream s(input);
	s >> name >> valid >> type;
}

vector<string> object::render(){
	vector<string> res;
	res.push_back(name);
	// res.push_back(to_string(image.size()));
	return res;
}

int monster::calculateDamage(int pa, int pd){
	int pdamage = max(pa - dfs, 1);
	int mdamage = max(atk - pd, 1);
	return (hp / pdamage) * mdamage;
}

void monster::trigger(void *Game = nullptr){
	if(((game*)Game)->cur->obj->valid == 0) return;
	int damage = calculateDamage(((game*)Game)->player->get("atk"), ((game*)Game)->player->get("dfs"));
	((game*)Game)->player->add("hp", -damage);
	((game*)Game)->player->add("money", reward);
	((game*)Game)->cur->obj->valid = 0;
	((game*)Game)->print_prompt("You defeated " + name + "! You lose " + to_string(damage) + " HP. You got " + to_string(reward) + " dollars.");
}

vector<string> monster::render(){
	vector<string> res;
	res.push_back("Monster: " + name);
	res.push_back("");
	res.push_back("HP: " + to_string(hp));
	res.push_back("ATK: " + to_string(atk));
	res.push_back("DFS: " + to_string(dfs));
	return res;
}

void props::trigger(void *Game){
	props *item = new props();
	memcpy(item, this, sizeof(props));
	((game*)Game)->player->bag.push_back(*item);
	((game*)Game)->print_prompt("You found " + name);
	valid = 0;
}


void props::use(void *Game){
	if(prop_type == 1 or prop_type == 2){
		if(((game*)Game)->player->get("eq" + to_string(prop_type)) == Hash(name)) {
			for(string command: commands){
				stringstream s(command);
				string key, opr; int v;
				s >> key >> opr >> v;
				if(opr == "+=") ((game*)Game)->player->add(key, -v);
				if(opr == "=") ((game*)Game)->player->mod(key, 0);
			}
			((game*)Game)->player->mod("eq" + to_string(prop_type), 0);
			((game*)Game)->print_prompt("Unload equipment " + name);
			return;
		} else if(((game*)Game)->player->get("eq" + to_string(prop_type)) != 0){
			((game*)Game)->print_prompt("You cannot be equiped with two " + (string)(prop_type == 1?"swords":"shields") +  " at the same time.");
		} else {
			for(string command: commands){
				stringstream s(command);
				string key, opr; int v;
				s >> key >> opr >> v;
				if(opr == "+=") ((game*)Game)->player->add(key, v);
				if(opr == "=") ((game*)Game)->player->mod(key, v);
			}
			((game*)Game)->player->mod("eq" + to_string(prop_type), Hash(name));
			((game*)Game)->print_prompt("Equiped with " + name);
		}
	} else if (prop_type == 3){
		for(string command: commands){
			stringstream s(command);
			string key, opr; int v;
			s >> key >> opr >> v;
			if(opr == "+=") ((game*)Game)->player->add(key, v);
			if(opr == "=") ((game*)Game)->player->mod(key, v);
		}
		valid = 0;
	}
	
}

vector<string> npc::render(){
	vector<string> render;
	render.push_back(name + ": ");
	render.push_back(" ");
	string tmp;
	for(int i = 0; i < script.length(); i++){
		tmp += script[i];
		if(tmp.size() >= 28) {
			render.push_back(tmp);
			tmp = "";
		}
	}
	if(tmp.size()) render.push_back(tmp);
	return render;
}

void npc::trigger(void *Game){
	game *G = (game*) Game;
	vector<string> render;
	for(int i = 0; i < number.size(); i++){
		stringstream s(commodity[i]);
		string a, b, c, d, e, f;
		s >> a >> b >> c >> d >> e >> f;
		if(number[i] == -1 or number[i] > 0) render.push_back("(" + to_string(i) + ") pay " + c + " " + b + " for " + f + " " + e);
	}
	render.push_back("(Q) Quit");
	
	while(1){
		G->draw(1, 1, 0, -1, 0); 
		int i = 10;
		for(string x: render){
			int j = 40;
			for(char y: x) mvprintw(i, j++, "%c", y);
			i++;
		}
		refresh();
		while(!G->events.empty()) {
			auto [typ, key] = G->events.front(); G->events.pop();
			if(typ == 0 and '0' <= key and key <= '9' and key - '0' < number.size()){
				if(!(number[key - '0'] == -1 or number[key - '0'] > 0)) continue;
				stringstream s(commodity[key - '0']);
				string a, b, d, e; int c, f;
				s >> a >> b >> c >> d >> e >> f;
				if(G->player->get(b) >= c){
					G->player->add(b, -c);
					G->player->add(e, f);
					if(number[key - '0'] != -1) number[key - '0'] -= 1;
					G->print_prompt("You paid " + to_string(c) + " " + b + " for " + to_string(f) + " " + e);
				} else {
					G->print_prompt("You don't have enough money.");
				}
			} 
			if (typ == 0 and tolower(key) == 'q') return;
		}
        usleep(50);
	}
}