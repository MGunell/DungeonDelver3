#include "Engine.h"
#include "InventoryRedux.h"
#include "HyperCube.h"
#include "Equipment_Inventory.h"
#include <math.h>
#include "Weapon.h"
#include "Bow.h"
#include "Sword.h"
#include "RPotion.h"
#include "Level_Creation.h"
#include "Room.h"
#include "EnemyManager.h"
#include "LevelGenerator.h"
#include "Level.h"
#include "Enemy.h"
#include <time.h>
#include "Core.h"
//#include "Wisp.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

bool loadTextBoxMedia(SDL_Renderer* gRenderer)
{
	bool success = true;
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("sdlimag failure");
		success = false;
	}

	//initialize sdl ttf
	if ( TTF_Init() == -1 )
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		//success = false;
	}

	gFont = TTF_OpenFont("fonts/lazy.ttf", 45);
	textColor = { 255, 255, 255 };
	if (gFont == NULL) {
		printf("Failed to load the font, SDL_TTF error: %s", TTF_GetError());
	}

	if (!dialogue.loadFromRenderedText("Level: 1", textColor, gRenderer, gFont))
	{
		printf("fucked up with word texture");
		//success = false;
	}

	textColor = { 0, 0, 0 };
	gFont = TTF_OpenFont("fonts/lazy.ttf", 90);
	if (!endMessage.loadFromRenderedText("You Lost", textColor, gRenderer, gFont))
	{
		printf("end message failed to load");
	}

	return success;
}

bool loadMusicMedia(SDL_Renderer* gRenderer)
{
	bool success = true;

	//gMusic = Mix_LoadMUS("fonts/jupiter.wav");

	return success;
}

void initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0)
	{
		printf("SDL could not initialize");
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else
	{
		window = SDL_CreateWindow("My first sdl window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window failed to load");
		}
		else
		{
			gRenderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_PRESENTVSYNC | SDL_SWSURFACE);
		}
	}

	loadItemMedia(gRenderer);
}
bool firstscript = true;
void startgame()
{
	
	
	initSDL();
	printf("we looped");
	loadTextBoxMedia(gRenderer);
	loadMusicMedia(gRenderer);
	printf("we looped");
	int frame = 0;
	SDL_Rect RECT;
	RECT.x = 0;
	RECT.y = 0;
	RECT.w = 50;
	RECT.h = 50;
	printf("we looped");
	SDL_Rect camera = { 0,0,   SCREEN_WIDTH,SCREEN_HEIGHT };
	//Mix_PlayMusic(gMusic, -1);
	while (firstscript == true)
	{
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		{
			
			SDL_RenderClear(gRenderer);
			 
			dialogue.render(0, 0, NULL, gRenderer);
			
			SDL_RenderPresent(gRenderer);
			
		}
	}
}

void startMapEditor()
{
	loadTextBoxMedia(gRenderer);
	
	Selected selector;
	initSDL();
	Tile* tileSet[TOTAL_TILES + 18];
	Tile* mutableTile[11];
	int padding = 10;
	int tileS = 128;
	int gOff = 0;
	for (int i = 0; i < 11; i++)
	{
		mutableTile[i] = new Tile{ (SCREEN_WIDTH/2 - 128 + padding * i + tileS * i), 11, i };
	}
	//Tile mutableTile = { 0, 11, selector.m_selected };
	int  tileNumbers = 10;
	//std::string levelname;
	//std::cin >> levelname;
	loadLevelOneMedia(tileSet, gRenderer, "maps/startTown.map");
	int gX, gY;
	
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	SDL_Event e;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	int frame = 0;
	SDL_Rect* currentClip; SDL_Color textColor = { 0, 0, 255 };
	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				//quit == true;
			}
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_q: 
					gOff += tileS + padding;
					break;
				case SDLK_e:
					gOff -= tileS + padding;
				}
			}
			selector.handleEvent(e, tileSet, camera);
		}
		SDL_GetMouseState(&gX, &gY);
		gX = (gX + camera.x) / 128;
		gY = (gY + camera.y) / 128;
		SDL_Rect tileBox = { (gX * 128) - camera.x , (gY * 128) - camera.y, 128, 128 };
		SDL_Rect tileBox1 = { SCREEN_WIDTH / 2 - 133, 6, 138, 138 };
		SDL_Rect UIBox = { 0, 0, SCREEN_WIDTH, 150 };

		for (int j = 0; j < 11; j++)
		{
			mutableTile[j]->mBox.x = (SCREEN_WIDTH / 2 - 128 + padding * j + tileS * j) + camera.x + gOff;
			mutableTile[j]->mBox.y = 11 + camera.y;
		}
		//mutableTile[i] = {SCREEN_WIDTH / 2 - 128 + camera.x, 11 + camera.y, selector.m_selected };
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		SDL_RenderClear(gRenderer);
		gTileTexture.renderHalf(0, 0, &gLevelOneClips[selector.m_selected], gRenderer);

		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//tileSet[i]->render(camera, gRenderer);
		}
		SDL_RenderDrawRect(gRenderer, &tileBox);
		SDL_SetRenderDrawColor(gRenderer, 0x128, 0x64, 0x156, 255);
		SDL_RenderFillRect(gRenderer, &UIBox);
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

		SDL_RenderFillRect(gRenderer, &tileBox1);
		for (int x = 0; x < 11; x++)
		//mutableTile[x]->render(camera, gRenderer);
		


		//gTileTexture.renderHalf(0, 0, &gLevelOneClips[selector.m_selected], gRenderer);

		
		//system("CLS");
		
		//dialogue.loadFromRenderedText("The quic brown fox jumps over the lazy dog", textColor, gRenderer, gFont);
		//dialogue.render(0, 0, currentClip, gRenderer, NULL, 0, flipType);
		SDL_RenderPresent(gRenderer);
	}
}

/*{
	template <typename T>
	class vec3d<T>
	{
	public:

		vec3d();
		vec3d(T x, T y, T z) : x(x), y(y), z(z) {}
		vec3d(const vec3d& vect)
		{
			x = vect.x;
			y = vect.y;
			z = vect.z;
		}

		T x, y, z;

		vec3d& operator=(const vec3d &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}
		T operator*(const vec3d &rhs)
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		T LenSq() const {
			return x * x + y * y + z * z;
		}
		T Len() const { return sqrt(LenSq() }
	};

	template <typename T>
	class Mat3
	{
	public:
		Mat3 & operator*=(T rhs)
		{

		}
		Mat3 & operator*(T rhs) const
		{
			Mat3 result = *this;
			return result *= rhs;
		}
		Mat3 operator*(const Mat3& rhs) const
		{
			Mat3 result;
			for (size_t j = 0; j < 3; j++)
			{
				for (size_t k = 0; k < 3; k++)
				{
					T sum = (T)0.0;
					for (size_t i = 0; i < 3; i++)
					{
						sum += elements[j][i] * rhs.elements[i][k];
					}
					result.elements[j][k] = sum;
				}
			}
			return result;
		}
		static Mat3 Identity()
		{
			return {
				(T)1.0, (T)0.0, (T)0.0,
				(T)0.0, (T)1.0, (T)0.0,
				(T)0.0, (T)0.0, (T)1.0
			};
		}
		static Mat3 scaling(T factor)
		{
			return {
				factor, (T)0.0, (T)0.0,
				(T)0.0, factor, (T)0.0,
				(T)0.0, (T)0.0, factor
			};
		}
	public:
		T elements[3][3];
	};

	template <typename T>
	class _3dSpace
	{
	public:
		_3dSpace()
			:
			xFactor(float(SCREEN_WIDTH) / 2.0f),
			yFactor(float(SCREEN_HEIGHT) / 2.0f)
		{}
		vec3d<T>& Transform(vec3d<T>& v) const
		{
			v.x = (v.x + 1.0f) * xFactor;
			v.y = (v.y + 1.0f) * yFactor;
			return v;
		}
		vec3d<T> getTransformed(const vec3d<T>& v) const
		{
			return Transform(vec3d(v));
		}
	private:
		float xFactor;
		float yFactor;

	};

	template <typename T>
	vec3d operator*(const vec3d<T>& lhs, const mat3d<T>& rhs)
	{
		return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
		};
	}

	void loadLevelMesh()
	{






	};

	void matrixMult(vec3d matrix)
	{

	}
}*/

