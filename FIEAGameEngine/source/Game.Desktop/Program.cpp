#pragma warning(disable: 26812)
#pragma warning(disable: 26819)
#pragma warning(disable:4100)

#include "pch.h"
#include "SList.h"
#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "Event.h"
#include "Rendering/Renderer.h"
#include "Input/InputListener.h"
#include "LevelStuff/LevelManager.h"
#include "LevelStuff/Level.h"
#include "LevelStuff/Platform.h"
#include "LevelStuff/MoveablePlatform.h"
#include "LevelStuff/DestructablePlatform.h"
#include "LevelStuff/TimePlatform.h"
#include "JsonTableParseHelper.h"
#include "Player/PlayerManager.h"
#include "Player/PlayerProjectile.h"
#include "../Library.Desktop/EnemyBase.h"
#include "GameState.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ActionList.h"
#include "Vector.h"
#include "ActionSetPlayerHeight.h"
#include "ActionListIf.h"
#include "../Library.Desktop//EnemyHorizontal.h"
#include "../Library.Desktop///EnemyJump.h"
#include "Output/AudioManager.h"

#include "SDL2/SDL.h"
#undef main

#include "SDL2/SDL_image.h"

using namespace FieaGameEngine;
using namespace std::literals;

//initializes the registry
void InitializeRegistery();
void Score(PlayerManager& player);

int main()
{
	/* Init SDL and Renderer */
	SDL_Init(SDL_INIT_VIDEO);

	InitializeRegistery();
	Renderer* renderer = Renderer::GetInstance();
	renderer->Initialize("Jump++", 300, 250, 318, 500);

	EventQueue queue;
	InputListener input;
	input.SetEventQueue(queue);
	GameTime time;

	const std::string filename = "JSON\\Levels.json"s;

	GameObject root;
	GameState gameState(&root);
	gameState.SetEventQueue(&queue);

	/* FACTORY INITIALIZATION */
	PlatformFactory platformFactory;
	MoveablePlatformFactory moveablePlatformFactory;
	PlayerManagerFactory playerManagerFactory;
	LevelManagerFactory levelManagerFactory;
	LevelFactory levelFactory;
	PlayerProjectileFactory projectileFactory;
	EnemyBaseFactory enemyBaseFactory;
	TimePlatformFactory timePlatformFactory;
	DestructablePlatformFactory destructablePlatformFactory;
	GameObjectFactory gobjf;
	EnemyHorizontalFactory emhf;
	EnemyJumpFactory emjf;
	ActionSetPlayerHeightFactory actionSetPlayerHeightFactory;
	ActionListIfFactory actionListIfFactory;
	ActionListFactory factoryaction;
	/* INITIALIZE PARSING*/
	JsonTableParseHelper::Wrapper wrapper(root);
	JsonTableParseHelper tableParseHelper;
	JsonParseCoordinator parseCoordinator(wrapper);
	parseCoordinator.AddHelper(tableParseHelper);
	parseCoordinator.Initialize();
	parseCoordinator.ParseFromFile(filename);
	Datum* Children = root.Find("Children");
	LevelManager& levelManager = *Children->GetTable(1)->As<LevelManager>();
	Datum* levelsDatum = levelManager.Find("ActiveLevels");
	PlayerManager& player = *Children->GetTable(0)->As<PlayerManager>();
	player.SetEventQueue(queue);
	ActionListIf* ifblah = player.Actions().GetTable()->As<ActionListIf>();
	ifblah->SetCondition(0);
	gameState.SetClockEnabled();
	KeyPress keyPressSubscriber(&player);
	KeyRelease keyReleaseSubscriber(&player);
	PlayerJump playerJumpSubscriber(&player);
	Event<InputDownEventPayload>::Subscribe(keyPressSubscriber);
	Event<InputDownEventPayload>::ProcessSubscriberCommands();
	Event<InputUpEventPayload>::Subscribe(keyReleaseSubscriber);
	Event<InputUpEventPayload>::ProcessSubscriberCommands();
	Event<PlayerManager>::Subscribe(playerJumpSubscriber);
	Event<PlayerManager>::ProcessSubscriberCommands();
	AudioManager audio;

	float scrollSpeed = 1.5f;

	/* Load PNG images */
	renderer->LoadPNG("Content/background.png", "background");
	renderer->LoadPNG("Content/basePlatform.png", "basePlatform");
	renderer->LoadPNG("Content/basicEnemy.png", "basicEnemy");
	renderer->LoadPNG("Content/bullet.png", "bullet");
	renderer->LoadPNG("Content/destructablePlatform.png", "destructablePlatform");
	renderer->LoadPNG("Content/jumpingEnemy.png", "jumpingEnemy");
	renderer->LoadPNG("Content/movingEnemy.png", "movingEnemy");
	renderer->LoadPNG("Content/movingPlatform.png", "movingPlatform");
	renderer->LoadPNG("Content/paulPlayer.png", "player");
	renderer->LoadPNG("Content/timerPlatform.png", "timerPlatform");
	renderer->LoadPNG("Content/Title_Screen.png", "titleScreen");

	/*Load Audio*/
	audio.AddAudio("Content/jump.wav");
	audio.AddAudio("Content/breaking.wav");
	audio.AddAudio("Content/shoot.wav");
	audio.AddAudio("Content/falling.wav");

	bool isDisplayed = false;
	bool isScrolling = false;
	bool _isGameStarted{ false };
	bool _isQuit{ false };
	while (!_isQuit)
	{

		if (_isGameStarted)
		{
			if (player.GetPosition().y < -25 && !isScrolling)
			{
				player.SetCanMove(false);
				isScrolling = true;
				ifblah->SetCondition(1);
				//player.lastHeight = 500;
			}

			if (isScrolling)
			{
				Level& Level1 = *levelsDatum->GetTable(0)->As<Level>();
				Datum* Platforms = Level1.Find("Platforms");
				Datum* Enemies = Level1.Find("Enemies");
				for (size_t i = 0; i < Platforms->Size(); ++i)
				{
					Platforms->GetTable(i)->As<Platform>()->GetPosition().y += scrollSpeed;
				}


				for (size_t i = 0; i < Enemies->Size(); ++i)
				{
					Enemies->GetTable(i)->As<EnemyBase>()->GetPosition().y += scrollSpeed;
				}
				player.GetPosition().y += scrollSpeed;

				if (player.GetPosition().y > 475.f)
				{
					levelManager.SwitchActiveLevel();
					player.EnqueueJumpEvent(time);
					player.EnqueueJumpEvent(time);
					player.SetCanMove(true);
					isScrolling = false;
				}
			}

			if (!player._isGameOver)
			{
				input.Update(time);


				renderer->BeginFrame();
				renderer->Draw("background", 0, 0, 318, 117);
				renderer->Draw("background", 0, 117, 318, 117);
				renderer->Draw("background", 0, 234, 318, 117);
				renderer->Draw("background", 0, 351, 318, 117);
				renderer->Draw("background", 0, 468, 318, 117);
				gameState.Update();
				ifblah->SetCondition(0);
			}
			else
			{
				if (!isDisplayed)
				{
					Score(player);
					isDisplayed = true;
				}
				else
				{
					SDL_Event event;
					while (SDL_PollEvent(&event))
					{
						switch (event.type)
						{
						case SDL_KEYDOWN:
						{
							if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
							{
								_isQuit = true;
							}
						}
						}
					}
				}
			}
		}
		else
		{
			renderer->BeginFrame();
			renderer->Draw("titleScreen", 0, 0, 318, 500);
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_KEYDOWN:
					{
						if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
						{
						_isGameStarted = true;
						}
					}
				}
			}
		}
		renderer->EndFrame();


	}

	

	TypeRegistry::GetInstance()->Clear();

	return 0;
}

