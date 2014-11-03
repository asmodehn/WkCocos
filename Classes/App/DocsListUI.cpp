#include "WkCocosApp/DocsListUI.h"
#include "WkCocosApp/GameLogic.h"

const std::string DocsListUI::id = "docs_list";

DocsListUI::DocsListUI()
	: Interface()
{
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{
		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		m_widgetSize = m_widget->getContentSize();

		auto refreshButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		refreshButton->addTouchEventListener(CC_CALLBACK_2(DocsListUI::refreshCallback, this));
		refreshButton->setPosition(cocos2d::Vec2(m_widgetSize.width / 5, (refreshButton->getContentSize().height - m_widgetSize.height) / 2));
		m_widget->addChild(refreshButton);

		m_refreshLabel = cocos2d::ui::Text::create("page", "Thonburi", 21);
		m_refreshLabel->setPosition(cocos2d::Vec2(0, (refreshButton->getContentSize().height - m_widgetSize.height) / 2));
		m_widget->addChild(m_refreshLabel);

		auto docIDLabel = cocos2d::ui::Text::create("Document ID", "Thonburi", 21);
		docIDLabel->setPosition(cocos2d::Vec2(-m_widgetSize.width / 5 * 2, (m_widgetSize.height - refreshButton->getContentSize().height) / 2));
		m_widget->addChild(docIDLabel);

		auto jsonDocLabel = cocos2d::ui::Text::create("JSON Document", "Thonburi", 21);
		jsonDocLabel->setPosition(cocos2d::Vec2(-m_widgetSize.width / 5, (m_widgetSize.height - refreshButton->getContentSize().height) / 2));
		m_widget->addChild(jsonDocLabel);

		auto ownerLabel = cocos2d::ui::Text::create("Owner", "Thonburi", 21);
		ownerLabel->setPosition(cocos2d::Vec2(0, (m_widgetSize.height - refreshButton->getContentSize().height) / 2));
		m_widget->addChild(ownerLabel);

		auto createdLabel = cocos2d::ui::Text::create("Created On", "Thonburi", 21);
		createdLabel->setPosition(cocos2d::Vec2(m_widgetSize.width / 5, (m_widgetSize.height - refreshButton->getContentSize().height) / 2));
		m_widget->addChild(createdLabel);

		auto updatedLabel = cocos2d::ui::Text::create("Updated On", "Thonburi", 21);
		updatedLabel->setPosition(cocos2d::Vec2(m_widgetSize.width / 5 * 2, (m_widgetSize.height - refreshButton->getContentSize().height) / 2));
		m_widget->addChild(updatedLabel);

	}

	if (g_gameLogic->getPlayer().getOnlineDatamgr())
	{
		g_gameLogic->getPlayer().getOnlineDatamgr()->getEventManager()->subscribe<WkCocos::OnlineData::Events::DocsList>(*this);
	}

}

void DocsListUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{	
		for (unsigned int i = 0; i != m_table.size(); ++i)
		{
			std::map<std::string, cocos2d::ui::Text*>::iterator currentText;
			for (currentText = m_table[i].begin(); currentText != m_table[i].end(); ++currentText)
			{
				m_widget->removeChild(currentText->second);
				//delete currentPTB->second;
			}
			m_table[i].clear();
		}
		m_table.clear();
		m_user_request = true;
		g_gameLogic->getPlayer().getAllDocsPaging(m_quantity, m_offset);
	}
}

void DocsListUI::receive(const WkCocos::OnlineData::Events::DocsList &doclist)
{
	m_offset += m_quantity;
	auto tempVector = doclist.eventMessage;
	if (tempVector.size())
	{
		for (unsigned int i = 0; i != tempVector.size(); ++i)
		{
			std::map<std::string, cocos2d::ui::Text*> line;
			std::string temp;

			temp = tempVector[i]["Document_ID"];
			if (temp.size() > 20)
			{
				temp.resize(16);
				temp = temp + "...";
			}
			cocos2d::ui::Text* docIdText = cocos2d::ui::Text::create(temp, "Thonburi", 15);
			docIdText->setPosition(cocos2d::Vec2(-m_widgetSize.width / 5 * 2, m_widgetSize.height / 2 - 22 - 20 * (m_quantity / 2 + i)));
			line["Document_ID"] = docIdText;
			m_widget->addChild(docIdText);

			temp = tempVector[i]["JSON_Document"];
			if (temp.size() > 20)
			{
				temp.resize(16);
				temp = temp + "...";
			}
			cocos2d::ui::Text* jsonDocText = cocos2d::ui::Text::create(temp, "Thonburi", 15);
			jsonDocText->setPosition(cocos2d::Vec2(-m_widgetSize.width / 5, m_widgetSize.height / 2 - 22 - 20 * (m_quantity / 2 + i)));
			line["JSON_Document"] = jsonDocText;
			m_widget->addChild(jsonDocText);

			temp = tempVector[i]["Owner"];
			if (temp.size() > 20)
			{
				temp.resize(16);
				temp = temp + "...";
			}
			cocos2d::ui::Text* ownerText = cocos2d::ui::Text::create(temp, "Thonburi", 15);
			ownerText->setPosition(cocos2d::Vec2(0, m_widgetSize.height / 2 - 22 - 20 * (m_quantity / 2 + i)));
			line["Owner"] = ownerText;
			m_widget->addChild(ownerText);

			temp = tempVector[i]["Created_On"];
			if (temp.size() > 20)
			{
				temp.resize(16);
				temp = temp + "...";
			}
			cocos2d::ui::Text* createdText = cocos2d::ui::Text::create(temp, "Thonburi", 15);
			createdText->setPosition(cocos2d::Vec2(m_widgetSize.width / 5, m_widgetSize.height / 2 - 22 - 20 * (m_quantity / 2 + i)));
			line["Created_On"] = createdText;
			m_widget->addChild(createdText);

			temp = tempVector[i]["Updated_On"];
			if (temp.size() > 20)
			{
				temp.resize(16);
				temp = temp + "...";
			}
			cocos2d::ui::Text* updatedText = cocos2d::ui::Text::create(temp, "Thonburi", 15);
			updatedText->setPosition(cocos2d::Vec2(m_widgetSize.width / 5 * 2, m_widgetSize.height / 2 - 22 - 20 * (m_quantity / 2 + i)));
			line["Updated_On"] = updatedText;
			m_widget->addChild(updatedText);

			m_table.push_back(line);
		}
		m_refreshLabel->setString("page " + WkCocos::ToolBox::itoa(++m_pages));
	}
	else
	{
		m_pages = 0;
		m_offset = 0;
		if (m_user_request)
		{
			m_user_request = false;
			g_gameLogic->getPlayer().getAllDocsPaging(m_quantity, m_offset);
		}
		else
		{
			m_refreshLabel->setString("no documents found");
		}
	}
}

DocsListUI::~DocsListUI()
{
	for (unsigned int i = 0; i != m_table.size(); ++i)
	{
		std::map<std::string, cocos2d::ui::Text*>::iterator currentText;
		for (currentText = m_table[i].begin(); currentText != m_table[i].end(); ++currentText)
		{
			m_widget->removeChild(currentText->second);
			//delete currentPTB->second;
		}
		m_table[i].clear();
	}
	m_table.clear();
}