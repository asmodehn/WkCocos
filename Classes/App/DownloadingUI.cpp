#include "WkCocosApp/DownloadingUI.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Utils/log/logstream.h"

//TMP
#include "WkCocos/Download/Systems/ProgressUpdate.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "WkPlatform_WkCocos.h"
#endif

#include "WkCocos/Utils/WkJniHelper.h"

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

        //BIG button to start download
        auto dlLabel = cocos2d::ui::Text::create(MANIFEST_FILENAME, "Thonburi", 21);
        auto button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		button->addTouchEventListener(CC_CALLBACK_2(DownloadingUI::DLCallback, this));
		button->setPosition(-1 * widgetSize /2 + button->getContentSize());
        dlLabel->setPosition(button->getPosition() + cocos2d::Vec2(0, button->getContentSize().height));
        m_widget->addChild(button);
        m_widget->addChild(dlLabel);

		//loadingbar
		auto loadingbar = cocos2d::ui::LoadingBar::create("loadingbar.png", 0.5f);
		loadingbar->setName("LoadingBar");
		loadingbar->setPosition(button->getPosition() + cocos2d::Vec2(button->getContentSize().width + loadingbar->getContentSize().width ,0));
		m_widget->addChild(loadingbar);

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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		WkCocos::Download::Version curVersion("v" + std::string(WK_WkCocos_VERSION));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        WkCocos::Download::Version curVersion("v" + WkCocos::Utils::jni::Utils::getVersionName());
#endif
            m_dlmgr->addDataDownload(curVersion ,MANIFEST_FILENAME);
            m_dlStarted = true;
        }
    }
}

void DownloadingUI::receive(const WkCocos::Download::Events::DownloadOptions &dlo)
{
	LOG_DEBUG << "Received DownloadOptions Event !" << std::endl;

    int posincr = -40;
    int pos = posincr;

    //title //ERROR Thonburi doesnt have => or ->
    auto verdlTitle = cocos2d::ui::Text::create(to_string(dlo.m_current_version) + " upgrade to " + dlo.m_url, "Thonburi", 21);
    cocos2d::Vec2 posanchor = cocos2d::Vec2(0, m_widget->getContentSize().height /2);
    verdlTitle->setPosition(posanchor + cocos2d::Vec2(20,pos));
    m_widget->addChild(verdlTitle);
    pos+=posincr;

	//display the list of possible URL.
    for ( auto v : dlo.m_version_vec )
	{
	    auto verdlLabel = cocos2d::ui::Text::create(to_string(v), "Thonburi", 21);
	    if ( v == dlo.m_force_update)
	    {
            verdlLabel->setColor(cocos2d::Color3B::RED);
            verdlLabel->setString(verdlLabel->getString() + " FORCE UPDATE");
	    }
        verdlLabel->setPosition(posanchor + cocos2d::Vec2(20,pos));
        m_widget->addChild(verdlLabel);
        pos += posincr;
	}
}

void DownloadingUI::receive(const WkCocos::Download::Events::DownloadAdvised &dla)
{
	LOG_DEBUG << "Received DownloadAdvised Event : " << dla.m_version << std::endl;

	//if we started the advised download already : display the url and version.
	if ( dla.m_started)
    {
        auto lbar = getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar");
        auto verdlLabel = cocos2d::ui::Text::create(to_string(dla.m_version), "Thonburi", 21);
        verdlLabel->setPosition(lbar->getPosition());
        m_widget->addChild(verdlLabel);
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
