#include "WkCocos/SaveManager.h"


namespace WkCocos
{
	SaveManager::SaveManager()
		: m_descCallback(nullptr)
	{}

	SaveManager::~SaveManager()
	{}

	void SaveManager::configure()
	{
		//GPGSManager::getInstance()->getEventManager()->subscribe<GPGSManager::SignedIn>(*this);
		//GPGSManager::getInstance()->getEventManager()->subscribe<GPGSManager::SignedOut>(*this);
		
		GPGSManager::getInstance()->getEventManager()->subscribe<GPGSManager::SnapshotSaveRequested>(*this);
		//subscribing the player so he knows what to load and when.
		GPGSManager::getInstance()->getEventManager()->subscribe<GPGSManager::SnapshotLoaded>(*this);
	}

	void SaveManager::createBackup()
	{
		std::vector<uint8_t> snapshot;
		int size = m_managedSave.size();
		char* ptr = reinterpret_cast<char*>(&size);
		for (int i = 0; i < 4; ++i)snapshot.push_back(ptr[i]);
		std::string strSnapShot;
		for (auto curSave = m_managedSave.begin(); curSave != m_managedSave.end(); ++curSave)
		{
			WkCocos::StrongBox::StrongBox secure((*curSave)->getKey());
			secure.set((*curSave)->getData());
			size = secure.get_encryptedHex().size();
			for (int i = 0; i < 4; ++i)snapshot.push_back(ptr[i]);
			strSnapShot = secure.get_encryptedHex();
			snapshot.insert(snapshot.end(), strSnapShot.begin(), strSnapShot.end());
		}

		SaveInfo info;
		if(m_descCallback)
		{
			m_descCallback(info);
		}
		assert(!m_saveName.empty());
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		GPGSManager::getInstance()->saveSnapshot(m_saveName, info.m_desc, info.m_timePlayedms, info.m_icon, snapshot);
#endif
		dispatchBackup(snapshot);
	}

	void SaveManager::dispatchBackup(const std::vector<uint8_t>& snapshot)
	{
		int offset = 0;
		int saveCount = *(reinterpret_cast<const int*>(&(snapshot[offset])));

		assert(saveCount == m_managedSave.size());

		offset += sizeof(int);
		for (auto curSave = m_managedSave.begin(); curSave != m_managedSave.end(); ++curSave)
		{
			int saveSize = *(reinterpret_cast<const int*>(&(snapshot[offset])));
			offset += sizeof(int);

			std::string data(reinterpret_cast<const char*>(&(snapshot[offset])), saveSize);
			offset += saveSize;

			WkCocos::StrongBox::StrongBox secure((*curSave)->getKey());
			secure.set_encryptedHex(data);
			(*curSave)->requestSaveData(secure.get<std::string>());
		}
	}

	void SaveManager::receive(const GPGSManager::SnapshotSaveRequested&)
	{
		CCLOG("Save Requested");
		createBackup();
	}

	void SaveManager::receive(const GPGSManager::SnapshotLoaded& evt)
	{
		dispatchBackup(evt.mSnapData);
	}

}//namespace WkCocos
