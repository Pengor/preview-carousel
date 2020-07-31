// Container object for data loaded from API's JSON

#include <string>
#include <vector>
#include <curl/curl.h>
#include "document.h"
#include "curl_helper.h"
#include "game_list.h"

const size_t TIME_POS = 11;
const size_t TIME_LENGTH = 5;

// Build a single game using the given parameters
GameList::Game::Game(const char *photo_url, const char *away_team,
					 const char *home_team, const char *field, const char *time)
{
	this->photo_url = photo_url;

	this->headline = away_team + " vs "s + home_team;
	std::string time_str = time;
	this->details = time_str.substr(TIME_POS, TIME_LENGTH) + "\n"s + field;

	this->photo.memory = (char *)malloc(1);
	this->photo.size = 0;
}

std::string GameList::Game::GetHeadline(void)
{
	return headline;
}

std::string GameList::Game::GetDetails(void)
{
	return details;
}

void GameList::Game::CurlPhoto(void)
{
	// Get photo if it isn't already in memory
	if (photo.size == 0) 
	{
		CurlFromUrl(&photo, photo_url.c_str());
	}
}

void GameList::Game::ReleasePhoto(void)
{
	free(photo.memory);
	photo.size = 0;
}

char* GameList::Game::GetPhotoPointer(void)
{
	return photo.memory;
}

size_t GameList::Game::GetPhotoSize(void)
{
	return photo.size;
}

// Build object from a given URL to get the JSON data from
GameList::GameList(const char *json_url)
{
	//TODO Improvement: Cache the API response to speed up future runs

	// Get JSON from API
	struct MemoryStruct chunk;
	CurlFromUrl(&chunk, json_url);

	// Parse JSON
	rapidjson::Document document;
	document.Parse(chunk.memory);

	// Build linked list of details of day's games from JSON data
	rapidjson::Value &games = document["dates"][0]["games"];
	list_size = games.Size();
	list.reserve(list_size);
	for (rapidjson::SizeType i = 0; i < games.Size(); i++)
	{
		//TODO Improvement: dynamically get most appropriate size image
		// to fit the screen the application is running on
		list.emplace_back(
			games[i]["content"]["editorial"]["recap"]["mlb"]["photo"]["cuts"]
				 ["135x77"]["src"].GetString(),
			games[i]["teams"]["away"]["team"]["name"].GetString(),
			games[i]["teams"]["home"]["team"]["name"].GetString(),
			games[i]["venue"]["name"].GetString(),
			games[i]["gameDate"].GetString());
	}

	// Clean up memory footprint
	free(chunk.memory);
	chunk.size = 0;
	document.SetObject();
}

size_t GameList::GetListSize(void)
{
	return list_size;
}

std::string GameList::GetHeadline(size_t index)
{
	return list[index].GetHeadline();
}

std::string GameList::GetDetails(size_t index)
{
	return list[index].GetDetails();
}

void GameList::CurlPhoto(size_t index)
{
	list[index].CurlPhoto();
}

void GameList::ReleasePhoto(size_t index)
{
	list[index].ReleasePhoto();
}

char* GameList::GetPhotoPointer(size_t index)
{
	return list[index].GetPhotoPointer();
}

size_t GameList::GetPhotoSize(size_t index)
{
	return list[index].GetPhotoSize();
}
