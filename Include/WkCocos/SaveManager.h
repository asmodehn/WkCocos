#ifndef __WKCOCOS_SAVEMANAGER_H__
#define __WKCOCOS_SAVEMANAGER_H__

#include "WkCocos/Save.h"
#include "WkCocos/Utils/GPGSManager.h"

#include <vector>

namespace WkCocos
{
	/**
	* Manage save.
	* Bridge with google save.
	*/
	class SaveManager : public entityx::Receiver<SaveManager>
	{
	public:
		struct SaveInfo
		{
			std::string					m_desc;
			std::chrono::milliseconds	m_timePlayedms;
			std::vector<uint8_t>		m_icon;
		};

	public:
		/**
		* Default constructor
		*/
		SaveManager();

		/**
		* Destructor
		*/
		~SaveManager();

		/**
		* Configure event
		*/
		void configure();

		/**
		* Take all save and compact them as one binary data.
		*/
		void createBackup();

		/**
		* Split binary save and dispatch to each.
		*/
		void dispatchBackup(const std::vector<uint8_t>& snapshot);

		/**
		* Add a save to manage.
		*/
		inline void manageSave(Save* save){ m_managedSave.push_back(save); }

		/**
		* receive snapshot save requested
		*/
		void receive(const GPGSManager::SnapshotSaveRequested&);

		/**
		* receive snapshot loaded
		*/
		void receive(const GPGSManager::SnapshotLoaded& evt);

		/**
		* Set description callback
		*/
		inline void setDescCallback(std::function<void(SaveInfo&)> cb) { m_descCallback = cb; }

		/**
		* Set the save name
		*/
		inline void setSaveName(const std::string& name) { m_saveName = name; }

	private:
		/**
		* Name of the save
		*/
		std::string			m_saveName;

		/**
		* Vector of save managed
		*/
		std::vector<Save*> m_managedSave;

		/**
		* Function callback to get the description
		*/
		std::function<void(SaveInfo&)>		m_descCallback;
	};

}//namespace WkCocos

#endif //__WKCOCOS_SAVEMANAGER_H__