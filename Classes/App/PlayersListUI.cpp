#include "WkCocosApp/PlayersListUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "json/document.h"  

const std::string PlayersListUI::id = "players_list";

PlayersListUI::PlayersListUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	//m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		m_widgetSize = m_widget->getContentSize();

		m_refreshButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_refreshButton->addTouchEventListener(CC_CALLBACK_2(PlayersListUI::refreshCallback, this));
		m_refreshButton->setPosition(cocos2d::Vec2(m_widgetSize.width / 4, m_widgetSize.height / 5));
		m_widget->addChild(m_refreshButton);

		m_refreshLabel = cocos2d::ui::Text::create("page", "Thonburi", 21);
		m_refreshLabel->setPosition(m_refreshButton->getPosition() + cocos2d::Vec2(0, m_refreshButton->getContentSize().height));
		m_widget->addChild(m_refreshLabel);
	
		m_enemyData = cocos2d::ui::Text::create("... GEMS and ... GOLD", "Thonburi", 21);
		m_enemyData->setPosition(cocos2d::Vec2(m_widgetSize.width / 4, -m_widgetSize.height / 5));
		m_widget->addChild(m_enemyData);

		m_enemyLabel = cocos2d::ui::Text::create("player ... has", "Thonburi", 15);
		m_enemyLabel->setPosition(m_enemyData->getPosition() + cocos2d::Vec2(0, m_enemyData->getContentSize().height));
		m_widget->addChild(m_enemyLabel);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
	if (g_gameLogic->getPlayer().getOnlineDatamgr()) {
		g_gameLogic->getPlayer().getOnlineDatamgr()->getEventManager()->subscribe<WkCocos::OnlineData::Events::PlayersList>(*this);
		//g_gameLogic->getPlayer().getUsersKeyValue("currency.gold", 7711, m_quantity, m_offset);
		//g_gameLogic->getPlayer().getUsersFromTo("currency.gold", 1, 999999, m_quantity, m_offset);
	}
}

PlayersListUI::~PlayersListUI()
{
	std::map<std::string, cocos2d::ui::Text*>::iterator currentPTB = m_ptb.begin();
	for (; currentPTB != m_ptb.end(); ++currentPTB)
	{
		m_widget->removeChild(currentPTB->second);
		//delete currentPTB->second;
	}
	m_ptb.clear();
}

void PlayersListUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{	
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{	//this has bug if app42 respone has more than one record from one user
		std::map<std::string, cocos2d::ui::Text*>::iterator currentPTB;
		for (currentPTB = m_ptb.begin(); currentPTB != m_ptb.end(); ++currentPTB)
		{
			m_widget->removeChild(currentPTB->second);
			//delete currentPTB->second;
		}
		m_ptb.clear();

		if (m_pages)
		{
			m_offset += m_quantity;
			if (m_pages == m_offset / m_quantity)
				m_offset = 0;
		}
		g_gameLogic->getPlayer().getUsersFromTo("currency.gold", 1, 999999, m_quantity, m_offset);
	}
}

void PlayersListUI::receive(const WkCocos::OnlineData::Events::PlayersList &pl)
{
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
	{
		auto listSize = pl.recordCount;
		if (listSize)
		{
			auto tempList = pl.eventMessage;
			int i = 0;
			for (std::map<std::string, std::string>::iterator record = tempList.begin(); record != tempList.end(); ++record)
			{
				auto enemyName = record->first;
				cocos2d::ui::Text* playertextbutton = cocos2d::ui::Text::create("#" + WkCocos::ToolBox::itoa(i) + " " + enemyName, "Thonburi", 15);
				playertextbutton->setPosition(cocos2d::Vec2(-m_widgetSize.width / 4, m_widgetSize.height / 2 - 22 * (m_quantity / 2 + i++))); 
				playertextbutton->setTouchEnabled(true);

				rapidjson::Document jsonDoc;
				jsonDoc.Parse<0>(record->second.c_str());

				rapidjson::Value& temp = jsonDoc["currency"];
				std::string gems = WkCocos::ToolBox::itoa(temp["gem"].GetInt());
				std::string gold = WkCocos::ToolBox::itoa(temp["gold"].GetInt());

				playertextbutton->addTouchEventListener
				(
					[=](cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
					{
						if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
						{
							m_enemyData->setText(gems + " GEMS and " + gold + " GOLD");
							m_enemyLabel->setText("player " + enemyName + " has");
						}
					}
				);

				m_ptb[enemyName] = playertextbutton;
				m_widget->addChild(playertextbutton);
			}
		}
		m_pages = (int)round((float)listSize / (float)m_quantity);
		m_refreshLabel->setText("page " + WkCocos::ToolBox::itoa(m_offset / m_quantity + 1) + "/" + WkCocos::ToolBox::itoa(m_pages));
	});
}