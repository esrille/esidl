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

#include <any.h>
#include <reflect.h>
#include <org/w3c/dom.h>

using namespace org::w3c::dom;

void initializeHtmlMetaDataA_G()
{
    registerMetaData(html::HTMLAllCollection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLAllCollection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLAnchorElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLAnchorElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLAppletElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLAppletElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLAreaElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLAreaElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLAudioElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLAudioElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLBaseElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLBaseElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLBaseFontElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLBaseFontElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLBodyElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLBodyElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLBRElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLBRElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLButtonElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLButtonElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLCanvasElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLCanvasElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLCollection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLCollection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLCommandElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLCommandElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLDataListElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDataListElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLDetailsElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDetailsElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLDirectoryElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDirectoryElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLDivElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDivElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLDListElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDListElement_Bridge<Any, invoke> >::createInstance));
    // registerMetaData(html::HTMLDocument::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLDocument_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLEmbedElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLEmbedElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFieldSetElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFieldSetElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFontElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFontElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFormControlsCollection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFormControlsCollection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFormElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFormElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFrameElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFrameElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLFrameSetElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLFrameSetElement_Bridge<Any, invoke> >::createInstance));
}