void polygon3D()
{
	initSDL();

}

void levelOn11e()
{
	initSDL();
	Inventory3 newInv2((SCREEN_WIDTH - (15 * (64 + 5)))/2, SCREEN_HEIGHT - 100, 15, 1);
	Inventory3 newInv( 100, 200, 4, 5);
	LTexture boxSprite;
	boxSprite.loadFromFile("images/boxSprite.png", gRenderer);
	newInv.setBoxTexture(&boxSprite);
	newInv2.setBoxTexture(&boxSprite);
	Player player;
	player.hotbar = new Inventory3((SCREEN_WIDTH - (15 * (64 + 5))) / 2, SCREEN_HEIGHT - 100, 15, 1);
	player.boxSprite.loadFromFile("images.boxSprite.png", gRenderer);
	player.hotbar->setBoxTexture(&boxSprite);
	Bow bow1(100, 100, 1, 0, gRenderer, 0, 10, 16);
	Bow bow2(200, 100, 1, 1, gRenderer, 0, 10, 16);
	Bow bow3(300, 100, 1, 2, gRenderer, 0, 10, 16);
	RPotion smallPotion( 300, 100, 25, 0, gRenderer);
	RenderableManager r;
	r.addRenderable(&bow1);
	r.addRenderable(&bow2);
	r.addRenderable(&smallPotion);
	SDL_Rect camera = { 0, 0, 1200, 900 };
	
	int mx, my;
	int mmy, mmx;
	bool clicking = true;
	SDL_Event e;
	while (true)
	{
		SDL_GetMouseState(&mx, &my);
		SDL_GetRelativeMouseState(&mmx, &mmy);
		mx += mmx*2;
		my += mmy*2;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				//quit == true;
			}
			if (clicking)
			{
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_o:
						newInv.addItem(&bow1);
						break;
					case SDLK_i:
						newInv.addItem(&bow2);
						break;
					case SDLK_k:
						newInv.addItem(&smallPotion);
						break;
					case SDLK_l:
						clicking = false;
						break;
					
					}
				}

				if (e.wheel.y == 1)
				{
					if (newInv.selectedBox < newInv.capacity - 1)
						newInv.selectedBox += 1;
				}
				if (e.wheel.y == -1)
				{
					if (newInv.selectedBox > 0)
						newInv.selectedBox -= 1;
				}
			}
			else
			{
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_o:
						newInv2.addItem(&bow1);
						break;
					case SDLK_i:
						newInv2.addItem(&bow2);
						break;
					case SDLK_k:
						newInv2.addItem(&smallPotion);
						break;
					case SDLK_l:
						clicking = true;
						break;
					}
				}
				if (e.wheel.y == 1)
				{
					if (newInv2.selectedBox < newInv2.capacity - 1)
						newInv2.selectedBox += 1;
				}
				if (e.wheel.y == -1)
				{
					if (newInv2.selectedBox > 0)
						newInv2.selectedBox -= 1;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0)
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					newInv.grabIitem(newInv.getBoxCollision());
					std::cout << newInv.getBoxCollision();
					break;
				case SDL_BUTTON_RIGHT:
					break;
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP && e.key.repeat == 0)
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					newInv.releaseItem();

					break;
				case SDL_BUTTON_RIGHT:
					break;
				}
			}
			
		}

		SDL_RenderClear(gRenderer);
		
		//newInv.render(gRenderer, mx, my);
		//newInv2.render(gRenderer, mx, my);
		player.render(camera, 0, gRenderer, 0, 0, SDL_FLIP_NONE, 0);
		//r.renderAll(gRenderer, player.hotbar);

		SDL_RenderPresent(gRenderer);
	}
}

void le567velOne()
{
	initSDL();
	SDL_Rect rooms[4];
	LTexture src;
	src.loadFromFile("images/tiles.png", gRenderer);
	rooms[0].x = 10;
	rooms[0].y = 0;
	rooms[0].w = 20;
	rooms[0].h = 10;

	rooms[1].x = 20;
	rooms[1].y = 10;
	rooms[1].w = 20;
	rooms[1].h = 6;

	rooms[2].x = 30;
	rooms[2].y = 16;
	rooms[2].w = 31;
	rooms[2].h = 30;

	rooms[3].x = 00;
	rooms[3].y = 46;
	rooms[3].w = 32;
	rooms[3].h = 2;
	Room room1(rooms, 4);
	if (!gTileTexture.loadFromFile("images/tiles.png", gRenderer))
		std::cout << "problem";
	
	setTiles_();
	//std::cout << room1.wallsCapacity << std::endl;
	for (int i = 0; i < room1.wallsCapacity; i++)
	{
		std::cout << room1.walls[i].getType() << std::endl;
	}

	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_Event e;
	
	
	//room1.printLines(gRenderer, camera);
	//std::cin.get();
	while (true)
	{

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					camera.x -= 150;
					break;
				case SDLK_d:
					camera.x += 150;
					break;
				case SDLK_w:
					camera.y -= 150;
					break;
				case SDLK_s:
					camera.y += 150;
					break;
				}
			}
		}

		SDL_RenderClear(gRenderer);

		//room1.render(gRenderer, camera, src);

	//	gTileTexture.renderHalf(25 - camera.x, 25 - camera.y, &gLevelOneClips[2], gRenderer);
		SDL_RenderPresent(gRenderer);
	}
}
void levelOn1e()
{
	SDL_Rect camera = { 0, 0, 600, 600 };
	SDL_Event e;
	LevelMap map(5, 5);
	LevelMap map1(5, 5);
	Tile* tiles[27 * 27];
	//for (int i = 0; )
	map.randomizeMapGen();
	map = map.build();
	map = map.addBorders();
	map.print();
    map1 = map.makeWallMap(map);
	//map1 = map1.getGround(map1);
	//map1.cull(map1);
	map1.print();

	initSDL();

	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					camera.x -= 150;
					break;
				case SDLK_d:
					camera.x += 150;
					break;
				case SDLK_w:
					camera.y -= 150;
					break;
				case SDLK_s:
					camera.y += 150;
					break;
				}
			}
		}


		




		map.render(gRenderer, camera);
		
	}

}

