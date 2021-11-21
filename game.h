#ifndef GAME_H
#define GAME_H

#include<string>
#include<iostream>
#include<memory>
#include<vector>
#include<map>
#include"player.h"

class Player;
class GameKey;

enum class Mode;

class Game: public std::enable_shared_from_this<Game>{
protected:
	std::string name;
	std::weak_ptr<Player> host;
	std::map<std::string, std::weak_ptr<Player>> players;
public:
	//Constructors
	Game(std::string name, std::shared_ptr<Player>host);

	//Methods
	std::string get_name() const;
	std::weak_ptr<Player> get_host()const;
	std::map<std::string, std::weak_ptr<Player>>get_players() const;
	bool is_allowed(int n) const;
	bool remove_player(const GameKey& gk, std::shared_ptr<Player> p);
	bool add_player(const GameKey& gk, std::shared_ptr<Player> p);
	size_t number_of_players() const;
	std::weak_ptr<Player> best_player() const;
	virtual int change(bool) const = 0;
	std::weak_ptr<Player> play(size_t i); 
	virtual ~Game() = default;
	
	//print methods
	virtual std::ostream& print(std::ostream& o) const;
	friend std::ostream& operator<<(std::ostream& o, const Game& p);

	class RGame;
	class UGame;
	class MGame;
};

class RGame:public Game{
public:
	RGame(std::string, std::shared_ptr<Player>);
	int change(bool n)const;
	std::ostream& print(std::ostream& o)const;
};

class UGame:public Game {
public:
	UGame(std::string, std::shared_ptr<Player>);
	int change(bool n)const;
	std::ostream& print(std::ostream& o)const;
};

#endif 

