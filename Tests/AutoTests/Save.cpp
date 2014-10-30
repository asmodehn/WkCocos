// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/Save.h"

#include <string>

SUITE(Save)
{
	
	struct OnlineSave
	{
		OnlineSave()
			: localmgr(new WkCocos::LocalData::LocalDataManager())
			, onlinemgr( new WkCocos::OnlineData::OnlineDataManager("aa52b08e5f5284a1f45a59d2d150c5a1da8fe65d67ccb54f2124dcc16e7bc51a","0f30591599be40aa1bb4f933bbaa5c03813664a67d5b98cdaf452600b92b4cfd"))
			, m_save("save_test", WkCocos::Save::Mode::ONLINE)
		{
			m_save.setLocalDataMgr(localmgr);
			m_save.setOnlineDataMgr(onlinemgr);
		}
		~OnlineSave() 
		{
		}

		WkCocos::Save m_save;
		std::shared_ptr<WkCocos::LocalData::LocalDataManager> localmgr;
		std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinemgr;

	};

	struct OfflineSave
	{
		OfflineSave()
			: localmgr(new WkCocos::LocalData::LocalDataManager())
			, m_save("save_test", WkCocos::Save::Mode::OFFLINE)
		{
			m_save.setLocalDataMgr(localmgr);
		}
		~OfflineSave() { /* some teardown */ }

		WkCocos::Save m_save;
		std::shared_ptr<WkCocos::LocalData::LocalDataManager> localmgr;
		std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinemgr;
	};

	struct EncryptedOfflineSave
	{
		EncryptedOfflineSave()
			: localmgr(new WkCocos::LocalData::LocalDataManager())
			, m_save("save_test", WkCocos::Save::Mode::OFFLINE, "53CR3TK3Y") 
		{
			m_save.setLocalDataMgr(localmgr);
		}
		~EncryptedOfflineSave() { /* some teardown */ }

		WkCocos::Save m_save;
		std::shared_ptr<WkCocos::LocalData::LocalDataManager> localmgr;
		std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinemgr;
	};

	////OFFLINE Save Tests Begin
	//Testing if saving and loading are symmetric for a test string. synchronous run.
	//NOT WORKING
	/*
	TEST_FIXTURE(OfflineSave, ClearSymmetric_Sync)
	{
		std::string original_value = "This is my save that I dont want to loose";
		
		m_save.requestSaveData(original_value);

		while (!m_save.isSaved())
		{
			this_thread::sleep_for(chrono::milliseconds(100));
		}

		m_save.requestLoadData();

		while (!m_save.isLoaded())
		{
			this_thread::sleep_for(chrono::milliseconds(100));
		}

		std::string loaded_value = m_save.getData();

		CHECK(original_value == loaded_value);

	}
	*/

	//Testing if saving and loading are symmetric for a test string. Asynchronous run.
	//TODO


	////OFFLINE Save Tests End
}

