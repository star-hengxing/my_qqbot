#include <iostream>
//#include <mirai.h>
#include "myheader.h"

using namespace std;
using namespace Cyan;

int main()
{
#if defined(WIN32) || defined(_WIN32)
	// 切换代码页，让 CMD 可以显示 UTF-8 字符
	system("chcp 65001");
#endif
	MiraiBot bot("127.0.0.1", 8080);
	// 检查一下版本
	try
	{
		// 获取 mirai-api-http 插件地版本
		string current_version = bot.GetApiVersion();
		// 获取 mirai-cpp 适配的版本
		string required_version = bot.GetRequiredApiVersion();
		cout << "! 需要的 API 版本: " << required_version
			<< "; 当前 API 版本: " << current_version << "; " << endl;
		if (current_version != required_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 适配的版本不同，可能存在潜在的异常。" << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}
	//	connect consloe
	while (true)
	{
		try
		{
			bot.Auth("hengxings783", 3044360150_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;
	//bot behavior
	bot.On<BotJoinGroupEvent>(
		[&](BotJoinGroupEvent e)
		{
			MiraiBot::SleepSeconds(2);
			bot.SendMessage(e.Group.GID, MessageChain().Plain("👴 进群了！都来欢迎 👴！"));
		});

	bot.On<MemberJoinEvent>(
		[&](MemberJoinEvent e)
		{
			string memberName = e.NewMember.MemberName;
			bot.SendMessage(e.NewMember.Group.GID,
				MessageChain().Plain("欢迎 " + memberName + " 加入本群!"));
		});

	bot.On<MemberLeaveEventQuit>(
		[&](MemberLeaveEventQuit e)
		{
			auto mc = MessageChain().Plain(e.Member.MemberName + "离开了群聊!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});
	//deal with event
	//bot.On<GroupMessage>(DeelWithMessage);
	bot.On<GroupMessage>(
		[&](GroupMessage e)
		{
			MessageChain msg = e.MessageChain;
			string command = msg.GetPlainText();
			if (command.find("!help") == 0 || command.find("！help") == 0)
			{
				e.Reply(MessageChain()
					.Plain("!steam搜索[游戏名]")
					.Plain("!issue")
				);
			}
			else if (command.find("!steam搜索") == 0 || command.find("！steam搜索") == 0)
			{
				string GameName = command.substr(command.find_last_of("steam搜索"), command.size());
				auto msg = get_game_msg(GameName);
				auto [context, image_url] = msg;
				e.Reply(MessageChain()
					.Plain(context)
					//.Plain(image_url)
				);
			}
			else if (command.find("!issue") == 0 || command.find("！issue") == 0)
			{
				e.Reply(MessageChain().Plain("如有问题请私聊发给bot"));
			}
		});

	bot.EventLoop();

	return 0;
}