#include "WkCocos/Save.h"

#include "WkCocos/Utils/log/logstream.h"

namespace WkCocos
{
	Save::Save(const std::string& saveName, Mode mode)
		: m_name(saveName)
		, m_onLoading(nullptr)
		, m_onSaving(nullptr)
	{
		m_saveModes[static_cast<int>(mode)] = 1;
	}
	
	Save::~Save()
	{}

	bool Save::requestLoadData(std::function<void()> loaded_cb, std::string key)
	{
		bool loaded = true;

		if (isMode(Mode::ONLINE)) //no encryption online
		{
			if (m_onlinedata)
			{
				m_onlinedata->load(m_user, m_name, [=](std::map<std::string, std::string> data)
				{
					m_onLoading(data);
					//CCLOG("user data loaded : %s", data.c_str());

					if (loaded_cb) loaded_cb();
				});
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
				LOG_WARNING << "Offline load temporary disabled due to multiply docs unfinished implementation!" << std::endl;
				m_localdata->loadData(m_name, [=](std::map<std::string, std::string> data){
					m_onLoading(data);

					if (loaded_cb) loaded_cb();
				}, key);
			}
			else
			{
				LOG_WARNING << "Offline load requested but no offline manager!" << std::endl;
				loaded = false;
			}
		}

		return loaded;
	}

	bool Save::requestSaveData(std::function<void()> saved_cb, std::string key)
	{
		bool loaded = true;

		if (isMode(Mode::ONLINE)) //no encryption online
		{
			if (m_onlinedata)
			{
				m_onlinedata->save(m_user, m_name, m_onSaving(), [=](std::string data)
				{
					//CCLOG("user data saved : %s", data.c_str());
					saved_cb();
				});
			}
			else
			{
				LOG_WARNING << "Online save requested but no online manager!" << std::endl;
				loaded = false;
			}
		}

		if (isMode(Mode::OFFLINE))
		{
			if (m_localdata)
			{
				LOG_WARNING << "Offline save temporary disabled due to multiply docs unfinished implementation!" << std::endl;
				m_localdata->saveData(m_name, m_onSaving(), key);
				saved_cb();
			}
			else
			{
				LOG_WARNING << "Offline save requested but no offline manager!" << std::endl;
				loaded = false;
			}
		}

		return loaded;
		
	}

	bool Save::requestDeleteData(std::function<void()> delete_cb)
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
				m_localdata->deleteData(m_name, [](std::string){});
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