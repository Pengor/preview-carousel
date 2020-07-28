#include <string>
#include <vector>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "curl_helper.h"
#include "game_list.h"

GameList::Game::Game(const char* photo_url, const char* away_team, const char* home_team, const char* field, const char* time)
{
	this->photo_url = photo_url;
	this->away_team = away_team;
	this->home_team = home_team;
	this->field = field;
	this->time = time;
}

GameList::GameList(const char *json_url)
{
	CURL *curl;
	CURLcode curl_error;

	struct MemoryStruct chunk;
	chunk.memory = (char *)malloc(1);
	chunk.size = 0;

	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, json_url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		curl_error = curl_easy_perform(curl);

		//TODO: Do some cleaner error handling here
		if (curl_error != CURLE_OK)
		{
			fprintf(stderr,
					"curl_easy_perform() failed: %s\n",
					curl_easy_strerror(curl_error));
		}

		curl_easy_cleanup(curl);
	}

	// Parse JSON
	rapidjson::Document document;
	document.Parse(chunk.memory);

	rapidjson::Value &games = document["dates"][0]["games"];

	//list.resize(sizeof(Game) * games.Size());

	for (rapidjson::SizeType i = 0; i < games.Size(); i++)
	{
		const char* photo_url = "";
		const char* away_team = 
			games[i]["teams"]["away"]["team"]["name"].GetString();
		const char* home_team = 
			games[i]["teams"]["home"]["team"]["name"].GetString();
		const char* field = "";
		const char* time = "";
		//printf("Game #%d Details:\n", i);
		//printf("%s vs. %s\n",
		//	games[i]["teams"]["away"]["team"]["name"].GetString(),
		//	games[i]["teams"]["home"]["team"]["name"].GetString());

		//list.push_back(Game(photo_url, away_team, home_team, field, time));
		/*list[i] = */
		//TODO: Figure out why this doesn't work
		Game game = Game(photo_url, away_team, home_team, field, time);
	}

	// Minimize memory used
	free(chunk.memory);
	document.SetObject();
}
