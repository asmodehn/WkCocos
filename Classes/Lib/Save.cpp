#include "WkCocos/Save.h"

#include "WkCocos/Utils/log/logstream.h"

namespace WkCocos
{
	Save::Save(const std::string& saveName, Mode mode, std::string key)
		: m_name(saveName)
		, m_key(key)
		, m_rawData("")
		, m_onLoading()
		, m_onSaving()
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
				m_current_load = m_onlinedata->load(m_user, m_name, [=](std::string docId, std::vector<std::string> data)
				{
					m_current_load = entityx::Entity::Id();
					m_docId = docId;
					if (!data.empty())
					{
						m_rawData = data.back();
					}
					else
					{
						m_rawData = "";
					}
					//deprecated
					if ( m_onLoading) m_onLoading(m_rawData);
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
					if (m_onLoading) m_onLoading(m_rawData);
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

		LOG_WARNING << "requested save of " << m_rawData << std::endl;

		if (isMode(Mode::ONLINE)) //no encryption online
		{
			if (m_onlinedata)
			{
				++m_saved;
				if (m_saved == 1) // we process first save only. others will be queued ( and intermediate will be skipped as useless )
				{
					if (m_docId.size() > 0)
					{
						m_current_save = m_onlinedata->save(m_user, m_name, m_docId, m_rawData, [=](std::string saveName, std::string docId, std::string data)
						{
							m_current_save = entityx::Entity::Id();
							//we do not modify local data to not lose more recent changes.
							if (--(this->m_saved) == 0) // if last answer came back
							{
								//CCLOG("user data saved : %s", data.c_str());
								this->event_manager->emit<Saved>(this->getId(), this->m_name, data);
							}
							else //if we have many saves queued, we cancel all except last one, and we process it
							{
								this->m_saved = 0;
								this->requestSaveData(m_rawData);
							}
						}, m_key);
					}
					else
					{
						m_current_save =  m_onlinedata->saveNew(m_user, m_name, m_rawData, [=](std::string saveName, std::string docId, std::string data)
						{
							m_current_save = entityx::Entity::Id();
							//storing docId to mark creation of save
							this->m_docId = docId;

							//we do not modify local data to not lose more recent changes.
							if (--(this->m_saved) == 0) // if last answer came back
							{
								//CCLOG("user data saved : %s", data.c_str());
								this->event_manager->emit<Saved>(this->getId(), this->m_name, data);
							}
							else //if we have many saves queued, we cancel all except last one, and we process it
							{
								this->m_saved = 0;
								this->requestSaveData(m_rawData);
							}
						}, m_key);
					}
				}
			}
			else
			{
				LOG_WARNING << "Online save requested but no online manager!" << std::endl;
				saved = false;
			}
		}
		else if (isMode(Mode::OFFLINE))
		{
			if (m_localdata)
			{
				//no queue for local save at the moment as local save is assumed (wrongly) synchronous for now.
				++m_saved;
				m_localdata->saveData(m_name, m_rawData, m_key);
				//BUG : Save is not yet saved here...
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
			//TODO
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


	/**
	* Set local data manager
	*/
	void Save::setLocalDataMgr(std::shared_ptr<LocalData::LocalDataManager> localdata)
	{
		m_localdata = localdata; 
	}

	/**
	* Set online  data manager
	*/
	void Save::setOnlineDataMgr(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata)
	{
		m_onlinedata = onlinedata; 
		//hoking us up to errors event
		m_onlinedata->getEventManager()->subscribe<OnlineData::Events::Error>(*this);
	}


	void Save::receive(const WkCocos::OnlineData::Events::Error& err)
	{
		if (err.id == m_current_load)
		{
			m_current_load = entityx::Entity::Id();// load finished
			//current load requests has errored
			if (--(m_loaded) == 0) // if last answer came back
			{
				//CCLOG("user data saved : %s", data.c_str());
				event_manager->emit<Error>(this->getId(), ErrorType::LOAD_UNKNOWN_ERROR); //TODO : check for and trigger more errors type
			}
			else //if we have many saves queued, we cancel all except last one, and we process it
			{
				m_loaded = 0;
				requestLoadData();
				//no need to trigger errors here, if a saves succeed we are fine...
			}
		}

		if (err.id == m_current_save)
		{
			m_current_save = entityx::Entity::Id();// save finished
			//current save requests has errored
			if (--(m_saved) == 0) // if last answer came back
			{
				//CCLOG("user data saved : %s", data.c_str());
				event_manager->emit<Error>(this->getId(), ErrorType::SAVE_UNKNOWN_ERROR); //TODO : check for and trigger more errors type
			}
			else //if we have many saves queued, we cancel all except last one, and we process it
			{
				m_saved = 0;
				requestSaveData(m_rawData);
				//no need to trigger errors here, if a saves succeed we are fine...
			}
		}
	}

}// namespace WkCocos