void levelOne()
{


//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int cLevel(0);
	time_t timer;
	double seconds;
	time_t timer2;
	Inventory inventory;
	initSDL();
	loadPotionMedia(gRenderer);
	if (!gCubeTexture1.loadFromFile("images/hypercube.png", gRenderer))
	{
		printf("couldnt load slime stuff");
		
	}
	else
	{
		cubeSprites[0].x = 0;
		cubeSprites[0].y = 0;
		cubeSprites[0].h = 32;
		cubeSprites[0].w = 32;

		cubeSprites[1].x = 32;
		cubeSprites[1].y = 0;
		cubeSprites[1].h = 32;
		cubeSprites[1].w = 32;

		cubeSprites[2].x = 64;
		cubeSprites[2].y = 0;
		cubeSprites[2].h = 32;
		cubeSprites[2].w = 32;

		cubeSprites[3].x = 96;
		cubeSprites[3].y = 0;
		cubeSprites[3].h = 32;
		cubeSprites[3].w = 32;

		cubeSprites[4].x = 128;
		cubeSprites[4].y = 0;
		cubeSprites[4].h = 32;
		cubeSprites[4].w = 32;

		cubeSprites[5].x = 160;
		cubeSprites[5].y = 0;
		cubeSprites[5].h = 32;
		cubeSprites[5].w = 32;

		cubeSprites[6].x = 192;
		cubeSprites[6].y = 0;
		cubeSprites[6].h = 32;
		cubeSprites[6].w = 32;

		cubeSprites[7].x = 224;
		cubeSprites[7].y = 0;
		cubeSprites[7].h = 32;
		cubeSprites[7].w = 32;

		cubeSprites[8].x = 256;
		cubeSprites[8].y = 0;
		cubeSprites[8].h = 32;
		cubeSprites[8].w = 32;

		cubeSprites[9].x = 0;
		cubeSprites[9].y = 32;
		cubeSprites[9].h = 32;
		cubeSprites[9].w = 32;

		cubeSprites[10].x = 32;
		cubeSprites[10].y = 32;
		cubeSprites[10].h = 32;
		cubeSprites[10].w = 32;

		cubeSprites[11].x = 64;
		cubeSprites[11].y = 32;
		cubeSprites[11].h = 32;
		cubeSprites[11].w = 32;

		cubeSprites[12].x = 96;
		cubeSprites[12].y = 32;
		cubeSprites[12].h = 32;
		cubeSprites[12].w = 32;

		cubeSprites[13].x = 128;
		cubeSprites[13].y = 32;
		cubeSprites[13].h = 32;
		cubeSprites[13].w = 32;

		cubeSprites[14].x = 160;
		cubeSprites[14].y = 32;
		cubeSprites[14].h = 32;
		cubeSprites[14].w = 32;

		cubeSprites[15].x = 192;
		cubeSprites[15].y = 32;
		cubeSprites[15].h = 32;
		cubeSprites[15].w = 32;

		cubeSprites[16].x = 224;
		cubeSprites[16].y = 32;
		cubeSprites[16].h = 32;
		cubeSprites[16].w = 32;

		cubeSprites[17].x = 256;
		cubeSprites[17].y = 32;
		cubeSprites[17].h = 32;
		cubeSprites[17].w = 32;

		cubeSprites[18].x = 0;
		cubeSprites[18].y = 64;
		cubeSprites[18].h = 32;
		cubeSprites[18].w = 32;

		cubeSprites[19].x = 32;
		cubeSprites[19].y = 64;
		cubeSprites[19].h = 32;
		cubeSprites[19].w = 32;

		cubeSprites[20].x = 64;
		cubeSprites[20].y = 64;
		cubeSprites[20].h = 32;
		cubeSprites[20].w = 32;

		cubeSprites[21].x = 96;
		cubeSprites[21].y = 64;
		cubeSprites[21].h = 32;
		cubeSprites[21].w = 32;

		cubeSprites[22].x = 128;
		cubeSprites[22].y = 64;
		cubeSprites[22].h = 32;
		cubeSprites[22].w = 32;

		cubeSprites[23].x = 160;
		cubeSprites[23].y = 64;
		cubeSprites[23].h = 32;
		cubeSprites[23].w = 32;

		cubeSprites[24].x = 192;
		cubeSprites[24].y = 64;
		cubeSprites[24].h = 32;
		cubeSprites[24].w = 32;

		cubeSprites[25].x = 224;
		cubeSprites[25].y = 64;
		cubeSprites[25].h = 32;
		cubeSprites[25].w = 32;

		cubeSprites[26].x = 256;
		cubeSprites[26].y = 64;
		cubeSprites[26].h = 32;
		cubeSprites[26].w = 32;

	}

	{
	UIBoxes[0].x = 130;
	UIBoxes[0].y = 0;
	UIBoxes[0].w = 46;
	UIBoxes[0].h = 32;

	UIBoxes[1].x = 177;
	UIBoxes[1].y = 0;
	UIBoxes[1].w = 32;
	UIBoxes[1].h = 46;

	UIBoxes[2].x = 130;
	UIBoxes[2].y = 33;
	UIBoxes[2].w = 46;
	UIBoxes[2].h = 32;

	UIBoxes[3].x = 210;
	UIBoxes[3].y = 0;
	UIBoxes[3].w = 32;
	UIBoxes[3].h = 46;

	UIBoxes[4].x = 130;
	UIBoxes[4].y = 66;
	UIBoxes[4].w = 46;
	UIBoxes[4].h = 32;

	UIBoxes[5].x = 177;
	UIBoxes[5].y = 47;
	UIBoxes[5].w = 32;
	UIBoxes[5].h = 46;

	UIBoxes[6].x = 130;
	UIBoxes[6].y = 99;
	UIBoxes[6].w = 46;
	UIBoxes[6].h = 32;

	UIBoxes[7].x = 210;
	UIBoxes[7].y = 47;
	UIBoxes[7].w = 32;
	UIBoxes[7].h = 46;
	}

	UIAssets.loadFromFile("images/UIAssets.png", gRenderer);
	
	LTexture CubeTexture;
	CubeTexture.loadFromFile("images/hypercube.png", gRenderer);

	loadTextBoxMedia(gRenderer);
	LTexture boxsprite;
	SDL_Rect something = { 100, 100, 25, 25 };
		LTexture ogre;
		SDL_Color textColor1 = { 255, 255, 255 };
		TTF_Font* gFont2 = TTF_OpenFont("fonts/lazy.ttf", 15);
		ogre.loadFromRenderedText("Ogre", textColor1, gRenderer, gFont2);
		ogre.render(100, 100, &something, gRenderer);

	const int Slimes = 5;
	BaseNpc* basenpc[Slimes];
	//basenpc[0] = new BaseNpc(400, 400);
	//basenpc[1] = new BaseNpc(200, 200);
	basenpc[0] = new BaseNpc(1900, 700);
	basenpc[1] = new BaseNpc(1700, 800);
	basenpc[2] = new BaseNpc(1800, 700);
	basenpc[3] = new BaseNpc(1900, 1400);
	basenpc[4] = new BaseNpc(2300, 1400);

	//BossSlime Boss(500, 100);
	//BaseNpc slime1(400, 400);
	//BaseNpc slime2(200, 200);
	Player player;
	boxsprite.loadFromFile("images/boxSprite.png", gRenderer);
	player.hotbar = new Inventory3(SCREEN_WIDTH+25, 100, 5, 6);
	player.hotbar->setBoxTexture(&boxsprite);
	EnemyManager eM;
	LevelGenerator LG;
	//eM.insertEnemy(basenpc[0]);
	//eM.insertEnemy(basenpc[1]);
	//eM.insertEnemy(basenpc[2]);
	//eM.insertEnemy(basenpc[3]);
	//eM.insertEnemy(basenpc[4]);
	RenderableManager r;
	// Inventory2 inventory2(gRenderer);
	//r.addRenderablePotion(200, 300, 25, gRenderer);
	//r.addRenderablePotion(300, 450, 25, gRenderer);
	//r.addRenderableManaPotion(400, 250, 25, gRenderer);
	//inventory2.placePotion(gRenderer);
	//inventory2.placeWeapon(gRenderer, 1, 19, 0, 20, 10);
	/*inventory2.placeWeapon(gRenderer, 1, 18, 1, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 2, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 3, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 4, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 5, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 6, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 7, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 8, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 9, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 10, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 11, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 12, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 13, 35, 10);
	inventory2.placeWeapon(gRenderer, 1, 19, 14, 20, 10);
	inventory2.placeWeapon(gRenderer, 1, 18, 15, 35, 10);*/ 



	SDL_Rect rooms[4];
	//SDL_Rect rooms[2];
	LTexture src;
	src.loadFromFile("images/tiles.png", gRenderer);
	rooms[0].x = 10;
	rooms[0].y = 0;
	rooms[0].w = 20;
	rooms[0].h = 10;

	rooms[1].x = 20;
	rooms[1].y = 10;
	rooms[1].w = 20;
	rooms[1].h = 6;

	rooms[2].x = 30;
	rooms[2].y = 16;
	rooms[2].w = 31;
	rooms[2].h = 30;

	rooms[3].x = 0;
	rooms[3].y = 46;
	rooms[3].w = 32;
	rooms[3].h = 2;
	//Room room1(rooms, 4);

	setTiles_();

	Room* temp = NULL;
	temp = LG.rooms[0];

	CoreManager cM;


	CoreFactory coref;
	coref.init(gRenderer);

	Core* coretemp = coref.makeCore(1, 800, 800);
	Core* coretemp1 = coref.makeCore(2, 700, 800);
	Core* coretemp2 = coref.makeCore(3, 600, 800);
	Core* coretemp3 = coref.makeCore(4, 500, 800);
	Core* coretemp4 = coref.makeCore(0, 400, 800);
	cM.addTail(coretemp);
	cM.addTail(coretemp1);
	cM.addTail(coretemp2);
	cM.addTail(coretemp3);
	cM.addTail(coretemp4);
	cM.addTail(coref.makeCore(5, 600, 600));

	Enemy firstEnemy(700, 700, 32);
	hyperCubeBoss Boss(1600, 1600, 64);
	HCBpawn pawn_0(-999, -999, 32);
	HCBpawn pawn_1(-999, -999, 32);
	HCBpawn pawn_2(-999, -999, 32);
	HCBpawn pawn_3(-999, -999, 32);
//	eM.insertEnemy(&firstEnemy);
//	firstEnemy.setTexture(&ogre);
//	firstEnemy.setAgro(&player);

	pawn_0.setNameTexture(&ogre);
	pawn_0.setAgro(&player);

	pawn_1.setNameTexture(&ogre);
	pawn_1.setAgro(&player);

	pawn_2.setNameTexture(&ogre);
	pawn_2.setAgro(&player);

	pawn_3.setNameTexture(&ogre);
	pawn_3.setAgro(&player);

	eM.insertEnemy(&Boss);
	Boss.setNameTexture(&ogre);
	Boss.setAgro(&player);
	Boss.setPawn(&pawn_0, 0);
	Boss.setPawn(&pawn_1, 1);
	Boss.setPawn(&pawn_2, 2);
	Boss.setPawn(&pawn_3, 3);
	Boss.setEM(&eM);

	////it goes in x, y, slot, sprite, projectilesprite, damage, range projectiles spd
	
	Bow* bow1 = new Bow(100, 100, 1, 0, gRenderer, 0, 30, 6, 1, 6);
	Bow* bow2 = new Bow(100, 100, 1, 1, gRenderer, 1, 10, 6, 2, 6);
	Bow* bow3 = new Bow(100, 100, 1, 2, gRenderer, 2, 10, 6, 3, 6);
	Bow* bow4 = new Bow(100, 100, 1, 3, gRenderer, 3, 40, 6, 1, 6);
	Sword* sword = new Sword(100, 100, 1, 3, gRenderer, 5, 60, 1, 3, 2);
	//r.addRenderable(&bow4);
	//bow1.placeInBag(&inventory2, gRenderer);
	//bow2.placeInBag(&inventory2, gRenderer);
	//bow3.placeInBag(&inventory2, gRenderer);
	//bow4.placeInBag(&inventory2, gRenderer);

	//inventory.placePotion(1);
	//inventory.placePotion(2);
	//r.addRenderable(&bow1);
	//Tile* tileSet[TOTAL_TILES+18];
//	loadLevelOneMedia(tileSet, gRenderer, "maps/levelone.map");
	loadPlayerMedia(gRenderer);
	loadSlimeMedia(gRenderer);
	loadProjectileMedia(gRenderer);
	
	SDL_Rect camera = {0,0,   SCREEN_WIDTH,SCREEN_HEIGHT};
	SDL_Rect largeChat = { 20, 152, 12, 8 };
	SDL_Event e;

	EquipInv equipInv(SCREEN_WIDTH, 50);
	//equipInv.equips[1] = new Bow(100, 100, 1, 0, gRenderer, 0, 10, 6);
	equipInv.loadSprite("images/equipinv.png", gRenderer);

	ProjectileManager p;
	ProjectileManager enemyP;

	Boss.setPM(&enemyP);
	pawn_0.setPM(&enemyP);
	pawn_1.setPM(&enemyP);
	pawn_2.setPM(&enemyP);
	pawn_3.setPM(&enemyP);

	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	int frame = 0;
	SDL_Rect* currentClip; 
	player.hotbar->addItem(bow4);
	player.hotbar->addItem(sword);
	player.setWeapon(sword);
	eM.renderNodes();

	Level level_1(&eM, &player);
	//loadinvUIassets(gRenderer);
	int starting = 0;

	//Now we are going to do some engine changes
	//primarily what i want is for the inventory to pause the normal game loop, while still rendering everything behind the screen

	while (true)
	{
		//time(&timer);


		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_p:
					starting += 4;
					break;
				case SDLK_o:
					starting -= 4;
					break;
				case SDLK_m:
					RenderableManager * temp = &r;
					delete temp;
				}
			}
			player.handleEvent(e);
			equipInv.handleEvent(e);
			//inventory2.handleEvent(e);
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		//equipInv.handleMovement(player, inventory2, gRenderer);
		//inventory2.handleMovement(player, equipInv, gRenderer);

		if (player.inventoryOpen == false)
		{
			SDL_RenderClear(gRenderer);

			eM.moveAll(player, &level_1, enemyP, cM, gRenderer, coref);
			player.update(&level_1, &p, &camera, &equipInv, gRenderer, eM, r);

			player.setCamera(camera);
			//temp->render(gRenderer, camera, src, &player);
			level_1.renderNodes(camera, gRenderer, src);

			//firstEnemy.render(gRenderer, camera);
			//firstEnemy.renderInfo(gRenderer, camera);
			eM.renderAll(camera, gRenderer, temp, &player);

			p.moveAll(camera, gRenderer, &eM, temp, &player);
			enemyP.moveAllEnemy(camera, gRenderer, player, level_1._Level->rooms[0]);

			p.renderAll(camera, gRenderer, player, level_1._Level->rooms[0]);
			enemyP.renderAll(camera, gRenderer, player, level_1._Level->rooms[0]);
			currentClip = player.getClip();

			//inventory2.renderAll(camera, gRenderer);
			//equipInv.render(camera, gRenderer);
			//equipInv.renderModularItems(player, gRenderer, camera);
			//player.render(camera, currentClip, gRenderer, 0, 0, flipType, 0);

			//temp->renderWalls(gRenderer, camera, src, &player);

			//room1.renderWalls(gRenderer, camera, src, &player );
			player.renderBars(gRenderer);
			player.renderBow(gRenderer, camera);
			//temp->renderMiniMap(gRenderer, &player);
			//room1.renderMiniMap(gRenderer, &player);
			r.updateAll(gRenderer, player.hotbar, &player);
			r.renderAll(gRenderer, camera);
			//player.render(camera, currentClip, gRenderer, NULL, 0, flipType, &largeChat);


			//SDL_RenderDrawLine(gRenderer, firstEnemy.x - camera.x, firstEnemy.y - camera.y, player.x - camera.x, player.y - camera.y);
			if (false)// && basenpc[1]->getAlive() && basenpc[2]->getAlive())
			{
				//this means a level was cleared. we should call this after all render loops
				//now the player can level up and we can add more enemies
				//player.LevelUp();
				cLevel++;
				dialogue.loadFromRenderedText("Level: " + std::to_string(player.getLevel()), textColor, gRenderer, gFont);
				/*for (int w = 0; w < Slimes; w++)
				{
					delete basenpc[w];
				}*/
				//delete basenpc;
				if (cLevel > 0 && cLevel % 3 == 0)
				{

					basenpc[0] = new BaseNpc(1800, 700);
					basenpc[1] = new HyperCube(1700, 800);
					basenpc[2] = new BossSlime(1900, 700);
					//eM.insertEnemy(basenpc[0]);
					//eM.insertEnemy(basenpc[1]);
					//eM.insertEnemy(basenpc[2]);
				}
				else
				{

					basenpc[0] = new BaseNpc(1800, 700);
					basenpc[1] = new HyperCube(1700, 800);
					basenpc[2] = new BossSlime(1900, 700);
					//eM.insertEnemy(basenpc[0]);
					//eM.insertEnemy(basenpc[1]);
					//eM.insertEnemy(basenpc[2]);
				}
			}
			//	coretemp->render(gRenderer, camera);
			cM.render(gRenderer, camera, &player);
			player.incrementFrames();
			//system("CLS");
			//std::cout << player.getLevel();
			//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			//dialogue.loadFromRenderedText("Coins x " + std::to_string(player.coins), textColor, gRenderer, gFont);
			//dialogue.render((SCREEN_WIDTH - dialogue.getWidth())/2, 30, NULL, gRenderer, NULL, 0, flipType);

			//gCubeTexture1.render(5, 5, &cubeSprites[1], gRenderer);
			//CubeTexture.render(5, 5, &cubeSprites[1], gRenderer);
			//time(&timer2);
			//seconds = difftime(timer, timer2);

			//std::cout << seconds << '\n';
			//endMessage.loadFromRenderedText(std::to_string(60.0 / seconds), textColor, gRenderer, gFont);
			//endMessage.render((SCREEN_WIDTH - endMessage.getWidth()) / 2, SCREEN_HEIGHT / 2, NULL, gRenderer, NULL, 0, flipType);
			SDL_RenderPresent(gRenderer);


			if (player.getHealth() <= 0)
				break;
		}
		else if (player.inventoryOpen == true)
		{
			
			SDL_RenderClear(gRenderer);

			level_1.renderNodes(camera, gRenderer, src);

			eM.renderAll(camera, gRenderer, temp, &player);
			p.renderAll(camera, gRenderer, player, level_1._Level->rooms[0]);
			enemyP.renderAll(camera, gRenderer, player, level_1._Level->rooms[0]);
			currentClip = player.getClip();

			

			player.renderBars(gRenderer);
			player.renderBow(gRenderer, camera);
			
			r.renderAll(gRenderer, camera);
		
			cM.render(gRenderer, camera, &player);
			player.incrementFrames();
			player.update(&level_1, &p, &camera, &equipInv, gRenderer, eM, r);

			

			SDL_RenderPresent(gRenderer);
		}
	}
	
	SDL_RenderPresent(gRenderer);
	//std::cin.get();
}

