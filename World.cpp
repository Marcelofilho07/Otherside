/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include "LinkedList.h"
#include <math.h>
#include <iostream>
#include "raylib.h"

//Singleton World class declaration.
class World
{
public:

	LinkedList worldList; //WorldList is a LinkedList
	Entity* player = NULL; //player entity
	int level = 0;
	
	Texture2D playerTxt = LoadTexture("Resources/player.png");
	Texture2D attackTxt = LoadTexture("Resources/playerAttack.png");
	Texture2D enemyTxt = LoadTexture("Resources/enemy.png");
	Texture2D bulletTxt = LoadTexture("Resources/bullet.png");
	Music bgm = LoadMusicStream("Resources/Linus Akesson - A Mind Is Born.mp3");

	static World& instance() //Singleton "constructor" method
	{
		static World* instance = new World();
		return *instance;
	}

	void resetWorld() //Reset level and worldList
	{
		LinkedList reset;
		level = 0;
		worldList = reset;
	}

private:
	World() {}
};