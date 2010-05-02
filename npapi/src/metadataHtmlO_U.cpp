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

void initializeHtmlMetaDataO_U()
{
    ProxyControl::registerMetaData(html::HTMLObjectElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLObjectElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLOListElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLOListElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLOptGroupElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLOptGroupElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLOptionElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLOptionElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLOptionsCollection::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLOptionsCollection_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLOutputElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLOutputElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLParagraphElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLParagraphElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLParamElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLParamElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLPreElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLPreElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLProgressElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLProgressElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLPropertiesCollection::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLPropertiesCollection_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLQuoteElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLQuoteElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLScriptElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLScriptElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLSelectElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLSelectElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLSourceElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLSourceElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLSpanElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLSpanElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLStyleElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLStyleElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableCaptionElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableCaptionElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableCellElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableCellElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableColElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableColElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableDataCellElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableDataCellElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableHeaderCellElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableHeaderCellElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableRowElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableRowElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTableSectionElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTableSectionElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTextAreaElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTextAreaElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTimeElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTimeElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLTitleElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLTitleElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLUListElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLUListElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(html::HTMLUnknownElement::getMetaData(), Proxy_Impl<ProxyObject, html::HTMLUnknownElement_Bridge<Any, invoke> >::createInstance);
}
