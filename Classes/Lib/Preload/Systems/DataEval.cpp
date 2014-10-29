#include "WkCocos/Preload/Systems/DataEval.h"
#include "WkCocos/Preload/Comp/DataLoad.h"
#include "WkCocos/Preload/Comp/LoadFunc.h"
#include "WkCocos/Preload/Comp/ProgressValue.h"
#include "WkCocos/Preload/Events/Error.h"

#include "WkCocos/Interface.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			/**
			* 
			*/
			void DataEval::chooseLoader(entityx::Entity entity, std::string datafile, bool force, entityx::ptr<entityx::EventManager> events)
			{
				//if png we can load asynchronously
				if (datafile.compare(datafile.size() - 4, 4, ".png") == 0)
				{
					if (force)
					{
						//we need to force the load : make sure it is not in cache
						std::string datafullpath  = cocos2d::FileUtils::getInstance()->fullPathForFilename(datafile);
						cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(datafullpath);
					}

					//assigning the proper loadfunc component
					entity.assign<Comp::LoadASyncFunc>(datafile, 0, [datafile](std::function<void(cocos2d::Texture2D*)> endcb){
						cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(datafile, endcb);
					});
				}
				//but plist has to be loaded synchronously
				else if (datafile.compare(datafile.size() - 6, 6, ".plist") == 0)
				{
					if (force)
					{
						//we need to force the load : make sure it s not in cache
						std::string datafullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(datafile);
						cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(datafullpath);
					}

					//assigning the proper loadfunc component
					entity.assign<Comp::LoadSyncFunc>(datafile, 0, [datafile](){
						cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(datafile);
					});
				}
				//and json needs to be loaded with specific loader.
				else if (datafile.compare(datafile.size() - 8, 8, ".ui.json") == 0)
				{
					//TODO : force needed ? 

					entityx::ptr<Comp::DataLoad> dataload = entity.component<Comp::DataLoad>();
					std::string file = dataload->getFilepath();
					//assigning the proper loadfunc component
					entity.assign<Comp::LoadSyncFunc>(file, 0, [file](){
						//BUG HERE sometimes ?
						Interface::load(file);
					});
				}
				else if (datafile.compare(datafile.size() - 10, 10, ".anim.json") == 0)
				{
					//TODO : force needed ?

					entityx::ptr<Comp::DataLoad> dataload = entity.component<Comp::DataLoad>();
					std::string file = dataload->getFilepath();
					//assigning the proper loadfunc component
					entity.assign<Comp::LoadSyncFunc>(file, 0, [file, entity](){
						cocostudio::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(file);
					});

				}
				else if (datafile.compare(datafile.size() - 11, 11, ".scene.json") == 0)
				{
					//TODO : force needed ?

					entityx::ptr<Comp::DataLoad> dataload = entity.component<Comp::DataLoad>();
					std::string file = dataload->getFilepath();
					//assigning the proper loadfunc component
					entity.assign<Comp::LoadSyncFunc>(file, 0, [file, entity](){
						auto entityCopy = entity;
						entityCopy.assign<Comp::ProgressValue>(1);
					});
				}
				else
				{
					//Signal Error
					events->emit<Events::Error>(entity, "DataEval system, chooseLoader error");
				}
			}

			void DataEval::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::DataLoad> dataload;
				entityx::ptr<Comp::DataDepends> datadpds;
				for (auto entity : es->entities_with_components(dataload, datadpds))
				{
					//we need to wait for the file to be there ( might come with DLC ), otherwise cocos loading will fail.
					if (dataload && !dataload->loaded && cocos2d::FileUtils::getInstance()->isFileExist(dataload->getFilepath()))
					{
						//we also need to wait for all it s dependencies to be there, otherwise cocos loading will fail.
						if (datadpds && datadpds->allLoaded())
						{
							bool allexists = true;
							for (auto dpdfp : datadpds->getDependsFilepath())
							{
								if (!cocos2d::FileUtils::getInstance()->isFileExist(dpdfp))
								{
									allexists = false;
									break;
								}
							}

							if (allexists)
							{
								//generate the appropriate loader component
								chooseLoader(entity, dataload->getFilepath(), dataload->force, events);

								//mark the entity as loaded
								dataload->loaded = true;
							}
						}
					}

				}

			}
					
		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

