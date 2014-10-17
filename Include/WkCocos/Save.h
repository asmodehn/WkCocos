#ifndef __WKCOCOS_SAVE_H__
#define __WKCOCOS_SAVE_H__

#include <string>
#include <functional>
#include <cstdint>
#include <bitset>

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

namespace WkCocos
{
	/**
	* Save Object.
	*/
	class Save
	{
	public:
		enum class Mode : std::int32_t
		{
			OFFLINE = 0,
			ONLINE,
			END
		};

	public:
		/**
		* Default constructor
		*/
		Save() = delete;

		/**
		* Constructor
		* @param saveName Will be used to create the file
		* @param mode 
		*/
		Save(const std::string& saveName, Mode mode);

		/**
		* Destructor
		*/
		virtual ~Save();
		
		/**
		* Request data to be loaded
		*/
		bool requestLoadData(std::function<void()> loaded_cb, std::string key = "");

		/**
		* Request data to be saved
		*/
		bool requestSaveData(std::function<void()> saved_cb, std::string key = "");

		/**
		* Request data to be saved
		*/
		bool requestDeleteData(std::function<void()> delete_cb);

		/**
		* Register callback filling the save string
		*/
		inline void registerSavingCallback(std::function<std::string()> onSaving) { m_onSaving = onSaving; }

		/**
		* Register callback emptying the save string
		*/
		inline void registerLoadingCallback(std::function<void(std::string)> onLoading) { m_onLoading = onLoading; }

		/**
		* Test if a mode is active
		*/
		inline bool isMode(Mode mode) const { return m_saveModes[static_cast<int>(mode)]; }

		/**
		* Set local data manager
		*/
		inline void setLocalDataMgr(std::shared_ptr<LocalData::LocalDataManager> localdata) { m_localdata = localdata; }

		/**
		* Set online  data manager
		*/
		inline void setOnlineDataMgr(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata) { m_onlinedata = onlinedata; }

		/**
		* Set user name
		*/
		inline void setUserName(const std::string& userName){ m_user = userName; }

		inline std::string getSaveName() const { return m_name; }
		inline void setSaveName(const std::string& name){ m_name = name; }

	private:
		/**
		* Name of the save
		*/
		std::string				m_name;

		/**
		* User linked to the save
		*/
		std::string				m_user;

		/**
		* Save mode, so far, Online, Offline or both
		*/
		std::bitset<static_cast<int>(Mode::END)>		m_saveModes;

		/**
		* The local data manager, to make local call for save/load
		*/
		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		
		/**
		* The online data manager, to make online call for save/load
		*/
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;

		/**
		* flag to specify if the save has already been requested
		*/
		bool m_saveInProgress;

		/**
		* flag to specify if the load has already been requested
		*/
		bool m_loadInProgress;

		/**
		* Callback function to fill up save
		*/
		std::function<std::string()>		m_onSaving;

		/**
		* Callback function to empty save
		*/
		std::function<void(std::string)>	m_onLoading;
	};

}// namespace WkCocos

#endif //__WKCOCOS_SAVE_H__