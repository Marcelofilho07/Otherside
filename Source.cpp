/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include <cmath>
#include <stdlib.h>  
#include "Entity.h"

void handleCol(Node* n) //Handle collision
{
	/*
	Simple colission handling algorithm. All game entities are circles.
	Checking if shortest distance between both entities are lesser than their radius sum.
	If collision is true mark it as pending to be destroyed.
	*/
	Node* pointer = n;
	while (pointer != NULL)
	{
		if (pointer->data->id != n->data->id)
		{
			double dx = (double)n->data->pos.x - pointer->data->pos.x;
			double dy = (double)n->data->pos.y - pointer->data->pos.y;
			double distance = sqrt(dx * dx + dy * dy);
			if (distance < (n->data->radius + pointer->data->radius))
			{
				if (n->data->id == "BU" && pointer->data->id == "Player") //If a bullet(BU) hits the player, destroy both.
				{
					n->data->destroy = true;
					pointer->data->destroy = true;
				}
				else if (n->data->id == "PA" && pointer->data->id == "Enemy") //If a Player Attack(PA) hits an Enemy, destroy the enemy.
				{
					pointer->data->destroy = true;
				}

				else if (n->data->id == "PA" && pointer->data->id == "BU") //If a Player Attack(PA) hits an Bullet, destroy the Bullet.
				{
					pointer->data->destroy = true;
				}
			}
		}
		pointer = pointer->next;
	}
}

void Update(double elapsedTime, World& w) //Update checks every entity in the game
{
	Node* n = w.worldList.head;
	UpdateMusicStream(w.bgm);
	
	/*
	Check if there are no enemies left by checking if there is only one entity(the player) left.
	If true, will go to next level and repopulate the WorldList with new enemies equal to the level we're at.
	*/
	if (w.worldList.length <= 1 && !w.player->destroy)
	{
		w.level++;
		//Spawn i enemies at random position
		for (int i = 0; i < w.level; i++) 
		{
			int iSecret = rand() % (GetScreenWidth()-50) + 50;
			int iSecret2 = rand() % (GetScreenHeight()-50) + 50;
			Vector2 vEnemyPos = { (double)((iSecret)), (double)((iSecret2)) };
			Enemy* enemy = new Enemy("Enemy", vEnemyPos, { 100 ,100 }, 16, w, 1, w.enemyTxt); 
			w.worldList.appendToHead(enemy);
		}
	}

	n = w.worldList.head;
	while (n != NULL) 
	{
		handleCol(n); //Handle collisions
		n->data->Update(elapsedTime); //Update entity
		n = n->next;
	}

	if (w.player->destroy && (GetGamepadButtonPressed() == 15 || IsKeyPressed(KEY_ENTER))) //Restart the game. Set score to zero and create a new player.
	{
		w.resetWorld();
		Vector2 vPlayerPos = { (double)800 / 2, (double)600 / 2 };
		Player* player = new Player("Player", vPlayerPos, { 200 ,200 }, 16, w, 1, w.playerTxt);
		w.worldList.appendToHead(player);
		w.player = player;
	}
}

//Draw World
void Draw(World& w)
{
	Node* n = w.worldList.head;
	Node* previous = w.worldList.head;
	ClearBackground(BLACK);
	DrawText(TextFormat("LEVEL: %08i", w.level), 10, 10, 20, RED);

	while (n != NULL)
	{
		//If entity is marked to be destroyed, handle LinkedList deletion and destroy it.
		if (n->data->destroy) 
		{
			Node* garbageCollector = n;
			if (n == w.worldList.head)
				w.worldList.head = n->next;
			else if (n->next != NULL)
				previous->next = n->next;
			else
				previous->next = NULL;

			n->data->~Entity();
			w.worldList.length--;
			previous = n->next;
			n = n->next;
			garbageCollector->~Node();
		}
		else //Else draw entity.
		{
			n->data->Draw();
			previous = n;
			n = n->next;
		}
	}

	if (w.player->destroy) //Check if player is dead to write "GAME OVER!"
	{
		DrawText(TextFormat("GAME OVER!"),  (GetScreenWidth() / 2) - 150, (GetScreenHeight() / 2) - 50, 50, RED);
		DrawText(TextFormat("PRESS START TO RESTART!"), (GetScreenWidth() / 2) - 90, (GetScreenHeight() / 2), 15, RED);
	}
}

int main()
{
	srand(time(NULL));

	//INITIALIZATION
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Otherside");
	InitAudioDevice();
	World world = World::instance(); //Init world

	//Init player
	Vector2 vPlayerPos = { (double)screenWidth / 2, (double)screenHeight / 2 };
	Player* player = new Player("Player", vPlayerPos, { 200 ,200 }, 16, world, 1, world.playerTxt);
	world.worldList.appendToHead(player);
	world.player = player;

	PlayMusicStream(world.bgm);
	int score = 0;
	double fPlayerSpeed = 10.0f;
	double previous = GetTime();
	double FPS = 60.0;
	double MS_PER_UPDATE = 1.0 / FPS;
	double dt = 0.0;
	double cd = 0.0;
	bool pause = false;
	//--------------------------------------------------------------------------------------
	// Main game loop with vairable timestep
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		double current = GetTime();
		if (current >= previous + MS_PER_UPDATE) // Variable Timestep
		{
			dt += current - previous;
			previous = current;
			// Update
			//----------------------------------------------------------------------------------
			while (dt >= MS_PER_UPDATE)
			{
				Update(dt, world);
				dt -= MS_PER_UPDATE;
			}
			//----------------------------------------------------------------------------------
			// Draw
			//----------------------------------------------------------------------------------
			BeginDrawing();


			Draw(world);

			EndDrawing();
			//----------------------------------------------------------------------------------
		}
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

