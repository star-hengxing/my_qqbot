#pragma once

#include <mirai.h>
#include "http/HTTPS.hpp"
#include "Transcoding.hpp"

string http_run(HTTP* x)
{
	x->get();
	x->connect_server();
	x->parse_response();
	if (x->response_status_code() == 200)
	{
		return x->GetPage();
	}
	else
	{
		throw runtime_error("http状态码为" + to_string(x->response_status_code()));
	}
}

string search_game(const string& game_name)
{
	HTTPS steam("https://store.steampowered.com/search/?term=" + ChineseURLEncode(game_name));
	HTTP* x = &steam;
	string page = http_run(x);

	//string appid;
	if (smatch result; regex_search(page, result, regex("data-ds-appid=\"(.*?)\"")))
	{
		return result[1];
	}
	else
	{
		throw runtime_error("找不到这个游戏！");
	}
}

tuple<string, string> get_game_msg(const string& appid)
{
	//https://store.steampowered.com/api/appdetails/?appids=346110
	HTTPS steam("https://store.steampowered.com/api/appdetails/?appids=" + appid);
	HTTP* x = &steam;
	string page = http_run(x);
	json j = json::parse(page);
	auto data = j[appid]["data"];
	string msg;
	msg += "游戏名: " + data["name"].get<string>() + "\n";
	if (data["is_free"].get<bool>())
	{
		msg += "价格: 免费\n";
	}
	else
	{
		if (data["price_overview"]["initial_formatted"] == "")
		{
			msg += "价格: " + data["price_overview"]["final_formatted"].get<string>() + "\n";
		}
		else
		{
			msg += "原价: "
				+ data["price_overview"]["initial_formatted"].get<string>()
				+ " 现价: "
				+ data["price_overview"]["final_formatted"].get<string>()
				+ "\n";
		}
	}
	msg += "游戏介绍: " + data["short_description"].get<string>() + "\n";
	return{ msg, data["header_image"] };
}
