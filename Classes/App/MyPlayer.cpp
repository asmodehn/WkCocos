#include "WkCocosApp/MyPlayer.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr)
: WkCocos::Player<MyPlayer>(localdatamngr)
{
}

MyPlayer::~MyPlayer()
{}


/**
* data accessor as json string
*/
std::string MyPlayer::get_data_json()
{
	rapidjson::Document doc;
	doc.SetObject();
	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value currency;
	currency.SetObject();
	currency.AddMember(sGold, m_gold, allocator);
	currency.AddMember(sGem, m_gem, allocator);
	doc.AddMember(sCurrency, currency, allocator);

	rapidjson::Value alarms;
	alarms.SetArray();
	
	entityx::ptr<WkCocos::Timer::Comp::ID> id;
	entityx::ptr<WkCocos::Timer::Comp::Alarm> alarm;
	for (auto entity : m_timer->getEntityManager()->entities_with_components(id, alarm))
	{
		rapidjson::Value time;
		time.SetObject();
		time.AddMember(sID, id->m_id.c_str(), allocator);
		time.AddMember(sTime, (uint64_t)mktime(&alarm->m_end), allocator);
		alarms.PushBack(time, allocator);
	}
	doc.AddMember(sAlarms, alarms, allocator);

	//TMP debug
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	doc.Accept(writer);

	return std::string(strbuf.GetString());
}

/**
* data setter from json string
*/
void MyPlayer::set_data_json(std::string data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (doc.HasParseError())
	{
		//if parse error (also empty string), we ignore existing data.
		doc.SetObject();
	}

	if (doc.HasMember(sCurrency))
	{
		rapidjson::Value& currencyvalue = doc[sCurrency];
		if (!currencyvalue.IsNull()){
			if (currencyvalue.HasMember(sGold))
			{
				m_gold = currencyvalue[sGold].GetInt();
			}
			if (currencyvalue.HasMember(sGem))
			{
				m_gem = currencyvalue[sGem].GetInt();
			}
		}
	}
	else //first time there is no save
	{
		m_gold = 424242;
		m_gem = 42;
	}
	
	if (doc.HasMember(sAlarms))
	{
		rapidjson::Value& alarmsarray = doc[sAlarms];
		if (alarmsarray.Size()){
			for (rapidjson::SizeType i = 0; i < alarmsarray.Size(); i++)
			{
				rapidjson::Value& time = alarmsarray[i];
				time_t temptime_t = time[sTime].GetUint64();
				struct tm * temptm = gmtime(&temptime_t);
				m_timer->setAlarm(time[sID].GetString(), *temptm);
			}	
		}
	}
}