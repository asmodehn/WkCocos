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
void MyPlayer::get_data_json(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value vholder;
	vholder.SetObject();
	vholder.AddMember("gold", m_gold, allocator);
	vholder.AddMember("gem", m_gem, allocator);
	doc.AddMember("currency", vholder, allocator);
}

/**
* data setter from json string
*/
void MyPlayer::set_data_json(rapidjson::Document& doc)
{
	if (doc.HasMember("currency"))
	{
		rapidjson::Value& currencyvalue = doc["currency"];
		if (!currencyvalue.IsNull()){
			if (currencyvalue.HasMember("gold"))
			{
				m_gold = currencyvalue["gold"].GetInt();
			}
			if (currencyvalue.HasMember("gem"))
			{
				m_gem = currencyvalue["gem"].GetInt();
			}
		}
	}
	else //first time there is no save
	{
		m_gold = 424242;
		m_gem = 42;
	}
}
