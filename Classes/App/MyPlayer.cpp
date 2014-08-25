#include "WkCocosApp/MyPlayer.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::shared_ptr<WkCocos::Shop::Inventory> shopInventory)
: WkCocos::Player<MyPlayer>(localdatamngr, shopInventory)
, m_gem(42)
, m_gold(424242)
{
}

MyPlayer::~MyPlayer()
{
}


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
	else //first time there is no save. we need sensible values.
	{
		//these should be set in constructor
	}
	
}
