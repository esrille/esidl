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

void initializeMetaData()
{
    ProxyControl::registerMetaData(Object::getMetaData(), Proxy_Impl<ProxyObject, Object_Bridge<Any, invoke> >::createInstance);

    ProxyControl::registerMetaData(Attr::getMetaData(), Proxy_Impl<ProxyObject, Attr_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(CaretPosition::getMetaData(), Proxy_Impl<ProxyObject, CaretPosition_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(CDATASection::getMetaData(), Proxy_Impl<ProxyObject, CDATASection_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(CharacterData::getMetaData(), Proxy_Impl<ProxyObject, CharacterData_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Comment::getMetaData(), Proxy_Impl<ProxyObject, Comment_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DocumentFragment::getMetaData(), Proxy_Impl<ProxyObject, DocumentFragment_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Document::getMetaData(), Proxy_Impl<ProxyObject, Document_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DocumentType::getMetaData(), Proxy_Impl<ProxyObject, DocumentType_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMConfiguration::getMetaData(), Proxy_Impl<ProxyObject, DOMConfiguration_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMError::getMetaData(), Proxy_Impl<ProxyObject, DOMError_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMErrorHandler::getMetaData(), Proxy_Impl<ProxyObject, DOMErrorHandler_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMImplementation::getMetaData(), Proxy_Impl<ProxyObject, DOMImplementation_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMImplementationList::getMetaData(), Proxy_Impl<ProxyObject, DOMImplementationList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMImplementationSource::getMetaData(), Proxy_Impl<ProxyObject, DOMImplementationSource_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMLocator::getMetaData(), Proxy_Impl<ProxyObject, DOMLocator_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(DOMStringList::getMetaData(), Proxy_Impl<ProxyObject, DOMStringList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Element::getMetaData(), Proxy_Impl<ProxyObject, Element_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Entity::getMetaData(), Proxy_Impl<ProxyObject, Entity_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(EntityReference::getMetaData(), Proxy_Impl<ProxyObject, EntityReference_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(FormData::getMetaData(), Proxy_Impl<ProxyObject, FormData_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(NamedNodeMap::getMetaData(), Proxy_Impl<ProxyObject, NamedNodeMap_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(NameList::getMetaData(), Proxy_Impl<ProxyObject, NameList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Node::getMetaData(), Proxy_Impl<ProxyObject, Node_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(NodeList::getMetaData(), Proxy_Impl<ProxyObject, NodeList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Notation::getMetaData(), Proxy_Impl<ProxyObject, Notation_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ProcessingInstruction::getMetaData(), Proxy_Impl<ProxyObject, ProcessingInstruction_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(Text::getMetaData(), Proxy_Impl<ProxyObject, Text_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(TypeInfo::getMetaData(), Proxy_Impl<ProxyObject, TypeInfo_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(UserDataHandler::getMetaData(), Proxy_Impl<ProxyObject, UserDataHandler_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(XMLHttpRequestEventTarget::getMetaData(), Proxy_Impl<ProxyObject, XMLHttpRequestEventTarget_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(XMLHttpRequest::getMetaData(), Proxy_Impl<ProxyObject, XMLHttpRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(XMLHttpRequestUpload::getMetaData(), Proxy_Impl<ProxyObject, XMLHttpRequestUpload_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::Counter::getMetaData(), Proxy_Impl<ProxyObject, css::Counter_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSS2Properties::getMetaData(), Proxy_Impl<ProxyObject, css::CSS2Properties_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSCharsetRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSCharsetRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSColorComponentValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSColorComponentValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSComponentValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSComponentValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSFontFaceRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSFontFaceRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSImportRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSImportRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSMediaRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSMediaRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSNamespaceRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSNamespaceRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSPageRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSPageRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSPercentageComponentValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSPercentageComponentValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSPrimitiveValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSPrimitiveValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSStyleDeclaration::getMetaData(), Proxy_Impl<ProxyObject, css::CSSStyleDeclaration_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSStyleDeclarationValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSStyleDeclarationValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSStyleRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSStyleRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSStyleSheet::getMetaData(), Proxy_Impl<ProxyObject, css::CSSStyleSheet_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSUnknownRule::getMetaData(), Proxy_Impl<ProxyObject, css::CSSUnknownRule_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSURLComponentValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSURLComponentValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSValue::getMetaData(), Proxy_Impl<ProxyObject, css::CSSValue_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::CSSValueList::getMetaData(), Proxy_Impl<ProxyObject, css::CSSValueList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::DocumentCSS::getMetaData(), Proxy_Impl<ProxyObject, css::DocumentCSS_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::DOMImplementationCSS::getMetaData(), Proxy_Impl<ProxyObject, css::DOMImplementationCSS_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::ElementCSSInlineStyle::getMetaData(), Proxy_Impl<ProxyObject, css::ElementCSSInlineStyle_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::Rect::getMetaData(), Proxy_Impl<ProxyObject, css::Rect_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::RGBColor::getMetaData(), Proxy_Impl<ProxyObject, css::RGBColor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(css::ViewCSS::getMetaData(), Proxy_Impl<ProxyObject, css::ViewCSS_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::CompositionEvent::getMetaData(), Proxy_Impl<ProxyObject, events::CompositionEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::CustomEvent::getMetaData(), Proxy_Impl<ProxyObject, events::CustomEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::DocumentEvent::getMetaData(), Proxy_Impl<ProxyObject, events::DocumentEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::Event::getMetaData(), Proxy_Impl<ProxyObject, events::Event_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::EventListener::getMetaData(), Proxy_Impl<ProxyObject, events::EventListener_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::EventTarget::getMetaData(), Proxy_Impl<ProxyObject, events::EventTarget_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::FocusEvent::getMetaData(), Proxy_Impl<ProxyObject, events::FocusEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::KeyboardEvent::getMetaData(), Proxy_Impl<ProxyObject, events::KeyboardEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::MouseEvent::getMetaData(), Proxy_Impl<ProxyObject, events::MouseEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::MutationEvent::getMetaData(), Proxy_Impl<ProxyObject, events::MutationEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::MutationNameEvent::getMetaData(), Proxy_Impl<ProxyObject, events::MutationNameEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::ProgressEvent::getMetaData(), Proxy_Impl<ProxyObject, events::ProgressEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::TextEvent::getMetaData(), Proxy_Impl<ProxyObject, events::TextEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::UIEvent::getMetaData(), Proxy_Impl<ProxyObject, events::UIEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(events::WheelEvent::getMetaData(), Proxy_Impl<ProxyObject, events::WheelEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(eventsource::EventSource::getMetaData(), Proxy_Impl<ProxyObject, eventsource::EventSource_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::DOMImplementationLS::getMetaData(), Proxy_Impl<ProxyObject, ls::DOMImplementationLS_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSInput::getMetaData(), Proxy_Impl<ProxyObject, ls::LSInput_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSLoadEvent::getMetaData(), Proxy_Impl<ProxyObject, ls::LSLoadEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSOutput::getMetaData(), Proxy_Impl<ProxyObject, ls::LSOutput_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSParserFilter::getMetaData(), Proxy_Impl<ProxyObject, ls::LSParserFilter_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSParser::getMetaData(), Proxy_Impl<ProxyObject, ls::LSParser_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSProgressEvent::getMetaData(), Proxy_Impl<ProxyObject, ls::LSProgressEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSResourceResolver::getMetaData(), Proxy_Impl<ProxyObject, ls::LSResourceResolver_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSSerializerFilter::getMetaData(), Proxy_Impl<ProxyObject, ls::LSSerializerFilter_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ls::LSSerializer::getMetaData(), Proxy_Impl<ProxyObject, ls::LSSerializer_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ranges::DocumentRange::getMetaData(), Proxy_Impl<ProxyObject, ranges::DocumentRange_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(ranges::Range::getMetaData(), Proxy_Impl<ProxyObject, ranges::Range_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(stylesheets::LinkStyle::getMetaData(), Proxy_Impl<ProxyObject, stylesheets::LinkStyle_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(stylesheets::MediaList::getMetaData(), Proxy_Impl<ProxyObject, stylesheets::MediaList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(stylesheets::StyleSheet::getMetaData(), Proxy_Impl<ProxyObject, stylesheets::StyleSheet_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(traversal::DocumentTraversal::getMetaData(), Proxy_Impl<ProxyObject, traversal::DocumentTraversal_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(traversal::NodeFilter::getMetaData(), Proxy_Impl<ProxyObject, traversal::NodeFilter_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(traversal::NodeIterator::getMetaData(), Proxy_Impl<ProxyObject, traversal::NodeIterator_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(traversal::TreeWalker::getMetaData(), Proxy_Impl<ProxyObject, traversal::TreeWalker_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(validation::CharacterDataEditVAL::getMetaData(), Proxy_Impl<ProxyObject, validation::CharacterDataEditVAL_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(validation::DocumentEditVAL::getMetaData(), Proxy_Impl<ProxyObject, validation::DocumentEditVAL_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(validation::ElementEditVAL::getMetaData(), Proxy_Impl<ProxyObject, validation::ElementEditVAL_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(validation::NodeEditVAL::getMetaData(), Proxy_Impl<ProxyObject, validation::NodeEditVAL_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(views::AbstractView::getMetaData(), Proxy_Impl<ProxyObject, views::AbstractView_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(views::ClientRect::getMetaData(), Proxy_Impl<ProxyObject, views::ClientRect_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(views::ClientRectList::getMetaData(), Proxy_Impl<ProxyObject, views::ClientRectList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(views::DocumentView::getMetaData(), Proxy_Impl<ProxyObject, views::DocumentView_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(webnotifications::NotificationCenter::getMetaData(), Proxy_Impl<ProxyObject, webnotifications::NotificationCenter_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(webnotifications::Notification::getMetaData(), Proxy_Impl<ProxyObject, webnotifications::Notification_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(websocket::WebSocket::getMetaData(), Proxy_Impl<ProxyObject, websocket::WebSocket_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(webstorage::StorageEvent::getMetaData(), Proxy_Impl<ProxyObject, webstorage::StorageEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(webstorage::Storage::getMetaData(), Proxy_Impl<ProxyObject, webstorage::Storage_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(xpath::XPathEvaluator::getMetaData(), Proxy_Impl<ProxyObject, xpath::XPathEvaluator_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(xpath::XPathExpression::getMetaData(), Proxy_Impl<ProxyObject, xpath::XPathExpression_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(xpath::XPathNamespace::getMetaData(), Proxy_Impl<ProxyObject, xpath::XPathNamespace_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(xpath::XPathNSResolver::getMetaData(), Proxy_Impl<ProxyObject, xpath::XPathNSResolver_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(xpath::XPathResult::getMetaData(), Proxy_Impl<ProxyObject, xpath::XPathResult_Bridge<Any, invoke> >::createInstance);

    ProxyControl::registerMetaData(eventsource::EventSource_Constructor::getMetaData(), Proxy_Impl<ProxyObject, eventsource::EventSource_Constructor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(websocket::WebSocket_Constructor::getMetaData(), Proxy_Impl<ProxyObject, websocket::WebSocket_Constructor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(XMLHttpRequest_Constructor::getMetaData(), Proxy_Impl<ProxyObject, XMLHttpRequest_Constructor_Bridge<Any, invoke> >::createInstance);

    //
    // Workaround for WebKit
    //
    ProxyControl::registerMetaData(FormData::getMetaData(), Proxy_Impl<ProxyObject, FormData_Bridge<Any, invoke> >::createInstance,
                                   "DOMFormData");
    ProxyControl::registerMetaData(events::ProgressEvent::getMetaData(), Proxy_Impl<ProxyObject, events::ProgressEvent_Bridge<Any, invoke> >::createInstance,
                                   "XMLHttpRequestProgressEvent");

}
