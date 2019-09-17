#include "RenderableManager.h"
#include "RPotion.h"
#include "InventoryRedux.h"

RenderableManager::RenderableManager()
{
	maxCapacity = 30;
	renderQueue = new Renderable*[maxCapacity];
	for (int i = 0; i < maxCapacity; i++)
	{
		renderQueue[i] = NULL;
		std::cout << renderQueue[i];
	}
	used = 0;
}

RenderableManager::~RenderableManager()
{
	for (int i = 0; i < maxCapacity; i++)
	{
		delete[] renderQueue[i];
	}
	delete[] renderQueue;
}

//void RenderableManager::renderAll(SDL_Renderer* gRenderer, SDL_Rect& camera, Player* p, Inventory3* inv)
//{
//	for (int i = 0; i < maxCapacity; i++)
//	{
//		if (renderQueue[i]!= NULL)
//		if (renderQueue[i]->getAlive())
//		{
//			renderQueue[i]->waveMove();
//			renderQueue[i]->render(gRenderer, camera);
//
//			if (PickUp(*renderQueue[i], *p))
//			{
//
//				renderQueue[i]->lifetime = 0;
//				renderQueue[i]->placeItem(inv);
//				
//			}
//		}
//		else
//		{
//			
//				delete renderQueue[i];
//				renderQueue[i] = NULL;
//			
//			used--;
//		}
//	}
//}

void RenderableManager::updateAll(SDL_Renderer* gRenderer, Inventory3* inv, Player* player)
{
	for (int i = 0; i < maxCapacity; i++)
	{
		if (renderQueue[i] != NULL)
		{
			if (renderQueue[i]->getAlive())
			{
				//if it's alive we can wavemove
				renderQueue[i]->waveMove();
				//if it's pickupable, if it's dropped set invul frames and each loop -- that frame
				if (renderQueue[i]->pickable == 0)
				{
					if (PickUp(*renderQueue[i], *player))
					{

						renderQueue[i]->lifetime = 0;
						renderQueue[i]->placeItem(inv);
						renderQueue[i] = NULL;
					}
				}
				else
				{
					renderQueue[i]->pickable--;
				}
			}
			else
			{
				delete[] renderQueue[i];
				renderQueue[i] = NULL;
					
			}
		}
	}
}

void RenderableManager::renderAll(SDL_Renderer* gRenderer, SDL_Rect& camera)
{
	for (int i = 0; i < maxCapacity; i++)
	{
		if (renderQueue[i] != NULL)
			renderQueue[i]->render(gRenderer, camera);
	}
}

void RenderableManager::addRenderable(Renderable* target)
{
	for (int i = 0; i < maxCapacity; i++)
	{
		if (renderQueue[i] == NULL)
		{
			renderQueue[i] = target;
			break;
		}
	}
}

void RenderableManager::addRenderableManaPotion(int x, int y, int resolution, SDL_Renderer* gRenderer)
{
	renderQueue[used] = new RPotion(x, y, resolution, 1, gRenderer);
	used++;
}