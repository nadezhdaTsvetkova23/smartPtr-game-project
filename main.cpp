#include "player.h"
#include "game.h"
#include <iostream>
#include <sstream>
#include <set>

int main() {
	std::map<std::string, bool> m;
	//1 Test player constructor with empty string
	{
		bool result = false;
		try {
			std::shared_ptr<Player> p = std::make_shared<Player>("", 100);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["01 Test player constructor with empty string"] = result;
	}
	//2 Test player constructor with mmr bigger 9999
	{
		bool result = false;
		try {
			std::shared_ptr<Player> p = std::make_shared<Player>("player", 10000);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["02 Test player constructor with mmr bigger 9999"] = result;
	}
	//3 Test player constructor with mmr lower 0
	{
		bool result = false;
		try {
			std::shared_ptr<Player> p = std::make_shared<Player>("player", -1);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["03 Test player constructor with mmr lower 0"] = result;
	}
	//4 Test player constructor valid and player get_name/get_mmr
	{
		bool result = true;
		try {
			std::shared_ptr<Player> p = std::make_shared<Player>("player", 0);
			std::shared_ptr<Player> p2 = std::make_shared<Player>("player", 9999);
		}
		catch (const std::runtime_error& error) {
			result = false;
		}
		if (result) {
			std::shared_ptr<Player> p = std::make_shared<Player>("player", 0);
			if (!(std::string::npos != p->get_name().find("player")))result = false;
			if (!(p->get_mmr() == 0))result = false;
		}
		m["04 Test player constructor valid and player get_name/get_mmr"] = result;
	}
	//5 Test player host_game valid
	{
		bool result = true;
		std::shared_ptr<Player> p = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 200);
		if (!(p->host_game("game1", Mode::Ranked)))result = false;
		if (!(p2->host_game("game2", Mode::Unranked)))result = false;
		if (!(std::string::npos != p->get_hosted_game()->get_name().find("game1")))result = false;
		if (!(std::string::npos != p2->get_hosted_game()->get_name().find("game2")))result = false;
		m["05 Test player host_game valid"] = result;
	}
	//6 Test player host_game error game string empty
	{
		bool result = false;
		try {
			std::shared_ptr<Player> p = std::make_shared<Player>("player", 0);
			p->host_game("", Mode::Ranked);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["06 Test player host_game error game string empty"] = result;
	}
	//7 Test player host_game where game already exists
	{
		bool result = false;
		std::shared_ptr<Player> p = std::make_shared<Player>("player", 100);
		p->host_game("game", Mode::Ranked);
		if (!(p->host_game("game", Mode::Ranked)))result = true;
		m["07 Test player host_game where game already exists"] = result;
	}
	//8 Test player host_game is Ranked/Unranked
	{
		bool result = true;
		std::shared_ptr<Player> p = std::make_shared<Player>("player", 100);
		p->host_game("game", Mode::Ranked);
		std::string s;
		std::ostringstream oss;
		p->get_hosted_game()->print(oss);
		s = oss.str();
		if (!(std::string::npos != s.find("Ranked Game: ")))result = false;
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player", 100);
		p2->host_game("game", Mode::Unranked);
		std::string s2;
		std::ostringstream oss2;
		p2->get_hosted_game()->print(oss2);
		s2 = oss2.str();
		if (!(std::string::npos != s2.find("Unranked Game: ")))result = false;
		m["08 Test player host_game is Ranked/Unranked"] = result;
	}
	//9 Test player change mmr
	{
		bool result = true;
		std::shared_ptr<Player> p = std::make_shared<Player>("player", 100);
		p->change_mmr(100);
		if (p->get_mmr() != 200)result = false;
		p->change_mmr(-200);
		if (p->get_mmr() != 0)result = false;
		p->change_mmr(-200);
		if (p->get_mmr() != 0)result = false;
		p->change_mmr(9999);
		if (p->get_mmr() != 9999)result = false;
		p->change_mmr(9999);
		if (p->get_mmr() != 9999)result = false;
		m["09 Test player change mmr"] = result;
	}
	//10 Test player join_game
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 99);
		p1->host_game("Dota2", Mode::Ranked);
		result = p2->join_game(p1->get_hosted_game());
		m["10 Test player join_game"] = result;

	}
	//11 Test player join_game join twice
	{
		bool result = true;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 99);
		p1->host_game("Dota2", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		result = p2->join_game(p1->get_hosted_game());
		m["11 Test player join_game join twice"] = !result;

	}
	//12 Test player leave_game
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 99);
		p1->host_game("Dota2", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		result = p2->leave_game(p1->get_hosted_game());
		m["12 Test player leave_game"] = result;

	}
	//13 Test player leave_game leave twice
	{
		bool result = true;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 99);
		p1->host_game("Dota2", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		p2->leave_game(p1->get_hosted_game());
		result = p2->leave_game(p1->get_hosted_game());
		m["13 Test player leave_game leave twice"] = !result;

	}
	//14 Test player close game 1
	{
		bool result = true;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		if (p1->get_hosted_game() != nullptr)result = false;
		p1->host_game("Dota2", Mode::Ranked);
		if (p1->get_hosted_game() == nullptr)result = false;
		p1->close_game();
		if (p1->get_hosted_game() != nullptr)result = false;
		m["14 Test player close game 1"] = result;
	}
	//15 Test plyer close game 2
	{
		bool result = true;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		p1->host_game("Dota2", Mode::Ranked);
		if (!(p1->close_game()))result = false;
		if (p1->close_game())result = false;
		m["15 Test player close game 2"] = result;
	}
	//16 Test player invite_players valid
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 121);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 120);
		std::vector<std::weak_ptr<Player>> v;
		v.push_back(std::weak_ptr<Player>(p1));
		v.push_back(std::weak_ptr<Player>(p2));
		v.push_back(std::weak_ptr<Player>(p3));
		p1->host_game("Dota2", Mode::Ranked);
		int ret_v_size = p1->invite_players(v).size();
		if (ret_v_size == 0)result = true;
		m["16 Test player invite_players valid"] = result;
	}
	//17 Test player invite_players mmr range to far
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 140);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 140);
		std::vector<std::weak_ptr<Player>> v;
		v.push_back(std::weak_ptr<Player>(p1));
		v.push_back(std::weak_ptr<Player>(p2));
		v.push_back(std::weak_ptr<Player>(p3));
		p1->host_game("Dota2", Mode::Ranked);
		int ret_v_size = p1->invite_players(v).size();
		if (ret_v_size == 2)result = true;
		m["17 Test player invite_players mmr range to far"] = result;
	}
	//18 Test player invite_players players expired
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 120);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 121);
		std::vector<std::weak_ptr<Player>> v;
		v.push_back(std::weak_ptr<Player>(p1));
		v.push_back(std::weak_ptr<Player>(p2));
		v.push_back(std::weak_ptr<Player>(p3));
		p1->host_game("Dota2", Mode::Ranked);
		p2.reset();
		p3.reset();
		int ret_v_size = p1->invite_players(v).size();
		if (ret_v_size == 2)result = true;
		m["18 Test player invite_players players expired"] = result;
	}
	//19 Test player join_game with print 1
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 120);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 121);
		p1->host_game("Dota2", Mode::Ranked);
		p2->host_game("Sims", Mode::Unranked);
		p3->host_game("WoW", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p1->join_game(p2->get_hosted_game());
		p1->join_game(p3->get_hosted_game());
		std::ostringstream ossp;
		std::string sp;
		p1->print(ossp);
		sp = ossp.str();
		std::ostringstream ossg;
		std::string sg;
		p1->get_hosted_game()->print(ossg);
		sg = ossg.str();
		if (std::string::npos != sp.find("[player1, 119, hosts: Dota2, games: {Dota2, Sims, WoW}]") &&
			std::string::npos != sg.find("Ranked Game: [Dota2, player1, 119, player: {[player1, 119]}]"))result = true;
		m["19 Test player join_game with print 1"] = result;

	}
	//20 Test player join_game with print 2
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 120);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 121);
		p1->host_game("Dota2", Mode::Unranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		std::ostringstream ossp;
		std::string sp;
		p1->print(ossp);
		sp = ossp.str();
		std::ostringstream ossg;
		std::string sg;
		p1->get_hosted_game()->print(ossg);
		sg = ossg.str();
		if (std::string::npos != sp.find("[player1, 119, hosts: Dota2, games: {Dota2}]") &&
			std::string::npos != sg.find("Unranked Game: [Dota2, player1, 119, player: {[player1, 119], [player2, 120], [player3, 121]}]"))result = true;
		m["20 Test player join_game with print 2"] = result;

	}
	//21 Test player invite_player with print
	{
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 120);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 121);
		std::vector<std::weak_ptr<Player>> v;
		v.push_back(std::weak_ptr<Player>(p1));
		v.push_back(std::weak_ptr<Player>(p2));
		v.push_back(std::weak_ptr<Player>(p3));
		p1->host_game("Dota2", Mode::Ranked);
		p1->invite_players(v);
		std::ostringstream ossg;
		std::string sg;
		p1->get_hosted_game()->print(ossg);
		sg = ossg.str();
		if (std::string::npos != sg.find("Ranked Game: [Dota2, player1, 119, player: {[player1, 119], [player2, 120], [player3, 121]}]"))result = true;
		m["21 Test player invite_player with print"] = result;

	}
	{
		//22 Test game konstruktor valid
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		p1->host_game("Game1", Mode::Ranked);
		std::ostringstream ossg;
		std::string sg;
		p1->get_hosted_game()->print(ossg);
		sg = ossg.str();
		if (std::string::npos != p1->get_hosted_game()->get_name().find("Game1") && std::string::npos != sg.find("Ranked Game: ") &&
			std::string::npos != sg.find("player1"))result = true;
		m["22 Test game konstruktor valid"] = result;
	}
	{
		//23 Test game konstruktor error name empty
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		try {
			p1->host_game("", Mode::Ranked);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}

		m["23 Test game konstruktor error name empty"] = result;
	}
	{
		//24 Test game konstruktor error host is nullptr
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 119);
		p1.reset();
		try {
			std::shared_ptr<Game> g = std::make_shared<RGame>("Game", p1);

		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["24 Test game konstruktor error host is nullptr"] = result;
	}
	{
		//25 Test game is_allowed valid
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		p1->host_game("Game", Mode::Ranked);
		if (p1->get_hosted_game()->is_allowed(109) && p1->get_hosted_game()->is_allowed(91))result = true;
		m["25 Test game is_allowed valid"] = result;

	}
	{
		//26 Test game is_allowed not valid
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		p1->host_game("Game", Mode::Ranked);
		if (!p1->get_hosted_game()->is_allowed(110) && !p1->get_hosted_game()->is_allowed(90))result = true;
		m["26 Test game is_allowed not valid"] = result;

	}
	{
		//27 Test game number of players
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 100);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 100);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 100);
		p1->host_game("Game", Mode::Unranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		p2.reset();
		p4.reset();
		if (p1->get_hosted_game()->number_of_players() == 2)result = true;
		m["27 Test game number of players"] = result;

	}
	{
		//28 Test game get_best player
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 99);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 106);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 106);
		p1->host_game("Game", Mode::Unranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		if (p1->get_hosted_game()->best_player().lock() == p3)result = true;
		m["28 Test game get_best player"] = result;

	}
	{
		//29 Test game get_best player with expired player
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 99);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 106);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 106);
		p1->host_game("Game", Mode::Unranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		p3.reset();
		if (p1->get_hosted_game()->best_player().lock() == p4)result = true;
		m["29 Test game get_best player with expired player"] = result;

	}
	{
		//30 Test game play Ranked
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 102);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 103);
		p1->host_game("Game", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		p1->get_hosted_game()->play(1);
		if (p1->get_mmr() == 95 && p2->get_mmr() == 106 && p3->get_mmr() == 92 && p4->get_mmr() == 93)result = true;
		m["30 Test game play Ranked"] = result;

	}
	{
		//31 Test game play Unranked
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 102);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 103);
		p1->host_game("Game", Mode::Unranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		p1->get_hosted_game()->play(1);
		if (p1->get_mmr() == 100 && p2->get_mmr() == 101 && p3->get_mmr() == 102 && p4->get_mmr() == 103)result = true;
		m["31 Test game play Unranked"] = result;

	}
	{
		//32 Test game play Ranked Expired players
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 102);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 103);
		p1->host_game("Game", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		p2.reset();
		p4.reset();
		p1->get_hosted_game()->play(1);
		if (p1->get_mmr() == 95 && p3->get_mmr() == 107)result = true;
		m["32 Test game play Ranked Expired players"] = result;

	}
	{
		//33 Test game play Ranked index error
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 102);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 103);
		p1->host_game("Game", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		try {
			p1->get_hosted_game()->play(4);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["33 Test game play Ranked index error"] = result;

	}
	{
		//34 Test game play Ranked host expired error
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		std::shared_ptr<Player> p3 = std::make_shared<Player>("player3", 102);
		std::shared_ptr<Player> p4 = std::make_shared<Player>("player4", 103);
		p1->host_game("Game", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p2->join_game(p1->get_hosted_game());
		p3->join_game(p1->get_hosted_game());
		p4->join_game(p1->get_hosted_game());
		std::shared_ptr<Game> g = p1->get_hosted_game();
		p1.reset();
		try {
			g->play(0);
		}
		catch (const std::runtime_error& error) {
			result = true;
		}
		m["34 Test game play Ranked host expired error"] = result;

	}
	{
		//35 Test RGame change
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		p1->host_game("Game", Mode::Ranked);
		if (p1->get_hosted_game()->change(true) == 5 && p1->get_hosted_game()->change(false) == -5)result = true;
		m["35 Test RGame change"] = result;

	}
	{
		//36 Test UGame change
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		p1->host_game("Game", Mode::Unranked);
		if (p1->get_hosted_game()->change(true) == 0 && p1->get_hosted_game()->change(false) == 0)result = true;
		m["36 Test UGame change"] = result;

	}
	{
		//37 Test add_player valid
		bool result = false;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		p1->host_game("Game", Mode::Ranked);
		result = p2->join_game(p1->get_hosted_game());
		m["37 Test add_player valid"] = result;
	}
	{
		//38 Test add_player invalid join twice
		bool result;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		p1->host_game("Game", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		result = p2->join_game(p1->get_hosted_game());
		m["38 Test add_player invalid join twice"] = !result;
	}
	{
		//39 Test add_player valid join when player with same name is expired in map
		bool result;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		p1->host_game("Game", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		p2.reset();
		p2 = std::make_shared<Player>("player2", 101);
		result = p2->join_game(p1->get_hosted_game());
		m["39 Test add_player valid join when player with same name is expired in map"] = result;
	}
	{
		//40 Test remove_player valid
		bool result;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		p1->host_game("Game", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		result = p2->leave_game(p1->get_hosted_game());
		m["40 Test remove_player valid"] = result;
	}
	{
		//41 Test remove_player twice
		bool result;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);
		std::shared_ptr<Player> p2 = std::make_shared<Player>("player2", 101);
		p1->host_game("Game", Mode::Ranked);
		p2->join_game(p1->get_hosted_game());
		p2->leave_game(p1->get_hosted_game());
		result = p2->leave_game(p1->get_hosted_game());
		m["41 Test remove_player twice"] = !result;
	}
	{
		//42 Test Player Test add Games with same name as expired game to map
		bool result;
		std::shared_ptr<Player> p1 = std::make_shared<Player>("player1", 100);

		p1->host_game("Game1", Mode::Ranked);
		p1->join_game(p1->get_hosted_game());
		p1->close_game();
		p1->host_game("Game1", Mode::Ranked);
		result = p1->join_game(p1->get_hosted_game());
		m["42 Test Player Test add Games with same name as expired game to map"] = result;
	}
	std::cout << "Tests: ";
	std::size_t cnt = 0;
	for (auto itr = m.begin(); itr != m.end(); ++itr) {
		if (itr->second)++cnt;
	}
	std::cout << cnt << "/" << m.size() << " passed" << std::endl;
	for (auto itr = m.begin(); itr != m.end(); ++itr) {
		if (!itr->second) {
			std::cout << itr->first << ": Failed" << std::endl;
		}
	}
	/*
	{
		//add everthing to everyone and test print
		std::vector<std::shared_ptr<Player>> v={std::make_shared<Player>("player1",100),std::make_shared<Player>("player2",101),
				std::make_shared<Player>("player3",102),std::make_shared<Player>("player4",103)};
		v.at(0)->host_game("Game1", Mode::Ranked);
		v.at(1)->host_game("Game2", Mode::Ranked);
		v.at(2)->host_game("Game3", Mode::Ranked);
		v.at(3)->host_game("Game4", Mode::Ranked);
		std::vector<std::weak_ptr<Player>> vw={v.begin(),v.end()};
		for(auto itr=v.begin();itr!=v.end();++itr){
			itr->get()->invite_players(vw);
		}
		for(auto itr=v.begin();itr!=v.end();++itr){
			itr->get()->print(std::cout);
			std::cout<<std::endl;
			itr->get()->get_hosted_game()->print(std::cout);
			std::cout<<std::endl;
		}

	}
	{
		//add everthing to everyone and test print 2 close games an reset player
		std::vector<std::shared_ptr<Player>> v={std::make_shared<Player>("player1",100),std::make_shared<Player>("player2",101),
				std::make_shared<Player>("player3",102),std::make_shared<Player>("player4",103)};
		v.at(0)->host_game("Game1", Mode::Ranked);
		v.at(1)->host_game("Game2", Mode::Ranked);
		v.at(2)->host_game("Game3", Mode::Ranked);
		v.at(3)->host_game("Game4", Mode::Ranked);
		std::vector<std::weak_ptr<Player>> vw={v.begin(),v.end()};
		for(auto itr=v.begin();itr!=v.end();++itr){
			itr->get()->invite_players(vw);
		}
		v.at(0)->close_game();
		v.at(3)->close_game();
		v.at(2).reset();
		for(auto itr=v.begin();itr!=v.end();++itr){
			if(*itr){
				itr->get()->print(std::cout);
				std::cout<<std::endl;
				if(itr->get()->get_hosted_game()){
					itr->get()->get_hosted_game()->print(std::cout);
					std::cout<<std::endl;
				}
			}
		}

	}
	*/
	return 0;
}
/*
[player1, 100, hosts: Game1, games: {Game1, Game2, Game3, Game4}]
Ranked Game: [Game1, player1, 100, player: {[player1, 100], [player2, 101], [player3, 102], [player4, 103]}]
[player2, 101, hosts: Game2, games: {Game1, Game2, Game3, Game4}]
Ranked Game: [Game2, player2, 101, player: {[player1, 100], [player2, 101], [player3, 102], [player4, 103]}]
[player3, 102, hosts: Game3, games: {Game1, Game2, Game3, Game4}]
Ranked Game: [Game3, player3, 102, player: {[player1, 100], [player2, 101], [player3, 102], [player4, 103]}]
[player4, 103, hosts: Game4, games: {Game1, Game2, Game3, Game4}]
Ranked Game: [Game4, player4, 103, player: {[player1, 100], [player2, 101], [player3, 102], [player4, 103]}]
[player1, 100, hosts: nothing, games: {Game2}]
[player2, 101, hosts: Game2, games: {Game2}]
Ranked Game: [Game2, player2, 101, player: {[player1, 100], [player2, 101], [player4, 103]}]
[player4, 103, hosts: nothing, games: {Game2}]
*/
using namespace std;

enum class Skill_Level { Noob, Casual, Pro };

map<Skill_Level, set<string>> prostrich(const vector<weak_ptr<Player>>& v) {
	map<Skill_Level, set<string>> player_sk;

	for (const auto& elem : v) {
		shared_ptr<Player> shared_elem = elem.lock();
		if (shared_elem) {
			
			Skill_Level skill{ Skill_Level::Casual };
			string name = shared_elem->get_name();

			int last_char = (int)name.at(name.size() - 1);

			if (last_char == 45)
				skill = Skill_Level::Pro;
			if (last_char >= 48 && last_char <= 57)
				skill = Skill_Level::Noob;

			if (player_sk.find(skill) == player_sk.end())
				player_sk[skill] = set<string>();
			player_sk[skill].insert(name);
		}
	}
	return player_sk;
}