/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include "raylib.h"
#include "World.cpp"
#include "string"

/*
Entity class and all derived classes from it, as Player, Enemies, Bullets and PlayerAttack.
*/
class Entity
{
public:
	std::string id;
	Vector2 pos;
	Vector2 vel;
	double radius;
	bool collision;
	bool destroy;
	Vector2 dir;
	World* w;
	int damage;
	int hitPoint;
	Texture2D txt2d;
	Entity();
	Entity(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, Texture2D txt2d);
	~Entity();

	virtual void Update(double elapsedTime);

	void Draw();
	std::string CheckCollision();
};

class Player : public Entity
{
public:
	int attackCount = 0;
	double dashCd = 0.0;
	double attackCd = 0.0;
	double dashTimer = 0.0;
	double dashSpeed = 5;
	double attackRangeMultiplier = 5;

	Player();
	Player(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, int hP, Texture2D txt2d);

	void Update(double elapsedTime) override;
};

class Enemy : public Entity
{
public:
	int bulletCount = 0;
	double attackCd = 1.5;

	Enemy();
	Enemy(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, int hP, Texture2D txt2d);

	void Update(double elapsedTime) override;
};

class Bullet : public Entity 
{
public:

	Bullet();
	Bullet(std::string id, Vector2 pos, Vector2 vel, Vector2 dir, double radius, World& world, Texture2D txt2d);

	void Update(double elapsedTime) override;
};

class PlayerAttack : public Entity
{
public:
	double lifeTime = 0.05;

	PlayerAttack();
	PlayerAttack(std::string id, Vector2 pos, Vector2 vel, double radius, World& world, Texture2D txt2d);

	void Update(double elapsedTime) override;
};
