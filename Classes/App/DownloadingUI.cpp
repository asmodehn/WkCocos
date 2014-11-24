#include "WkCocosApp/DownloadingUI.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Utils/log/logstream.h"

//TMP
#include "WkCocos/Download/Systems/ProgressUpdate.h"

#define MANIFEST_FILENAME "manifest.json"

const std::string DownloadingUI::id = "downloading";

DownloadingUI::DownloadingUI()
	: Interface()
	, m_dlStarted(false)
	, m_dlmgr (new WkCocos::Download::Download(5,std::function<void(float)>()))

{
    m_dlmgr->getEventManager()->subscribe<WkCocos::Download::Events::DownloadOptions>(*this);
    m_dlmgr->getEventManager()->subscribe<WkCocos::Download::Events::DownloadAdvised>(*this);

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		//loadingbar
		auto loadingbar = cocos2d::ui::LoadingBar::create("loadingbar.png", 0.5f);
		loadingbar->setName("LoadingBar");
		m_widget->addChild(loadingbar);

        //BIG button to start download
        auto dlLabel = cocos2d::ui::Text::create(MANIFEST_FILENAME, "Thonburi", 21);
        auto button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		button->addTouchEventListener(CC_CALLBACK_2(DownloadingUI::DLCallback, this));
		button->setPosition(cocos2d::Vec2(button->getContentSize().width / 2, -button->getContentSize().height / 2));
        dlLabel->setPosition(button->getPosition() + cocos2d::Vec2(0, button->getContentSize().height));
        m_widget->addChild(button);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

DownloadingUI::~DownloadingUI()
{
	if (m_dlmgr)
		delete m_dlmgr, m_dlmgr = nullptr;
}

void DownloadingUI::DLCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
    if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (m_dlmgr)
        {
            m_dlmgr->addDataDownload(MANIFEST_FILENAME);
            m_dlStarted = true;
        }
    }
}

void DownloadingUI::receive(const WkCocos::Download::Events::DownloadOptions &dlo)
{
	LOG_DEBUG << "Received DownloadOptions Event !" << std::endl;

	//TODO : display the list of possible URL, with special color for the force update if there is one
	//dlo.
}

void DownloadingUI::receive(const WkCocos::Download::Events::DownloadAdvised &dla)
{
	LOG_DEBUG << "Received DownloadAdvised Event : " << dla.m_version.toString() << std::endl;

	//if we started the advised download already : display the url and version.
	if ( dla.m_started)
    {
        //TODO
    }
}

void DownloadingUI::update(float delta)
{
    //update should do something only if the interface is visible
    if ( isVisible() )
    {
        if ( m_dlmgr && m_dlStarted )
        {
            m_dlmgr->update(delta);

            ///TMP while progress is not an event...
            std::pair<int,int> downCurProgVal = m_dlmgr->getCurrentProgress();

            float pct = 1.0f;
            if (downCurProgVal.second > 0)
            {
                pct -= (float)(downCurProgVal.first) / (float)(downCurProgVal.second);
            }

			cocos2d::ui::LoadingBar* loadbar = getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar");
			if (loadbar)
            {
				loadbar->setPercent(pct * 100.0f);
            }

            if (pct >= 1.0f)
            {
                //we've finished downloading
                m_dlStarted = false;
                //Purging file path cache ( same files might have been added with different paths )
                cocos2d::FileUtils::getInstance()->purgeCachedEntries();
            }

        }
    }
}
