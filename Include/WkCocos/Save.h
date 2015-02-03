#ifndef __WKCOCOS_SAVE_H__
#define __WKCOCOS_SAVE_H__

#include <string>
#include <functional>
#include <cstdint>
#include <bitset>

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

#include "WkCocos/Actor.h"
namespace WkCocos
{
	/**
	* Save Object.
	*/
	class Save : public Actor
	{
	public:
		enum class Mode : std::int32_t
		{
			OFFLINE = 0,
			ONLINE,
			END
		};

		//to manage async callbacks
		//entityx::EventManager save_events;

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
		Save(const std::string& saveName, Mode mode, std::string encrypt_key = "");

		/**
		* Destructor
		*/
		virtual ~Save();

		bool isLoaded() const {
			return 0 == m_loaded;
		}

		bool isSaved() const {
			return 0 == m_saved;
		}

		enum class ErrorType{
			LOAD_UNKNOWN_ERROR,
			SAVE_UNKNOWN_ERROR,
			LOAD_TIMEOUT_ERROR,
			SAVE_TIMEOUT_ERROR
		};

		//Error Event : for asynchronous errors only
		struct Error : Event < Error >
		{
			Error(ActorID id, ErrorType errt)
				: Event(id)
				, error_type(errt)
			{
			}

			ErrorType error_type;
		};

		struct Loading : Event < Loading >
		{
			Loading(ActorID senderId, const std::string& name, const std::string& data)
			: Event(senderId)
			, m_name(name)
			, m_data(data)
			{
			}

			std::string m_name;
			std::string m_data;
		};

		struct Saving : Event < Saving >
		{
			Saving(ActorID id, const std::string& name, const std::string& data)
			: Event(id)
			, m_name(name)
			, m_data(data)
			{
			}

			std::string m_name;
			std::string m_data;
		};

		struct Loaded : Event < Loaded >
		{
			Loaded(ActorID senderId, const std::string& name, const std::string& data)
			: Event(senderId)
			, m_name(name)
			, m_data(data)
			{
			}

			std::string m_name;
			std::string m_data;
		};


		struct Saved : Event < Saved >
		{
			Saved(ActorID id, const std::string& name, const std::string& data)
				: Event(id)
				, m_name(name)
				, m_data(data)
			{}

			std::string m_name;
			std::string m_data;
		};

		/**
		* Request data to be loaded
		* Will trigger Loaded or Error Event
		*/
		bool requestLoadData();

		/**
		* Request data to be saved.
		* this data will *replace* existing data
		* @ param saved_cb continuation callback
		*/
		bool requestSaveData(std::string data);

		/**
		* To ease migration
		* DEPRECATED
		*/
		WKCOCOS_DEPRECATED_ATTRIBUTE bool requestSaveData()
		{
			return requestSaveData(m_onSaving());
		}

		struct Deleted : Event < Deleted >
		{
			Deleted(ActorID id)
				:Event(id)
			{}
		};

		/**
		* Request data to be deleted
		* @ param delete_cb continuation callback
		*/
		bool requestDeleteData();

		/**
		* Register callback filling the save string
		* DEPRECATED
		*/
		WKCOCOS_DEPRECATED_ATTRIBUTE inline void registerSavingCallback(std::function<std::string()> onSaving) { m_onSaving = onSaving; }

		/**
		* Register callback emptying the save string
		* DEPRECATED
		*/
		WKCOCOS_DEPRECATED_ATTRIBUTE inline void registerLoadingCallback(std::function<void(std::string)> onLoading) { m_onLoading = onLoading; }

		/**
		* Test if a mode is active
		*/
		inline bool isMode(Mode mode) const { return m_saveModes[static_cast<int>(mode)]; }

		/**
		* Test if a mode is active
		*/
		inline void changeMode(Mode mode) 
		{ 
			m_saveModes.reset();
			m_saveModes[static_cast<int>(mode)].flip(); 
		}

		/**
		* Set local data manager
		*/
		void setLocalDataMgr(std::shared_ptr<LocalData::LocalDataManager> localdata);

		/**
		* Set online  data manager
		*/
		void setOnlineDataMgr(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata);

		/**
		* receiving errors from onlinedata manager
		*/
		void receive(const OnlineData::Events::Error& err);


		/**
		* Set user name
		*/
		inline void setUserName(const std::string& userName){ m_user = userName; }

		inline std::string getName() const { return m_name; }
		inline void setName(const std::string& name){ m_name = name; }

		inline std::string getData(){ return m_rawData; }
		inline std::string getKey() const { return m_key; }

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
		* DocId ( we fill it as soon as we know it )
		* not used for LocalSave.
		*/
		std::string				m_docId;

		/**
		* key for encrypted save
		*/
		std::string				m_key;

		/**
		* DocId ( we fill it as soon as we know it )
		* not used for LocalSave.
		*/
		std::string				m_rawData;

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
		* Callback function to fill up save
		*/
		WKCOCOS_DEPRECATED_ATTRIBUTE std::function<std::string()>		m_onSaving;

		/**
		* Callback function to empty save
		*/
		WKCOCOS_DEPRECATED_ATTRIBUTE std::function<void(std::string)>	m_onLoading;

		/**
		* incremented by load request, decremented by loaded response
		*/
		unsigned short m_loaded;
		entityx::Entity::Id m_current_load;

		/**
		* incremented by save request, decremented by saved response
		*/
		unsigned short m_saved;
		entityx::Entity::Id m_current_save;

#ifdef _DEBUG
		/**
		* Save request count
		*/
		int			m_saveRequest;

		/**
		* Save request success count
		*/
		int			m_saveSuccess;

		/**
		* Save request fail count
		*/
		int			m_saveFail;
#endif //_DEBUG
	};

}// namespace WkCocos

#endif //__WKCOCOS_SAVE_H__
