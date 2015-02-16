/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "cocos/base/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "WkCocos/WebView/UIWebViewImpl-linux.h"

#include <unordered_map>
#include <stdlib.h>
#include <string>

#include "WkCocos/WebView/UIWebView.h"
#include "cocos/platform/CCGLView.h"
#include "cocos/base/CCDirector.h"
#include "cocos/platform/CCFileUtils.h"

#define CLASS_NAME "com/asmodehn/wkcocos/lib/Cocos2dxWebViewHelper"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"",__VA_ARGS__)

namespace cocos2d {
    namespace experimental {
        namespace ui{

            static std::unordered_map<int, cocos2d::experimental::ui::WebViewImpl*> s_WebViewImpls;

            WebViewImpl::WebViewImpl(cocos2d::experimental::ui::WebView *webView)
            {}

            WebViewImpl::~WebViewImpl()
            {}

            void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme)
            {}

            void WebViewImpl::loadData(const cocos2d::Data &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL)
            {}

            void WebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL)
            {}

            void WebViewImpl::loadURL(const std::string &url)
            {}

            void WebViewImpl::loadFile(const std::string &fileName)
            {}

            void WebViewImpl::stopLoading()
            {}

            void WebViewImpl::reload()
            {}

            bool WebViewImpl::canGoBack()
            {}

            bool WebViewImpl::canGoForward()
            {}

            void WebViewImpl::goBack()
            {}

            void WebViewImpl::goForward()
            {}

            void WebViewImpl::evaluateJS(const std::string &js)
            {}

            void WebViewImpl::setScalesPageToFit(const bool scalesPageToFit)
            {}

            void WebViewImpl::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags)
            {}

            void WebViewImpl::setVisible(bool visible)
            {}

            bool WebViewImpl::shouldStartLoading(const int viewTag, const std::string &url)
            { return false;}

            void WebViewImpl::didFinishLoading(const int viewTag, const std::string &url)
            {}

            void WebViewImpl::didFailLoading(const int viewTag, const std::string &url)
            {}

            void WebViewImpl::onJsCallback(const int viewTag, const std::string &message)
            {}



        } // namespace ui
    } // namespace experimental
} //namespace cocos2d

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
