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

void initializeHtmlMetaData()
{
    registerMetaData(html::ApplicationCache::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::ApplicationCache_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::BarProp::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::BarProp_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::BeforeUnloadEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::BeforeUnloadEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::CanvasGradient::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::CanvasGradient_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::CanvasPattern::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::CanvasPattern_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::CanvasPixelArray::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::CanvasPixelArray_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::CanvasRenderingContext2D::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::CanvasRenderingContext2D_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::DataTransfer::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::DataTransfer_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::DOMSettableTokenList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::DOMSettableTokenList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::DOMStringMap::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::DOMStringMap_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::DOMTokenList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::DOMTokenList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::DragEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::DragEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Function::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Function_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::History::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::History_Bridge<Any, invoke> >::createInstance));
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
    registerMetaData(html::HTMLHeadElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLHeadElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLHeadingElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLHeadingElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLHRElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLHRElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLHtmlElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLHtmlElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLIFrameElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLIFrameElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLImageElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLImageElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLInputElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLInputElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLKeygenElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLKeygenElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLLabelElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLLabelElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLLegendElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLLegendElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLLIElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLLIElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLLinkElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLLinkElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMapElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMapElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMarqueeElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMarqueeElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMediaElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMediaElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMenuElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMenuElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMetaElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMetaElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLMeterElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLMeterElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::HTMLModElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLModElement_Bridge<Any, invoke> >::createInstance));
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
    registerMetaData(html::HTMLVideoElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::HTMLVideoElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::ImageData::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::ImageData_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Location::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Location_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::MediaError::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::MediaError_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::MessageChannel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::MessageChannel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::MessageEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::MessageEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::MessagePort::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::MessagePort_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Navigator::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Navigator_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::PopStateEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::PopStateEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::PropertyNodeList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::PropertyNodeList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::RadioNodeList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::RadioNodeList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Screen::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Screen_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Selection::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Selection_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::StyleMedia::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::StyleMedia_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::TextMetrics::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::TextMetrics_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::TimeRanges::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::TimeRanges_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::UndoManagerEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::UndoManagerEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::UndoManager::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::UndoManager_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::ValidityState::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::ValidityState_Bridge<Any, invoke> >::createInstance));
    registerMetaData(html::Window::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, html::Window_Bridge<Any, invoke> >::createInstance));

    // In HTML5, HTMLDocument is a mixin of Document, but historically it was a separate interface extended from Document.
    // Existing browsers uses HTMLDocument as a class name, and we need to register 'HTMLDocument' as a valid interface name for Document.
    registerMetaData(Document::getMetaData(),
                     reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, Document_Bridge<Any, invoke> >::createInstance),
                     "HTMLDocument");
}