bool checkCollision(SDL_Rect a, SDL_Rect b, int offsety = 0)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y+offsety;
	bottomB = b.y + b.h+offsety;

	if (bottomA <= topB)
	{
		return false;
	}
	if (topA >= bottomB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

bool touchesWall(SDL_Rect box, Room* room, int offsety)
{
	//go through the tiles
	for (int i = 0; i < room->wallsCapacity; i++)
	{
		if (((room->walls[i].getType() > 0) && (room->walls[i].getType() < 14)))
		{
			//check collision box
			if (checkCollision(box, room->walls[i].getBox(), offsety))
			{
				return true;
			}
		}
	}
	return false;
}

bool touchesPlayer(SDL_Rect box, Player player)
{
	if (checkCollision(box, player.getBox(), 0))
	{
		return true;
	}
	return false;
}















///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

//////////    ///////                   ///////
      ///     ///   ///                 ///
   ///        ///    //                 /// ///   
//    ///     ///   ///                 ///  
 //////       ///////                   ///////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LPolygon.h"
#include <vector>
static int terrainScale = 64;

class Player1
{
public:
	int x, y, z;
	float anglex, anglez;

	Player1()
	{
		x = 0;
		y = 0;
		z = 50;
		anglex = 0.0f;
		anglez = 0.0f;
	}

	void moveBack()
	{
		z += 15;
		
	}

	void moveForward()
	{
		z -= 15;
	}

	void moveLeft()
	{
		
		x -= 15;
		
	}

	void moveRight()
	{
		
		x += 15;
		
	}


};

class Camera
{
public:

	Camera()
	{
		x = -10;
		y = -10;
		z = 00;

		ax = 0.0;
		ay = 0.0;
		az = 0.0;
	}
	void setCamera(Player1& p)
	{
		x = p.x - cos(ax) * 128;
		y = p.y - sin(ay) * 128;
		z = p.z + 80;
	}

	void rotateUp()
	{
		az += 6.28 / 50;
	}

	void rotateDown()
	{
		az -= 6.28 / 50;
	}

	void rotateLeft()
	{
		ax += 6.28 / 50;
	}

	void rotateRight()
	{
		ax -= 6.28 / 50;
	}

	/*void rotateUp()
	{
		ax += 6.28 / 50;
	}

	void rotateDown()
	{
		ax -= 6.28 / 50;
	}*/

	int x, y, z;
	double ax, ay, az;


};

//some math has to be done to convert coordinates each frame as follows:
// first take objects point and subtract camera points
// newX = objx - camx
// newY = objy - camy
// newZ = objz - camz



//then we have to do this:
//Dx = cos(ay)(newy*sin(az) + cos(ax)*newx) - sin(az) * newZ
//Dy = sin(ax)(cos(ay)*newz + sin(ay)(sin(ay)*newY + cos(ax)*newX)) + cos(ax)(cos(ay)*newY - sin(ax)*newx)
//Dz = cos(ax)(cos(ay)*newz + sin(ay)(sin(az)*newY + cos(az)*newx)) - sin(ax)(cos(az)*newY - sin(ax)*newX)

//from here the x and y is 
//bx = (p.z/dz)*dx - p.x;
//by = (p.z/dz)*dy - p.y;



template <typename T>
class Vertice {
public: 
	Vertice()
	{
		x, y, z = 0, 0, 0;
	};

	Vertice(T m, T h, T k) : x(m), y(h), z(k)
	{};
	T x, y, z;
};

typedef Vertice<float> vec;

struct IndexedLineList
{
	std::vector<vec> vertices;
	std::vector<size_t> indices;
};

class Mesh
{
public:

	void rotateXLeft()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = (x - p->x) * cos(0.05) - (y - p->y) * sin(0.05);
			yp = (x - p->x) * sin(0.05) + (y - p->y) * cos(0.05);
			zp = z;

			v.x = xp + p->x;
			v.y = yp + p->y;
			v.z = zp;
		}
	}

	void rotateXRight()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = (x - p->x) * cos(-0.05) - (y - p->y) * sin(-0.05);
			yp = (x - p->x) * sin(-0.05) + (y - p->y) * cos(-0.05);
			zp = z;

			v.x = xp + p->x;
			v.y = yp + p->y;
			v.z = zp;
		}
	}

	void rotateYLeft()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = x;
			yp = (y - p->y) * cos(0.05) - (z - p->z) * sin(0.05);
			zp = (y - p->y) * sin(0.05) + (z - p->z) * cos(0.05);

			v.x = xp;
			v.y = yp + p->y;
			v.z = zp + p->z;
		}
	}

	void rotateYRight()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = x;
			yp = (y - p->y) * cos(-0.05) - (z - p->z) * sin(-0.05);
			zp = (y - p->y) * sin(-0.05) + (z - p->z) * cos(-0.05);

			v.x = xp;
			v.y = yp + p->y;
			v.z = zp + p->z;
		}
	}

	void rotateZLeft()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = (z - p->z) * sin(0.05) + (x - p->x) * cos(0.05);
			yp = y;
			zp = (z - p->z) * cos(0.05) - (x - p->x) * sin(0.05);

			v.x = xp + p->x;
			v.y = yp;
			v.z = zp + p->z;
		}
	}

	void rotateZRight()
	{
		for (auto& v : vertices)
		{
			float centerx = 0.0f;
			float centery = 0.0f;
			float centerz = 0.0f;

			float x, y, z;
			x = v.x;
			y = v.y;
			z = v.z;

			float xp, yp, zp;
			xp = (z - p->z) * sin(-0.05) + (x - p->x) * cos(-0.05);
			yp = y;
			zp = (z - p->z) * cos(-0.05) - (x - p->x) * sin(-0.05);

			v.x = xp + p->x;
			v.y = yp;
			v.z = zp + p->z;
		}
	}


	

	void moveUp()
	{
		for (auto& v : vertices)
		{
			v.y -= 5;
		}
	}

	void moveDown()
	{
		for (auto& v : vertices)
		{
			v.y += 5;
		}
	}

	void setp(Player1* src)
	{
		p = src;
	}

	std::vector<vec> vertices;
	Player1* p;
};

