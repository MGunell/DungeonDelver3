#include "Tile.h"
#include "Engine.h"

Tile::Tile(int x, int y, int tileType)
{
	mBox.x = x;
	mBox.y = y;

	//collision size of each tile
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	mType = tileType;
	//0 = walk thru, 1 = wall, 2 = ... 
}

void Tile::render(SDL_Rect& camera, SDL_Renderer* gRenderer, LTexture& target, double rotation)
{
	if (checkCollision(camera, mBox, 0))
	{
		//show tile
		target.renderInventory(mBox.x - camera.x, mBox.y - camera.y, &gLevelOneClips[mType], gRenderer, rotation);
	}
}

void Tile::renderRotation(double x1, double y1, SDL_Rect& camera, SDL_Renderer* gRenderer, LTexture& target, double rotation)
{
	//if (checkCollision(camera, mBox, 0))
	//{
		//show tile
		target.renderInventory(x1 - camera.x, y1 - camera.y, &gLevelOneClips[mType], gRenderer, rotation);
	//}
}

int Tile::getType()
{
	return mType;
}

void Tile::setType(int type) {
		mType = type;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}

void setTiles_()
{
	gLevelOneClips[14].x = 0;
	gLevelOneClips[14].y = 0;
	gLevelOneClips[14].w = 64;
	gLevelOneClips[14].h = 64;

	gLevelOneClips[0].x = 0;
	gLevelOneClips[0].y = 128;
	gLevelOneClips[0].w = 64;
	gLevelOneClips[0].h = 64;

	gLevelOneClips[1].x = 64;
	gLevelOneClips[1].y = 192;
	gLevelOneClips[1].w = 64;
	gLevelOneClips[1].h = 96;

	gLevelOneClips[2].x = 0;
	gLevelOneClips[2].y = 192;
	gLevelOneClips[2].w = 64;
	gLevelOneClips[2].h = 96;

	gLevelOneClips[3].x = 128;
	gLevelOneClips[3].y = 192;
	gLevelOneClips[3].w = 64;
	gLevelOneClips[3].h = 96;

	gLevelOneClips[4].x = 192;
	gLevelOneClips[4].y = 192;
	gLevelOneClips[4].w = 64;
	gLevelOneClips[4].h = 96;

	gLevelOneClips[5].x = 194;
	gLevelOneClips[5].y = 64;
	gLevelOneClips[5].w = 64;
	gLevelOneClips[5].h = 64;

	gLevelOneClips[6].x = 320;
	gLevelOneClips[6].y = 192;
	gLevelOneClips[6].w = 64;
	gLevelOneClips[6].h = 96;

	gLevelOneClips[7].x = 128;
	gLevelOneClips[7].y = 128;
	gLevelOneClips[7].w = 64;
	gLevelOneClips[7].h = 64;

	gLevelOneClips[8].x = 194;
	gLevelOneClips[8].y = 128;
	gLevelOneClips[8].w = 64;
	gLevelOneClips[8].h = 64;

	gLevelOneClips[9].x = 256;
	gLevelOneClips[9].y = 192;
	gLevelOneClips[9].w = 64;
	gLevelOneClips[9].h = 96;

	gLevelOneClips[10].x = 256;
	gLevelOneClips[10].y = 192;
	gLevelOneClips[10].w = 64;
	gLevelOneClips[10].h = 96;

	gLevelOneClips[11].x = 128;
	gLevelOneClips[11].y = 128;
	gLevelOneClips[11].w = 64;
	gLevelOneClips[11].h = 64;

	gLevelOneClips[12].x = 194;
	gLevelOneClips[12].y = 128;
	gLevelOneClips[12].w = 64;
	gLevelOneClips[12].h = 64;

	gLevelOneClips[13].x = 256;
	gLevelOneClips[13].y = 128;
	gLevelOneClips[13].w = 64;
	gLevelOneClips[13].h = 64;

	gLevelOneClips[0].x = 256;
	gLevelOneClips[0].y = 128;
	gLevelOneClips[0].w = 64;
	gLevelOneClips[0].h = 64;
}

bool setTiles(Tile* tiles[], std::string levelname)
{
	bool tilesLoaded = true;

	int x = 0;
	int y = 0;

	//open map
	std::ifstream map(levelname);

	if (!map.is_open())
	{
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			int tileType = -1;

			map >> tileType;

			if (map.fail())
			{
				tilesLoaded = false;
				break;
			}

			
			tiles[i] = new Tile(x, y, tileType);

			//move to next spot
			x += static_cast<int>(TILE_WIDTH );
			if (x >= (16 * TILE_WIDTH))
			{
				x = 0;
				y += static_cast<int>(TILE_HEIGHT);
			}
		}
		if (tilesLoaded)
		{
			gLevelOneClips[0].x = 0;
			gLevelOneClips[0].y = 0;
			gLevelOneClips[0].w = 64;
			gLevelOneClips[0].h = 64;

			gLevelOneClips[1].x = 0;
			gLevelOneClips[1].y = 128;
			gLevelOneClips[1].w = 64;
			gLevelOneClips[1].h = 64;

			gLevelOneClips[2].x = 128;
			gLevelOneClips[2].y = 0;
			gLevelOneClips[2].w = 64;
			gLevelOneClips[2].h = 64;

			gLevelOneClips[3].x = 192;
			gLevelOneClips[3].y = 0;
			gLevelOneClips[3].w = 64;
			gLevelOneClips[3].h = 64;

			gLevelOneClips[4].x = 256;
			gLevelOneClips[4].y = 0;
			gLevelOneClips[4].w = 64;
			gLevelOneClips[4].h = 64;

			gLevelOneClips[5].x = 128;
			gLevelOneClips[5].y = 64;
			gLevelOneClips[5].w = 64;
			gLevelOneClips[5].h = 64;

			gLevelOneClips[6].x = 194;
			gLevelOneClips[6].y = 64;
			gLevelOneClips[6].w = 64;
			gLevelOneClips[6].h = 64;

			gLevelOneClips[7].x = 256;
			gLevelOneClips[7].y = 64;
			gLevelOneClips[7].w = 64;
			gLevelOneClips[7].h = 64;

			gLevelOneClips[8].x = 128;
			gLevelOneClips[8].y = 128;
			gLevelOneClips[8].w = 64;
			gLevelOneClips[8].h = 64;

			gLevelOneClips[9].x = 194;
			gLevelOneClips[9].y = 128;
			gLevelOneClips[9].w = 64;
			gLevelOneClips[9].h = 64;

			gLevelOneClips[10].x = 256;
			gLevelOneClips[10].y = 128;
			gLevelOneClips[10].w = 64;
			gLevelOneClips[10].h = 64;
		}
	}

	map.close();

	return tilesLoaded;
}

bool loadLevelOneMedia(Tile* tileSet[], SDL_Renderer* gRenderer, std::string levelname)
{
	bool success = true;
	if (!gTileTexture.loadFromFile("images/tiles.png", gRenderer))
	{
		printf("couldnt load level one textures");
		success = false;
	}

	setTiles_();
	{
		printf("couldnt set tiles");
		success = false;
	}

	

	return success;
}

