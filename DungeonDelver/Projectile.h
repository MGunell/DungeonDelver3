#ifndef __Projectile__
#define __Projectile__

class BaseNpc;
class Player;
class Room;
class EnemyManager;
class Enemy;
#include "Player.h"
#include "BaseNpc.h"
#include "SDL.h"
#include "Ltexture.h"


class Projectile
{
public:
	Projectile(bool alive = false, double angle1 = 0, int x = -100, int y = -100, double VelX = 0, double VelY = 0, int damage1 = 25.0, int range1 = 5, int speed1 = 5, int mtype = 0);
	bool move(Room* room);
	bool enemyMove(Player& player);
	bool checkCollide(EnemyManager* eM);
	friend bool loadProjectileMedia(SDL_Renderer* gRenderer);
	void renderProjectile(SDL_Rect& camera, SDL_Renderer* gRenderer, Player* player, double rotation, int clips = 0);
	friend Projectile* makeProjectile();
	bool alive;
	double pposw, pposh;

	Projectile operator= (Projectile& target)
	{
		pposx = target.pposx, pposy = target.pposy;
		pposw = target.pposw, pposh = target.pposh;
		angle = target.angle;
		pVelX = target.pVelX, pVelY = target.pVelY;

		damage = target.damage;
		range = target.range;
		speed = target.speed;
		mType = target.mType;
		alive = target.alive;
		lifetime = target.lifetime;
		clip = target.clip;

		return *this;
	}


	bool checkHit(SDL_Rect enemy);
	bool checkHit(Enemy& enemy);
	double getAngle() { return angle; }
	void setAngle(double angle1) { angle = angle1; }
private:
	int damage;
	int range = 10;
	int speed = 3;
	int clip = 0;
	int aliveLength = 0;
	int frames = 0;

	double angle;

	double pVelX, pVelY;
	double pposx, pposy;
	
	int mType;
	int lifetime = range * 64;
};

static LTexture projectileSheets;
static SDL_Rect sprites1[16];

#endif