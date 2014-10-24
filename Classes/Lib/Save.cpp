#include "WkCocos/Save.h"

#include "WkCocos/Utils/log/logstream.h"

namespace WkCocos
{
	Save::Save(const std::string& saveName, Mode mode, std::string key)
		: m_name(saveName)
		, m_key(key)
		, m_rawData("")
		, m_onLoading(nullptr)
		, m_onSaving(nullptr)
		, m_loaded(0)
		, m_saved(0)
	{
		m_saveModes[static_cast<int>(mode)] = 1;

		//making sure logstream exists.
		WkCocos::LogStream::create();
	}
	
	Save::~Save()
	{
	}

	bool Save::requestLoadData()
	{
		bool loaded = true;

		if (isMode(Mode::ONLINE)) //no encryption online
		{
			if (m_onlinedata)
			{
				++m_loaded;
				m_onlinedata->load(m_user, m_name, [=](std::string docId, std::vector<std::string> data)
				{
					m_docId = docId;
					if (!data.empty())
					{
						m_rawData = data.back();
					}
					else
					{
						m_rawData = "";
					}

					--m_loaded;
					event_manager->emit<Loaded>(getId(),m_name, m_rawData);
					//CCLOG("user data loaded : %s", data.c_str());
				},m_key);
			}
			else
			{
				LOG_WARNING << "Online load requested but no online manager!" << std::endl;
				loaded = false;
			}
		}

		if (isMode(Mode::OFFLINE))
		{
			if (m_localdata)
			{
				++m_loaded;
				m_localdata->loadData(m_name, [=](std::vector<std::string> data){
					m_rawData = data.back(); // we only care about last doc ( most recent ? )
					//deprecated
					m_onLoading(data);
					--m_loaded;
					event_manager->emit<Loaded>(getId(),m_name, m_rawData);
				}, m_key);
			}
			else
			{
				LOG_WARNING << "Offline load requested but no offline manager!" << std::endl;
				loaded = false;
			}
		}

		return loaded;
	}

	bool Save::requestSaveData(std::string data)
	{
		bool saved = true;
		m_rawData = data;

		if (isMode(Mode::ONLINE)) //no encryption online
		{
			if (m_onlinedata)
			{
				++m_saved;
				m_onlinedata->save(m_user, m_name, m_docId, m_rawData, [=](std::string data)
				{
					//we do not modify local data to not lose more recent changes.
					--m_saved;
					//CCLOG("user data saved : %s", data.c_str());
					event_manager->emit<Saved>(getId(), m_name, data);
				},m_key);
			}
			else
			{
				LOG_WARNING << "Online save requested but no online manager!" << std::endl;
				saved = false;
			}
		}

		if (isMode(Mode::OFFLINE))
		{
			if (m_localdata)
			{
				++m_saved;
				m_localdata->saveData(m_name, m_rawData, m_key);
				event_manager->emit<Saved>(getId(), m_name, m_rawData);
				--m_saved;
			}
			else
			{
				LOG_WARNING << "Offline save requested but no offline manager!" << std::endl;
				saved = false;
			}
		}

		return saved;
		
	}

	bool Save::requestDeleteData()
	{
		bool deleteSave = true;
		if (isMode(Mode::ONLINE))
		{
			LOG_WARNING << "Online save deletion not supported!" << std::endl;
		}
		if (isMode(Mode::OFFLINE))
		{
			if (m_localdata)
			{
				m_localdata->deleteData(m_name, [=](std::string data){
					//ignoring data...
					event_manager->emit<Deleted>(getId());
				});
			}
			else
			{
				LOG_WARNING << "Offline deletion requested but no offline manager!" << std::endl;
				deleteSave = false;
			}
		}
		return deleteSave;
	}

}// namespace WkCocos