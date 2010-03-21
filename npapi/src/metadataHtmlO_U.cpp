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

void initializeHtmlMetaDataO_U()
{
    registerMetaData(html::HTMLObjectElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLObjectElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLOListElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLOListElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLOptGroupElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLOptGroupElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLOptionElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLOptionElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLOptionsCollection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLOptionsCollection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLOutputElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLOutputElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLParagraphElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLParagraphElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLParamElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLParamElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLPreElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLPreElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLProgressElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLProgressElement_Bridge<Any, invoke> >::createInstance));
    // TODO: HTMLPropertiesCollection overrides HTMLCollection in a way the current Bridge classes can not handle.
    // registerMetaData(html::HTMLPropertiesCollection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLPropertiesCollection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLQuoteElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLQuoteElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLScriptElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLScriptElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLSelectElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLSelectElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLSourceElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLSourceElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLSpanElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLSpanElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLStyleElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLStyleElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableCaptionElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableCaptionElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableCellElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableCellElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableColElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableColElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableDataCellElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableDataCellElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableHeaderCellElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableHeaderCellElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableRowElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableRowElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTableSectionElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTableSectionElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTextAreaElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTextAreaElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTimeElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTimeElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLTitleElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLTitleElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLUListElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLUListElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLUnknownElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLUnknownElement_Bridge<Any, invoke> >::createInstance));
}