//some math has to be done to convert coordinates each frame as follows:
// first take objects point and subtract camera points
// newX = objx - camx
// newY = objy - camy
// newZ = objz - camz



//then we have to do this:
//Dx = cos(ay)(newy*sin(az) + cos(ax)*newx) - sin(az) * newZ
//Dy = sin(ax)(cos(ay)*newz + sin(ay)(sin(ay)*newY + cos(ax)*newX)) + cos(ax)(cos(ay)*newY - sin(ax)*newx)
//Dz = cos(ax)(cos(ay)*newz + sin(ay)(sin(az)*newY + cos(az)*newx)) - sin(ax)(cos(az)*newY - sin(ax)*newX)

//from here the x and y is 
//bx = (p.z/dz)*dx - p.x;
//by = (p.z/dz)*dy - p.y;
struct Point2
{
	int x, y;
};

vec transform(vec& v, Player1& p, Camera& c)
{
	int newX = v.x - c.x;
	int newY = v.y - c.y;
	int newZ = v.z - c.z;

	double Dx = cos(c.ay) * (newY*sin(c.az) + cos(c.ax)*newX) - sin(c.az) * newZ;
	double Dy = sin(c.ax)*(cos(c.ay)*newZ + sin(c.ay)*(sin(c.ay)*newY + cos(c.ax)*newX)) + cos(c.ax)*(cos(c.ay)*newY - sin(c.ax)*newX);
	double Dz = cos(c.ax)*(cos(c.ay)*newZ + sin(c.ay)*(sin(c.az)*newY + cos(c.az)*newX)) - sin(c.ax)*(cos(c.az)*newY - sin(c.ax)*newX);

	Point2 point = { (p.z / Dz)*Dx - p.x, (p.z / Dz)*Dy - p.y };

	
	v.x = point.x;
	v.y = point.y;

	return v;
}

