#ifndef GAME_LIST_H
#define GAME_LIST_H

#include <string>
#include <vector>

using namespace std;

class GameList
{
public:
	class Game
	{
		string photo_url;
		string away_team;
		string home_team;
		string field;
		string time;

	public:
		Game(const char *photo_url, const char *away_team, const char *home_team, const char *field,
			 const char *time);
	};

private:
	vector<Game> list;

public:
	GameList(const char *json_url);
	string GetPhotoUrl(size_t index);
	string GetAwayTeam(size_t index);
	string GetHomeTeam(size_t index);
	string GetField(size_t index);
	string GetTime(size_t index);
};

#endif /* GAME_LIST_H */
