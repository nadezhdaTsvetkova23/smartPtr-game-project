#include "game.h"
#include "player.h"
#include "gamekey.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<memory>
#include<numeric>
#include<vector>
#include<map>

Game::Game(std::string name, std::shared_ptr<Player> host) :
	name(name),
	host(host) {
	if (name.empty() || host == nullptr)
		throw std::runtime_error("illegal player name or host");
}

std::string Game::get_name() const {
	return name;
}

std::weak_ptr<Player> Game::get_host() const {
	return host;
}

std::map<std::string, std::weak_ptr<Player>> Game::get_players() const {
	return players;
}

bool Game::is_allowed(int n) const {
	std::shared_ptr<Player> help{ host.lock() };
	if (!help)
		return false;
	int mmr{ help.get()->get_mmr() };
	return (n > (mmr * 90) / 100 && n < (mmr * 110) / 100);
}

bool Game::remove_player(const GameKey& gk, std::shared_ptr<Player> p) {
	return players.erase(p->get_name());
}

bool Game::add_player(const GameKey& gk, std::shared_ptr<Player> p) {
	if (!is_allowed(p->get_mmr()))
		return false;

	for (auto it = players.begin(); it != players.end(); it++) {
		std::shared_ptr<Player> mp = it->second.lock();
		if (mp == p)
			return false;
	}

	players.insert(make_pair(p->get_name(), p));
	return true;

}

size_t Game::number_of_players() const {
	size_t num{ 0 };
	for (auto& elem : players) {
		if (elem.first != "" && !elem.second.expired())
			num++;
	}
	return num;
}

std::weak_ptr<Player> Game::best_player() const {
	if (number_of_players() == 0)
		throw std::runtime_error("no active players");
	auto it = max_element(players.begin(), players.end(), [](auto p1, auto p2) {
		if (std::shared_ptr<Player> p1_lock = p1.second.lock()) {
			if (std::shared_ptr<Player> p2_lock = p2.second.lock()) {
				return p1_lock->get_mmr() < p2_lock->get_mmr();
			}
			else
				return false;
		}
		else
			return false;
		});
	return it->second;
}

int Game::change(bool w) const {
	if (w)
		return 1;
	else
		return -1;
}


std::weak_ptr<Player> Game::play(std::size_t i)
{
	for (auto it = players.begin(); it != players.end();) {
		if (it->second.expired())
			players.erase(it++);
		else
			it++;
	}
	if (number_of_players() <= i || host.expired()) {
		throw std::runtime_error("illegal game");
	}
	std::weak_ptr<Player> win = std::weak_ptr<Player>(std::next(players.begin(), i)->second);
	for (auto it = players.begin(); it != players.end(); it++) {
		if (it->second.lock() != win.lock()) {
			if (it->second.lock()->get_mmr() > win.lock()->get_mmr())
				it->second.lock()->change_mmr(change(false) * 2);
			else
				it->second.lock()->change_mmr(change(false));
		}
	}
	win.lock()->change_mmr(change(true));
	return win;
}

std::ostream& Game::print(std::ostream& o) const
{
	/*: [name, host->name, host->mmr, player: {[Player_name, Player_mmr], [Player_name,
Player_mmr], ... }]
 [DotA 2, Juliane, 558, player: [Heinrich, 575], [Helmut, 582], [Juliane, 558]]*/
	std::shared_ptr<Player> help_host{ host.lock() };
	o << "[" << name << ", " << help_host->get_name() << ", " << help_host->get_mmr() << ", player: {";
	bool first{ true };
	for (const auto& elem : players) {
		std::shared_ptr<Player> help_elem{ elem.second.lock() };
		if (first) {
			o << "[" << help_elem->get_name() << ", " << help_elem->get_mmr() << "]";
			first = false;
		}
		else o << ", " "[" << help_elem->get_name() << ", " << help_elem->get_mmr() << "]";
	}
	o << "}]";
	return o;
}

std::ostream& operator<<(std::ostream& o, const Game& p) {
	return p.print(o);
}

RGame::RGame(std::string name, std::shared_ptr<Player> host) : Game{ name,host } {
}

int RGame::change(bool won) const {
	if (won)
		return 5;
	else
		return -5;
}

std::ostream& RGame::print(std::ostream& o) const {
	o << "Ranked Game: ";
	return Game::print(o);
}

UGame::UGame(std::string name, std::shared_ptr<Player> host) :Game{ name, host } {
}

int UGame::change(bool n) const {
	return 0;
}

std::ostream& UGame::print(std::ostream& o) const {
	o << "Unranked Game: ";
	return Game::print(o);
}
