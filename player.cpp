#include "player.h"
#include "game.h"
#include "gamekey.h"
#include<string>
#include<iostream>
#include<memory>
#include<algorithm>
#include<vector>
#include<map>

Player::Player(std::string name, int mmr): name(name), mmr(mmr)	{
	if (name.empty() || mmr < 0 || mmr>9999)
		throw std::runtime_error{ "illegal name or mmr" };
}

std::string Player::get_name() const	{
	return name;
}

int Player::get_mmr() const	{
	return mmr;
}

std::shared_ptr<Game> Player::get_hosted_game() const	{
	return hosted_game;
}

void Player::change_mmr(int n)	{
	int new_value = mmr + n;
	if (new_value >= 0 && new_value <= 9999)
		this->mmr = new_value;
	else if (new_value < 0)
		this->mmr = 0;
	else if (new_value > 9999)
		this->mmr = 9999;
}

bool Player::host_game(std::string s, Mode m)	{
	if (s.empty())
		throw std::runtime_error("empty string");
	if (get_hosted_game() != nullptr) 
		return false;
	else {
		switch (m) {
		case Mode::Ranked:
			hosted_game = std::make_shared<RGame>(s, shared_from_this());
			break;
		case Mode::Unranked:
			hosted_game = std::make_shared<UGame>(s, shared_from_this());
			break;
		}
		return true;
	}
}

bool Player::join_game(std::shared_ptr<Game> g)	{
	if (g == nullptr)
		return false;
	GameKey gk;
	bool help = g->add_player(gk, shared_from_this());
	if (!help)
		return false;
	games.insert(std::make_pair(g->get_name(), g));
	return true;
}

bool Player::leave_game(std::shared_ptr<Game> g) {
	if (g == nullptr)
		return false;

	auto it = games.find(g->get_name());
	if (it != games.end()) {
		games.erase(it);
		GameKey gk;
		return g->remove_player(gk, shared_from_this());
	}
	return false;
	
}

std::vector<std::weak_ptr<Player>> Player::invite_players(const std::vector<std::weak_ptr<Player>>& v)	{
	std::vector<std::weak_ptr<Player>> ret;
	for (const auto& elem : v)	{
		bool valid = false;
		if (!elem.expired()) 
			valid = elem.lock()->join_game(hosted_game);
		if (!valid) 
			ret.push_back(elem);
	}
	return ret;
}

bool Player::close_game()	{
	if (hosted_game != nullptr) {
		hosted_game = nullptr;
		return true;
	}
	else 
		return false;
}

std::ostream& Player::print(std::ostream& o) const	{
	// Format: [name, mmr, hosts: hosted_game_name, games: {Game_name, Game_name, ... }]
	//Beispiel: [Heinrich, 20, hosts: nothing, games{Sims 4, Sims 3, Doom}]
	o << "[" << get_name() << ", " << get_mmr() << ", hosts: ";
	if (hosted_game)
		o << get_hosted_game()->get_name();
	else
		o << "nothing";
	o << ", games: {";
	bool first = true;
	for (auto elem : games) 
		if (!elem.second.expired()) {
			if (!first)
				o << ", ";
			o << elem.second.lock()->get_name();
			first = false;
		}
	return o << "}]";
}

std::ostream& operator<<(std::ostream& o, const Player& p)	{
	return p.print(o);
}