bool inVision(Player1& p, vec& x)
{
	bool success = false;
	double anglex = atan2(p.y - x.y, p.x - x.x);
	double anglez = atan2(p.x - x.x, p.z - x.z);
	if (anglex > p.anglex - (3.14 / 2) && anglex < p.anglex + (3.14 / 2))
		success = true;
	else
		success = false;

	if (anglez > p.anglez - (3.14 / 2) && anglez < p.anglez + (3.14 / 2))
		success = true;
	else
		success = false;


	return success;
}



class LeftTriangle : public Mesh
{
public:
	LeftTriangle(float size)
	{
		float side = size / 10.0f;
		vertices.emplace_back(-side, -side, -side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(side, side, -side);
	}

	LeftTriangle(float sizex, float sizez, float sizey)
	{
		float sidex = sizex;
		float sidez = sizez;
		float sidey = sizey;
		vertices.emplace_back(sidex + terrainScale, sidey + terrainScale , -sidez);
		vertices.emplace_back(sidex, sidey + terrainScale, -sidez);
		vertices.emplace_back(sidex + terrainScale, sidey, -sidez);
	}


	IndexedLineList getLines()
	{
		return {
			vertices,{
			0,1,  1,2,  2,0  
		}
		};
	}
};

class RightTriangle : public Mesh
{
public:
	RightTriangle(float size)
	{
		float side = size / 10.0f;
		vertices.emplace_back(side, side - terrainScale, -side);
		vertices.emplace_back(side, side, -side);
		vertices.emplace_back(side - terrainScale, side - terrainScale, -side);
	}

