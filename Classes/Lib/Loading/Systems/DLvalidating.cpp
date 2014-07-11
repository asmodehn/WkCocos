#include "WkCocos/Loading/Systems/DLvalidating.h"

#include "WkCocos/Loading/Events/Error.h"


namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			void DLvalidating::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::LocalFile> localfile;
				entityx::ptr<Comp::TempFile> tmpfile;
				entityx::ptr<Comp::TempMD5> tmpsig;
				entityx::ptr<Comp::RemoteFile> remotefile;
				entityx::ptr<Comp::RemoteMD5> remotesig;
				for (auto entity : es->entities_with_components(localfile, tmpfile, tmpsig, remotefile, remotesig))
				{
					if (remotesig->getMD5() == tmpsig->getMD5())
					{
						//download is fine.
						const std::string finalFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + localfile->getPath().c_str();
						if (cocos2d::FileUtils::getInstance()->isFileExist(finalFileName))
						{
							remove(finalFileName.c_str());
						}
						if (0 != rename(tmpfile->getPath().c_str(), finalFileName.c_str()))
						{
							CCLOG("error renaming %s to %s", tmpfile->getPath().c_str(), finalFileName.c_str());
						}
						CCLOG("succeed downloading %s", localfile->getPath().c_str());

						//we remove the remotefile to validate the success of the download
						entity.remove<Comp::RemoteFile>();

						//NOTE :
						//we dont need to register the new files in manifest,
						//because we compute locally the md5 before deciding to download or not
						//=> If the file is at the expected place with proper signature, download will not happen again.

						//we delete the entity to register progress
						entity.destroy();
					}
					
				}
			};
					
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

