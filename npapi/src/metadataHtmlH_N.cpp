/*
 * Copyright 2008-2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "esnpapi.h"
#include "proxyImpl.h"

#include <org/w3c/dom.h>

using namespace org::w3c::dom;

void initializeHtmlMetaDataH_N()
{
    ProxyControl::registerMetaData(html::HTMLHeadElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLHeadElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLHeadingElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLHeadingElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLHRElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLHRElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLHtmlElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLHtmlElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLIFrameElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLIFrameElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLImageElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLImageElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLInputElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLInputElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLKeygenElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLKeygenElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLLabelElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLLabelElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLLegendElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLLegendElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLLIElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLLIElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLLinkElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLLinkElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMapElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMapElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMarqueeElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMarqueeElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMediaElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMediaElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMenuElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMenuElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMetaElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMetaElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLMeterElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLMeterElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLModElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLModElement_Bridge<Any, invoke> >::createInstance);
}
