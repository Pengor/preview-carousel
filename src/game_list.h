#ifndef GAME_LIST_H
#define GAME_LIST_H

#include <string>
#include <vector>
#include "curl_helper.h"

using namespace std;

class GameList
{
public:
	class Game
	{
		string photo_url;
		//string away_team;
		//string home_team;
		//string field;
		//string time;
		string headline;
		string details;

		struct MemoryStruct photo;

	public:
		Game(const char *photo_url, const char *away_team,
			 const char *home_team, const char *field, const char *time);
		//string GetPhotoUrl(void);
		//string GetAwayTeam(void);
		//string GetHomeTeam(void);
		//string GetField(void);
		//string GetTime(void);
		
		string GetHeadline(void);
		string GetDetails(void);

		void CurlPhoto(void);
		void ReleasePhoto(void);

		char * GetPhotoPointer(void);
		size_t GetPhotoSize(void);
	};

private:
	vector<Game> list;
	size_t list_size;

public:
	GameList(const char *json_url);

	size_t GetListSize(void);

	//string GetPhotoUrl(size_t index);
	//string GetAwayTeam(size_t index);
	//string GetHomeTeam(size_t index);
	//string GetField(size_t index);
	//string GetTime(size_t index);

	string GetHeadline(size_t index);
	string GetDetails(size_t index);

	void CurlPhoto(size_t index);
	void ReleasePhoto(size_t index);

	char * GetPhotoPointer(size_t index);
	size_t GetPhotoSize(size_t size);
};

#endif /* GAME_LIST_H */
