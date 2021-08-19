/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include "Entity.h"
#include<iostream>

Entity::Entity()
{
	id = "null";
	pos.x = 0;
	pos.y = 0;
	vel.x = 0;
	vel.y = 0;
	radius = 0.0;
	dir.x = 0;
	dir.y = 0;
	damage = 0;
	w = NULL;
	txt2d = Texture2D();
	collision = false;
	hitPoint = 1;
	destroy = false;
}

Entity::Entity(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, Texture2D txt2d) : id(id), pos(pos), vel(vel), radius(radius), collision(false), dir(Vector2{ 0.0f,0.0f }), w(&world), destroy(false), hitPoint(1), damage(0),
txt2d(txt2d) {}

Entity::~Entity() {} 

void Entity::Update(double elapsedTime)
{
	this->~Entity();
}

void Entity::Draw()
{
	DrawTextureEx(txt2d, { pos.x - (float)radius, pos.y - (float)radius }, 0,1, WHITE);
}

std::string Entity::CheckCollision()
{
	std::string strReturn = "";
	collision = true;

	return strReturn;
}

Player::Player()
{
}

Player::Player(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, int hP , Texture2D txt2d)
{
	Entity::id = id;
	Entity::pos = pos;
	Entity::vel = vel;
	Entity::radius = radius;
	Entity::dir = Vector2{ 1.0f,1.0f };
	Entity::w = &world;
	Entity::hitPoint = hP;
	Entity::txt2d = txt2d;
}

void Player::Update(double elapsedTime) //check this!
{
	/*if (hitPoint <= 0) //HP implementation is not beeing used
		destroy = true;*/
	dashCd -= elapsedTime;
	attackCd -= elapsedTime;
	if (dashTimer > 0) //Dash duration
	{
		vel.x = 1000;
		vel.y = 1000;
		dashTimer -= elapsedTime;
	}
	else
	{
		vel.x = 200;
		vel.y = 200;
	}

	//Player input handling
	//Controller
	dir.x = 0.f;
	dir.y = 0.f;

	if (abs(GetGamepadAxisMovement(GAMEPAD_PLAYER1, GAMEPAD_AXIS_LEFT_X)) > 0.1f)
		dir.x = (float)elapsedTime * vel.x * GetGamepadAxisMovement(GAMEPAD_PLAYER1, GAMEPAD_AXIS_LEFT_X);
	if (abs(GetGamepadAxisMovement(GAMEPAD_PLAYER1, GAMEPAD_AXIS_LEFT_Y)) > 0.1f)
		dir.y = (float)elapsedTime * vel.y * GetGamepadAxisMovement(GAMEPAD_PLAYER1, GAMEPAD_AXIS_LEFT_Y);

	//Keyboard
	if (IsKeyDown(KEY_RIGHT))
		dir.x = (float)elapsedTime * vel.x;
	if (IsKeyDown(KEY_LEFT))
		dir.x = (float)elapsedTime * vel.x * -1.f;
	if (IsKeyDown(KEY_UP))
		dir.y = (float)elapsedTime * vel.y * -1.f;
	if (IsKeyDown(KEY_DOWN))
		dir.y = (float)elapsedTime * vel.y;

	if (!(pos.x + dir.x > GetScreenWidth() || pos.x + dir.x < 0)) // X axis move
		pos.x += dir.x;

	if (!(pos.y + dir.y > GetScreenHeight() || pos.y + dir.y < 0)) // Y axis move
		pos.y += dir.y;

	if ((IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_S)) && dashCd <= 0) //Dash input
	{
		dashCd = 1.0;
		dashTimer = 0.05;
	}

	if ((IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_A)) && attackCd <= 0) //Attack input
	{
		//Create a new PlayerAttack and append to WorldList. Spawn it at a distance relative to player speed
		std::string attackId = "PA";
		float axisX = dir.x * attackRangeMultiplier;
		float axisY = dir.y* attackRangeMultiplier;
		w->worldList.appendToHead(new PlayerAttack(attackId, { pos.x + axisX, pos.y + axisY}, { 100 ,100 }, 32, *w, w->attackTxt));
		attackCount++;
		attackCd = 1.0;
	}

}


Enemy::Enemy()
{
	hitPoint = 0;
}

Enemy::Enemy(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, int hP, Texture2D txt2d)
{
	Entity::id = id;
	Entity::pos = pos;
	Entity::vel = vel;
	Entity::radius = radius; 
	Entity::w = &world;
	Entity::damage = 0;
	Entity::hitPoint = hP;
	Entity::txt2d = txt2d;
}

void Enemy::Update(double elapsedTime)
{
	attackCd -= elapsedTime;

	/*
		Get a vector between player and enemy and normalize it using a numerical method.
	*/

	float dx = w->player->pos.x - pos.x;
	float dy = w->player->pos.y - pos.y;
	// Get absolute value of each vector
	float ax = abs(dx);
	float ay = abs(dy);

	// Create a ratio
	float ratio = 1 / fmax(ax, ay);
	ratio = ratio * (1.29289f - (ax + ay) * ratio * 0.29289f);

	// Multiply by ratio
	dx = dx * ratio;
	dy = dy * ratio;

	// Enemy always run away from player
	dir.x = -1 * dx * elapsedTime * 10;
	dir.y = -1 * dy * elapsedTime * 10;
	if (!(pos.x + dir.x > GetScreenWidth() || pos.y + dir.y > GetScreenHeight() || pos.x + dir.x < 0 || pos.y + dir.y < 0)) //Don't let it leave screen area
	{
		pos.x += dir.x;
		pos.y += dir.y;
	}

	//Spawn bullet using going foward player position.
	if (attackCd <= 0)
	{
		std::string attackId = "BU";
		w->worldList.appendToHead(new Bullet(attackId, { pos.x, pos.y}, { 100, 100 }, { dx, dy }, 4, * w, w->bulletTxt));
		bulletCount++;
		attackCd = 1.5;
	}


}

Bullet::Bullet()
{}

Bullet::Bullet(std::string id, Vector2 pos, Vector2 vel, Vector2 dir, double radius, World& world, Texture2D txt2d)
{
	Entity::id = id;
	Entity::pos = pos;
	Entity::vel = vel;
	Entity::radius = radius;
	Entity::w = &world;
	Entity::dir = dir;
	Entity::damage = 1;
	Entity::txt2d = txt2d;
}

void Bullet::Update(double elapsedTime)
{
	pos.x += elapsedTime * dir.x * vel.x;
	pos.y += elapsedTime * dir.y * vel.y;
	if (pos.x > GetScreenWidth() || pos.y > GetScreenHeight() || pos.x < 0 || pos.y < 0) //Destroy it if it hits screen border
	{
		destroy = true;
	}
}

PlayerAttack::PlayerAttack() {}

PlayerAttack::PlayerAttack(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, Texture2D txt2d)
{
	Entity::id = id;
	Entity::pos = pos;
	Entity::vel = vel;
	Entity::radius = radius;
	Entity::w = &world;
	Entity::txt2d = txt2d;
	Entity::damage = 1;
}

void PlayerAttack::Update(double elapsedTime)
{
	collision = false;
	lifeTime -= elapsedTime;
	if (lifeTime <= 0)
	{
		destroy = true;
	}
}