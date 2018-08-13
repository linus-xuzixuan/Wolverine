/* Player.h
By Linus Xu (linus-xuzixuan on GitHub)
*/
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;

class Player{
	public:
	void set_life(bool new_state);
	string set_id(int id);
	bool get_state() const;
	string get_identity() const;
	void guard();
	void clear_guard();
	bool get_shield() const;
	friend void clear_guard(Player Players[],int num);
	private:
	bool state;
	string identity;
	bool guarded;
};

void Player::set_life(bool new_state){
	state=new_state;
}

string Player::set_id(int id){
	switch(id){
		case 1:
		identity="Wolverine";
		break;
		case 2:
		identity="Villager";
		break;
		case 3:
		identity="Witch";
		break;
		case 4:
		identity="Predictor";
		break;
		case 5:
		identity="Hunter";
		break;
		case 6:
		identity="Guard";
	}
	return identity;
}

string Player::get_identity() const{
	return identity;
}

bool Player::get_state() const{
	return state;
}

bool Player::get_shield() const{
	return guarded;
}

void Player::guard(){
	guarded=true;
}

void clear_guard(Player Players[],int num){
	for(int i=0;i<num;i++){
		Players[i].guarded=false;
	}
}

#endif //PLAYER_H