	RightTriangle(float sizex, float sizez, float sizey)
	{
		float sidex = sizex;
		float sidez = sizez;
		float sidey = sizey;
		vertices.emplace_back(sidex, sidey, -sidez);
		vertices.emplace_back(sidex + terrainScale, sidey, -sidez);
		vertices.emplace_back(sidex, sidey + terrainScale, -sidez);
	}


	IndexedLineList getLines()
	{
		return {
			vertices,{
			0,1,  1,2,  2,0
		}
		};
	}
};

class square 
{
public:
	square(float x, float y, float z) : rt(x, z, y), lt(x, z, y)
	{

	}

	void composeFrame(SDL_Renderer* gRenderer, Player1& p, Camera& c)
	{
		
		IndexedLineList lines = rt.getLines();
		for (auto& v : lines.vertices)
		{
			transform(v, p, c);
		}
		for (auto i = lines.indices.begin(), end = lines.indices.end(); i != end; std::advance(i, 2))
		{
			//if (inVision(p, lines.vertices[*i]))
			SDL_RenderDrawLine(gRenderer, lines.vertices[*i].x - p.x, lines.vertices[*i].y - p.y, lines.vertices[*std::next(i)].x - p.x, lines.vertices[*std::next(i)].y - p.y);
		}

		lines = lt.getLines();
		for (auto& v : lines.vertices)
		{
			transform(v, p, c);
		}
		for (auto i = lines.indices.begin(), end = lines.indices.end(); i != end; std::advance(i, 2))
		{
			//if (inVision(p, lines.vertices[*i]))
			SDL_RenderDrawLine(gRenderer, lines.vertices[*i].x - p.x, lines.vertices[*i].y - p.y, lines.vertices[*std::next(i)].x - p.x, lines.vertices[*std::next(i)].y - p.y);
		}
	}

	void rotateXLeft()
	{
		lt.rotateXLeft();
		rt.rotateXLeft();
	}

	void rotateXRight()
	{
		lt.rotateXRight();
		rt.rotateXRight();
	}

	void rotateYLeft()
	{
		lt.rotateYLeft();
		rt.rotateYLeft();
	}

	void rotateYRight()
	{
		lt.rotateYRight();
		rt.rotateYRight();
	}

	void rotateZLeft()
	{
		lt.rotateZLeft();
		rt.rotateZLeft();
	}

	void rotateZRight()
	{
		lt.rotateZRight();
		rt.rotateZRight();
	}


	//void moveBack()
	//{
	//	lt.moveBack();
	//	rt.moveBack();
	//}

	//void moveForward()
	//{
	//	lt.moveForward();
	//	rt.moveForward();
	//}

	//void moveLeft()
	//{
	//	lt.moveLeft();
	//	rt.moveLeft();
	//}

	//void moveRight()
	//{
	//	lt.moveRight();
	//	rt.moveRight();
	//}

	void moveUp()
	{
		lt.moveUp();
		rt.moveUp();
	}

	void moveDown()
	{
		lt.moveDown();
		rt.moveDown();
	}

	RightTriangle rt;
	LeftTriangle lt;
};

class Strip
{
public:
	int x, y, w, h;
	std::vector<vec> vertices;
	std::vector<square> squares;

	void setPlayer(Player1* p)
	{
		for (auto& v : squares)
		{
			v.lt.setp(p);
			v.rt.setp(p);
		}
	}

	Strip(int x1, int y1, int w1, int h1) : x(x1), y(y1), w(w1), h(h1)
	{

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				vertices.emplace_back(j * terrainScale + x1, i * terrainScale + y1, -4.0 * terrainScale);
			}
		}

		for (auto& v : vertices)
		{
			squares.emplace_back(square( v.x, v.y, v.z ) );
		}
		
	}

	void composeFrame(SDL_Renderer* gRenderer, Player1& p, Camera& c)
	{
		for (auto& v : squares)
		{
			v.composeFrame(gRenderer, p, c);
		}
		
	}

	void rotateXLeft()
	{
		for (auto& v : squares)
		{
			v.rotateXLeft();
		}
	}

	void rotateXRight()
	{
		for (auto& v : squares)
		{
			v.rotateXRight();
		}
	}

	void rotateYLeft()
	{
		for (auto& v : squares)
		{
			v.rotateYLeft();
		}
	}

	void rotateYRight()
	{
		for (auto& v : squares)
		{
			v.rotateYRight();
		}
	}

	void rotateZLeft()
	{
		for (auto& v : squares)
		{
			v.rotateZLeft();
		}
	}

	void rotateZRight()
	{
		for (auto& v : squares)
		{
			v.rotateZRight();
		}
	}


	//void moveBack()
	//{
	//	for (auto& v : squares)
	//	{
	//		v.moveBack();
	//	}
	//}

	//void moveForward()
	//{
	//	for (auto& v : squares)
	//	{
	//		v.moveForward();
	//	}
	//}

	//void moveLeft()
	//{
	//	for (auto& v : squares)
	//	{
	//		v.moveLeft();
	//	}
	//}

	//void moveRight()
	//{
	//	for (auto& v : squares)
	//	{
	//		v.moveRight();
	//	}
	//}

	void moveUp()
	{
		for (auto& v : squares)
		{
			v.moveUp();
		}
	}

	void moveDown()
	{
		for (auto& v : squares)
		{
			v.moveDown();
		}
	}
};


struct Cube : public Mesh
{
	Cube(float size)
	{
		float side = size / 2.0f;
		vertices.emplace_back(-side, -side, -side*2);
		vertices.emplace_back(side, -side, -side*2);
		vertices.emplace_back(-side, side, -side*2);
		vertices.emplace_back(side, side, -side*2);
		vertices.emplace_back(-side, -side, -side);
		vertices.emplace_back(side, -side, -side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(side, side, -side);	
	}

	IndexedLineList getLines() 
	{
		return {
			vertices, {
			0,1,  1,3,  3,2,  2,0,
			0,4,  1,5,  3,7,  2,6,
			4,5,  5,7,  7,6,  6,4
			}
		};
	}

	
};




class Game
{ 
public: 
	Game() : strip(100, 100, 15, 15)
	{
		strip.setPlayer(&p);
	}
	
	void composeFrame(SDL_Renderer* gRenderer)
	{
		strip.composeFrame(gRenderer, p, c);
	}



	//Cube cube;
	//square strip;
	//LeftTriangle Ltriangle;
	//RightTriangle Rtriangle;
	Strip strip;
	Player1 p;
	Camera c;

};



class polygon
{
public:

	polygon()
	{
		listStart = new vec[10];
		listEnd = new vec[10];
		vertices = 4;
		for (int i = 0; i < 10; i++)
		{
			listStart[i].x = NULL;
			listStart[i].y = NULL;
			listEnd[i].x = NULL;
			listEnd[i].y = NULL;
		}
	}

	void draw(SDL_Renderer* gRenderer)
	{
		SDL_SetRenderDrawColor(gRenderer, 100, 0, 0, 255);

		for (int i = 0; i < vertices; i++)
		{
			//if (!(listStart[i].x == NULL && listStart[i].y == NULL && listEnd[i].x == NULL && listEnd[i].y == NULL))
			{
				//a,b,c x d,e,f == bf - ce, cd - af, ae - bd
				//x = Y x Z, 
				double Factor1, Factor2, Factor3, Factor4;
				double Z1 = listStart[i].z + 0.01;
				double Z2 = listEnd[i].z + 0.01;
				Factor1 = 1.0 / Z1;
				Factor2 = 1.0 / Z2;

				SDL_RenderDrawLine(gRenderer, listStart[i].x * Factor1, listStart[i].y * Factor1, listEnd[i].x * Factor2, listEnd[i].y * Factor2);
				std::cout << "drew line \n";
			}
		}
	}