void InitializeRegistery()
{
	TypeRegistry::GetInstance()->RegisterType<GameObject>();
	TypeRegistry::GetInstance()->RegisterType<Action>();
	TypeRegistry::GetInstance()->RegisterType<ActionSetPlayerHeight, Action>();
	TypeRegistry::GetInstance()->RegisterType<ActionList, Action>();
	TypeRegistry::GetInstance()->RegisterType<ActionListIf, ActionList>();
	TypeRegistry::GetInstance()->RegisterType<Platform, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<MoveablePlatform, Platform>();
	TypeRegistry::GetInstance()->RegisterType<TimePlatform, Platform>();
	TypeRegistry::GetInstance()->RegisterType<DestructablePlatform, Platform>();
	TypeRegistry::GetInstance()->RegisterType<LevelManager, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<Level, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<PlayerManager, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<PlayerProjectile, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<EnemyBase, GameObject>();
	TypeRegistry::GetInstance()->RegisterType<EnemyHorizontal, EnemyBase>();
	TypeRegistry::GetInstance()->RegisterType<EnemyJump, EnemyBase>();
}

void Score(PlayerManager& player)
{
	Vector<int> oldScores;
	std::ifstream oldFile("highscore.txt");
	//myfile.open("highscore.txt", std::ios_base.);
	bool writeToFile = false;
	if (oldFile.is_open())
	{
		std::string line;
		while (getline(oldFile, line))
		{
			oldScores.PushBack(std::stoi(line));
			writeToFile = true;
		}

		if (!writeToFile)
		{
			oldScores.PushBack(player._score);
		}

		int nextNum = 0;
		for (size_t i = 0; i < oldScores.Size(); ++i)
		{
			if (player._score > oldScores[i])
			{
				nextNum = player._score;
				for (size_t j = i; j < oldScores.Size(); ++j)
				{
					if (i == oldScores.Size() - 1)
					{
						oldScores[j] = nextNum;
					}
					else
					{
						int toSave = oldScores[j];
						oldScores[j] = nextNum;
						nextNum = toSave;
					}

				}
				i = 5;
			}
		}
		std::ofstream myfile("highscore.txt");
		for (size_t i = 0; i < oldScores.Size(); ++i)
		{
			myfile << std::to_string(oldScores[i]) + "\n";
		}
		myfile.close();

	}
	Renderer::GetInstance()->BeginFrame();
	Renderer::GetInstance()->renderScore("HIGHSCORE", 75, 0);
	if (oldScores.Size() < 5)
	{
		for (int i = 0; i < static_cast<int>(oldScores.Size()); ++i)
		{
			Renderer::GetInstance()->renderScore(std::to_string(i + 1) + "\t" + std::to_string(oldScores[i]), 75, 50 + i * 50);
		}

	}

	else
	{
		for (int i = 0; i < 5; ++i)
		{
			Renderer::GetInstance()->renderScore(std::to_string(i + 1) + "\t" + std::to_string(oldScores[i]), 75, 50 + i * 50);
		}
	}
}