	void moveLeft()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].x -= 25;
			listEnd[i].x -= 25;
		}
	}

	void moveRight()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].x += 25;
			listEnd[i].x += 25;
		}
	}

	void moveBack()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].z -= .05;
			listEnd[i].z -= .05;
		}
	}

	void moveForward()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].z += .05;
			listEnd[i].z += .05;
		}
	}

	void moveUp()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].y -= 25;
			listEnd[i].y -= 25;
		}
	}

	void moveDown()
	{
		for (int i = 0; i < vertices; i++)
		{
			listStart[i].y += 25;
			listEnd[i].y += 25;
		}
	}

	void rotateXLeft()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = -200;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = (x - centerx) * cos(.05) - (y - centery) * sin(.05);
			double yp = (y- centery) * cos(.05) + (x - centerx) * sin(.05);
			double zp = z;

			listStart[i].x = xp + centerx;
			listStart[i].y = yp + centery;
			listStart[i].z = zp;
		}
		
		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = (x - centerx) * cos(.05) - (y - centery) * sin(.05);
			double yp = (y - centery) * cos(.05) + (x - centerx) * sin(.05);
			double zp = z;

			listEnd[i].x = xp + centerx;
			listEnd[i].y = yp + centery;
			listEnd[i].z = zp;
		}
	}

	void rotateXRight()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = listStart[0].z + (listEnd[0].z - listStart[0].z) / 2;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = (x - centerx) * cos(-.05) - (y - centery) * sin(-.05);
			double yp = (y - centery) * cos(-.05) + (x - centerx) * sin(-.05);
			double zp = z;

			listStart[i].x = xp + centerx;
			listStart[i].y = yp + centery;
			listStart[i].z = zp;
		}

		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = (x - centerx) * cos(-.05) - (y - centery) * sin(-.05);
			double yp = (y - centery) * cos(-.05) + (x - centerx) * sin(-.05);
			double zp = z;

			listEnd[i].x = xp + centerx;
			listEnd[i].y = yp + centery;
			listEnd[i].z = zp;
		}
	}

	void rotateYLeft()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = -200;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = x;
			double yp = (y - centery) * cos(.05) - (z - centerz) * sin(.05);
			double zp = (z - centerz) * cos(.05) + (y - centery) * sin(.05);

			listStart[i].x = xp;
			listStart[i].y = yp + centery;
			listStart[i].z = zp + centerz;
		}

		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = x;
			double yp = (y - centery) * cos(.05) - (z - centerz) * sin(.05);
			double zp = (z - centerz) * cos(.05) + (y - centery) * sin(.05);

			listEnd[i].x = xp;
			listEnd[i].y = yp + centery;
			listEnd[i].z = zp + centerz;
		}
	}

	void rotateYRight()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = -200;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = x;
			double yp = (y - centery) * cos(-.05) - (z - centerz) * sin(-.05);
			double zp = (z - centerz) * cos(-.05) + (y - centery) * sin(-.05);

			listStart[i].x = xp;
			listStart[i].y = yp + centery;
			listStart[i].z = zp + centerz;
		}

		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = x;
			double yp = (y - centery) * cos(-.05) - (z - centerz) * sin(-.05);
			double zp = (z - centerz) * cos(-.05) + (y - centery) * sin(-.05);

			listEnd[i].x = xp;
			listEnd[i].y = yp + centery;
			listEnd[i].z = zp + centerz;
		}
	}

	void rotateZRight()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = -200;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = (z - centerz) * sin(-.05) + (x - centerx) * cos(-.05);
			double yp = y;
			double zp = (z - centerz) * cos(-.05) - (x - centerx) * sin(-.05);

			listStart[i].x = xp + centerx;
			listStart[i].y = yp ;
			listStart[i].z = zp + centerz;
		}

		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = (z - centerz) * sin(-.05) + (x - centerx) * cos(-.05);
			double yp = y;
			double zp = (z - centerz) * cos(-.05) - (x - centerx) * sin(-.05);

			listEnd[i].x = xp + centerx;
			listEnd[i].y = yp ;
			listEnd[i].z = zp + centerz;
		}
	}

	void rotateZLeft()
	{
		double centerx = SCREEN_WIDTH / 2;
		double centery = SCREEN_HEIGHT / 2;
		double centerz = -200;

		for (int i = 0; i < vertices; i++)
		{
			//rotat around center of object == liststart[0].x  + liststart[0].x- listend[0].x/2

			int x = listStart[i].x;
			int y = listStart[i].y;
			int z = listStart[i].z;

			double xp = (z - centerz) * sin(.05) + (x - centerx) * cos(.05);
			double yp = y;
			double zp = (z - centerz) * cos(.05) - (x - centerx) * sin(.05);

			listStart[i].x = xp + centerx;
			listStart[i].y = yp;
			listStart[i].z = zp + centerz;
		}

		for (int i = 0; i < vertices; i++)
		{
			int x = listEnd[i].x;
			int y = listEnd[i].y;
			int z = listEnd[i].z;

			double xp = (z - centerz) * sin(.05) + (x - centerx) * cos(.05);
			double yp = y;
			double zp = (z - centerz) * cos(.05) - (x - centerx) * sin(.05);

			listEnd[i].x = xp + centerx;
			listEnd[i].y = yp;
			listEnd[i].z = zp + centerz;
		}
	}

	int vertices;

	vec* listStart;
	vec* listEnd;
};

void drawRect(polygon *a)
{
	for (int i = 0; i < 10; i++)
	{
		
	}
}
void _3D_Engine_Init()
{
	initSDL();
	polygon* square  = new polygon;
	SDL_Event e;
	Game game;

	//SDL_Rect square = { 500, 500, 50, 50 };
	
	while (true)
	{


		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					game.p.moveLeft();
				//	game.strip.rotateXLeft();
					break;
				case SDLK_d:
					game.p.moveRight();
				//	game.strip.rotateXRight();
					break;
				case SDLK_w:
					game.p.moveForward();
					//game.p.moveUp();
					break;
				case SDLK_s:
					//game.p.moveDown();
					game.p.moveBack();
					break;
				case SDLK_q:
					game.c.rotateLeft();
					break;
				case SDLK_e:
					game.c.rotateRight();
					break;
				case SDLK_r:
					game.strip.rotateYLeft();
					break;
				case SDLK_t:
					game.strip.rotateYRight();
					break;
				case SDLK_f:
					game.strip.rotateZLeft();
					break;
				case SDLK_g:
					game.strip.rotateZRight();
					break;
				}
			}

			if (e.wheel.y == 1)
			{
				game.c.rotateUp();
			}
			if (e.wheel.y == -1)
			{
				game.c.rotateDown();
			}
		}
		game.c.setCamera(game.p);
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
		
		//square->draw(gRenderer);
		game.composeFrame(gRenderer);

		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderPresent(gRenderer);
		//Lpolygon.render;
	}
}































