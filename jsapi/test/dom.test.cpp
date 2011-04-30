/*
 * Copyright 2011 Esrille Inc.
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

#include <esjsapi.h>

#include <assert.h>

#include <iostream>

#include <org/w3c/dom/bootstrap/AnonXMLHttpRequestImp.h>
#include <org/w3c/dom/bootstrap/ApplicationCacheImp.h>
#include <org/w3c/dom/bootstrap/ArrayBufferImp.h>
#include <org/w3c/dom/bootstrap/ArrayBufferViewImp.h>
#include <org/w3c/dom/bootstrap/AttrImp.h>
#include <org/w3c/dom/bootstrap/BarPropImp.h>
#include <org/w3c/dom/bootstrap/BeforeUnloadEventImp.h>
#include <org/w3c/dom/bootstrap/BlobImp.h>
#include <org/w3c/dom/bootstrap/BooleanCallbackImp.h>
#include <org/w3c/dom/bootstrap/CSS2PropertiesImp.h>
#include <org/w3c/dom/bootstrap/CSSCharsetRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSColorComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSFontFaceRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSIdentifierComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSImportRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSKeywordComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSLengthComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSMapValueImp.h>
#include <org/w3c/dom/bootstrap/CSSMediaRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSNamespaceRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSPageRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSPercentageComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSPrimitiveValueImp.h>
#include <org/w3c/dom/bootstrap/CSSPropertyValueImp.h>
#include <org/w3c/dom/bootstrap/CSSPropertyValueListImp.h>
#include <org/w3c/dom/bootstrap/CSSRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSStringComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSStyleDeclarationImp.h>
#include <org/w3c/dom/bootstrap/CSSStyleDeclarationValueImp.h>
#include <org/w3c/dom/bootstrap/CSSStyleRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSStyleSheetImp.h>
#include <org/w3c/dom/bootstrap/CSSURLComponentValueImp.h>
#include <org/w3c/dom/bootstrap/CSSUnknownRuleImp.h>
#include <org/w3c/dom/bootstrap/CSSValueImp.h>
#include <org/w3c/dom/bootstrap/CSSValueListImp.h>
#include <org/w3c/dom/bootstrap/CanvasGradientImp.h>
#include <org/w3c/dom/bootstrap/CanvasPatternImp.h>
#include <org/w3c/dom/bootstrap/CanvasPixelArrayImp.h>
#include <org/w3c/dom/bootstrap/CanvasRenderingContext2DImp.h>
#include <org/w3c/dom/bootstrap/CaretPositionImp.h>
#include <org/w3c/dom/bootstrap/CharacterDataEditVALImp.h>
#include <org/w3c/dom/bootstrap/CharacterDataImp.h>
#include <org/w3c/dom/bootstrap/ClientRectImp.h>
#include <org/w3c/dom/bootstrap/ClientRectListImp.h>
#include <org/w3c/dom/bootstrap/CommentImp.h>
#include <org/w3c/dom/bootstrap/CompositionEventImp.h>
#include <org/w3c/dom/bootstrap/CounterImp.h>
#include <org/w3c/dom/bootstrap/CustomEventImp.h>
#include <org/w3c/dom/bootstrap/DOMConfigurationImp.h>
#include <org/w3c/dom/bootstrap/DOMImplementationCSSImp.h>
#include <org/w3c/dom/bootstrap/DOMImplementationImp.h>
#include <org/w3c/dom/bootstrap/DOMImplementationLSImp.h>
#include <org/w3c/dom/bootstrap/DOMSettableTokenListImp.h>
#include <org/w3c/dom/bootstrap/DOMStringListImp.h>
#include <org/w3c/dom/bootstrap/DOMStringMapImp.h>
#include <org/w3c/dom/bootstrap/DOMTokenListImp.h>
#include <org/w3c/dom/bootstrap/DataTransferImp.h>
#include <org/w3c/dom/bootstrap/DataViewImp.h>
#include <org/w3c/dom/bootstrap/DocumentCSSImp.h>
#include <org/w3c/dom/bootstrap/DocumentEditVALImp.h>
#include <org/w3c/dom/bootstrap/DocumentFragmentImp.h>
#include <org/w3c/dom/bootstrap/DocumentImp.h>
#include <org/w3c/dom/bootstrap/DocumentRangeImp.h>
#include <org/w3c/dom/bootstrap/DocumentTraversalImp.h>
#include <org/w3c/dom/bootstrap/DocumentTypeImp.h>
#include <org/w3c/dom/bootstrap/DragEventImp.h>
#include <org/w3c/dom/bootstrap/ElementCSSInlineStyleImp.h>
#include <org/w3c/dom/bootstrap/ElementEditVALImp.h>
#include <org/w3c/dom/bootstrap/ElementImp.h>
#include <org/w3c/dom/bootstrap/EventImp.h>
#include <org/w3c/dom/bootstrap/EventListenerImp.h>
#include <org/w3c/dom/bootstrap/EventSourceImp.h>
#include <org/w3c/dom/bootstrap/EventTargetImp.h>
#include <org/w3c/dom/bootstrap/FileErrorImp.h>
#include <org/w3c/dom/bootstrap/FileImp.h>
#include <org/w3c/dom/bootstrap/FileListImp.h>
#include <org/w3c/dom/bootstrap/FileReaderImp.h>
#include <org/w3c/dom/bootstrap/FileReaderSyncImp.h>
#include <org/w3c/dom/bootstrap/Float32ArrayImp.h>
#include <org/w3c/dom/bootstrap/Float64ArrayImp.h>
#include <org/w3c/dom/bootstrap/FocusEventImp.h>
#include <org/w3c/dom/bootstrap/FormDataImp.h>
#include <org/w3c/dom/bootstrap/FunctionImp.h>
#include <org/w3c/dom/bootstrap/HTMLAllCollectionImp.h>
#include <org/w3c/dom/bootstrap/HTMLAnchorElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLAppletElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLAreaElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLAudioElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLBRElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLBaseElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLBaseFontElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLBodyElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLButtonElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLCanvasElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLCollectionImp.h>
#include <org/w3c/dom/bootstrap/HTMLCommandElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDListElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDataListElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDetailsElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDirectoryElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDivElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLDocumentImp.h>
#include <org/w3c/dom/bootstrap/HTMLElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLEmbedElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLFieldSetElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLFontElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLFormControlsCollectionImp.h>
#include <org/w3c/dom/bootstrap/HTMLFormElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLFrameElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLFrameSetElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLHRElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLHeadElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLHeadingElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLHtmlElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLIFrameElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLImageElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLInputElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLKeygenElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLLIElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLLabelElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLLegendElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLLinkElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMapElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMarqueeElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMediaElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMenuElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMetaElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLMeterElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLModElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLOListElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLObjectElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLOptGroupElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLOptionElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLOptionsCollectionImp.h>
#include <org/w3c/dom/bootstrap/HTMLOutputElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLParagraphElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLParamElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLPreElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLProgressElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLPropertiesCollectionImp.h>
#include <org/w3c/dom/bootstrap/HTMLQuoteElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLScriptElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLSelectElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLSourceElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLSpanElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLStyleElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableCaptionElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableCellElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableColElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableDataCellElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableHeaderCellElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableRowElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTableSectionElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTextAreaElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTimeElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLTitleElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLUListElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLUnknownElementImp.h>
#include <org/w3c/dom/bootstrap/HTMLVideoElementImp.h>
#include <org/w3c/dom/bootstrap/HashChangeEventImp.h>
#include <org/w3c/dom/bootstrap/HistoryImp.h>
#include <org/w3c/dom/bootstrap/ImageDataImp.h>
#include <org/w3c/dom/bootstrap/Int16ArrayImp.h>
#include <org/w3c/dom/bootstrap/Int32ArrayImp.h>
#include <org/w3c/dom/bootstrap/Int8ArrayImp.h>
#include <org/w3c/dom/bootstrap/KeyboardEventImp.h>
#include <org/w3c/dom/bootstrap/LSInputImp.h>
#include <org/w3c/dom/bootstrap/LSLoadEventImp.h>
#include <org/w3c/dom/bootstrap/LSOutputImp.h>
#include <org/w3c/dom/bootstrap/LSParserFilterImp.h>
#include <org/w3c/dom/bootstrap/LSParserImp.h>
#include <org/w3c/dom/bootstrap/LSProgressEventImp.h>
#include <org/w3c/dom/bootstrap/LSResourceResolverImp.h>
#include <org/w3c/dom/bootstrap/LSSerializerFilterImp.h>
#include <org/w3c/dom/bootstrap/LSSerializerImp.h>
#include <org/w3c/dom/bootstrap/LinkStyleImp.h>
#include <org/w3c/dom/bootstrap/LocationImp.h>
#include <org/w3c/dom/bootstrap/MediaErrorImp.h>
#include <org/w3c/dom/bootstrap/MediaListImp.h>
#include <org/w3c/dom/bootstrap/MessageChannelImp.h>
#include <org/w3c/dom/bootstrap/MessageEventImp.h>
#include <org/w3c/dom/bootstrap/MessagePortImp.h>
#include <org/w3c/dom/bootstrap/MouseEventImp.h>
#include <org/w3c/dom/bootstrap/MutationEventImp.h>
#include <org/w3c/dom/bootstrap/MutationNameEventImp.h>
#include <org/w3c/dom/bootstrap/NameListImp.h>
#include <org/w3c/dom/bootstrap/NavigatorImp.h>
#include <org/w3c/dom/bootstrap/NodeEditVALImp.h>
#include <org/w3c/dom/bootstrap/NodeFilterImp.h>
#include <org/w3c/dom/bootstrap/NodeImp.h>
#include <org/w3c/dom/bootstrap/NodeIteratorImp.h>
#include <org/w3c/dom/bootstrap/NodeListImp.h>
#include <org/w3c/dom/bootstrap/PageTransitionEventImp.h>
#include <org/w3c/dom/bootstrap/PopStateEventImp.h>
#include <org/w3c/dom/bootstrap/ProcessingInstructionImp.h>
#include <org/w3c/dom/bootstrap/ProgressEventImp.h>
#include <org/w3c/dom/bootstrap/PropertyNodeListImp.h>
#include <org/w3c/dom/bootstrap/RGBColorImp.h>
#include <org/w3c/dom/bootstrap/RadioNodeListImp.h>
#include <org/w3c/dom/bootstrap/RangeImp.h>
#include <org/w3c/dom/bootstrap/RectImp.h>
#include <org/w3c/dom/bootstrap/ScreenImp.h>
#include <org/w3c/dom/bootstrap/SelectionImp.h>
#include <org/w3c/dom/bootstrap/StyleMediaImp.h>
#include <org/w3c/dom/bootstrap/StyleSheetImp.h>
#include <org/w3c/dom/bootstrap/TextEventImp.h>
#include <org/w3c/dom/bootstrap/TextImp.h>
#include <org/w3c/dom/bootstrap/TextMetricsImp.h>
#include <org/w3c/dom/bootstrap/TimeRangesImp.h>
#include <org/w3c/dom/bootstrap/TreeWalkerImp.h>
#include <org/w3c/dom/bootstrap/UIEventImp.h>
#include <org/w3c/dom/bootstrap/URLImp.h>
#include <org/w3c/dom/bootstrap/Uint16ArrayImp.h>
#include <org/w3c/dom/bootstrap/Uint32ArrayImp.h>
#include <org/w3c/dom/bootstrap/Uint8ArrayImp.h>
#include <org/w3c/dom/bootstrap/UndoManagerEventImp.h>
#include <org/w3c/dom/bootstrap/UndoManagerImp.h>
#include <org/w3c/dom/bootstrap/ValidityStateImp.h>
#include <org/w3c/dom/bootstrap/WebGLActiveInfoImp.h>
#include <org/w3c/dom/bootstrap/WebGLBufferImp.h>
#include <org/w3c/dom/bootstrap/WebGLContextAttributesImp.h>
#include <org/w3c/dom/bootstrap/WebGLContextEventImp.h>
#include <org/w3c/dom/bootstrap/WebGLFramebufferImp.h>
#include <org/w3c/dom/bootstrap/WebGLObjectImp.h>
#include <org/w3c/dom/bootstrap/WebGLProgramImp.h>
#include <org/w3c/dom/bootstrap/WebGLRenderbufferImp.h>
#include <org/w3c/dom/bootstrap/WebGLRenderingContextImp.h>
#include <org/w3c/dom/bootstrap/WebGLShaderImp.h>
#include <org/w3c/dom/bootstrap/WebGLTextureImp.h>
#include <org/w3c/dom/bootstrap/WebGLUniformLocationImp.h>
#include <org/w3c/dom/bootstrap/WheelEventImp.h>
#include <org/w3c/dom/bootstrap/WindowImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestEventTargetImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestUploadImp.h>
#include <org/w3c/dom/bootstrap/XPathEvaluatorImp.h>
#include <org/w3c/dom/bootstrap/XPathExpressionImp.h>
#include <org/w3c/dom/bootstrap/XPathNSResolverImp.h>
#include <org/w3c/dom/bootstrap/XPathNamespaceImp.h>
#include <org/w3c/dom/bootstrap/XPathResultImp.h>

using namespace org::w3c::dom::bootstrap;
using namespace org::w3c::dom;

html::Window window(0);

static JSClass globalClass = {
    "global", JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

void reportError(JSContext* cx, const char* message, JSErrorReport* report)
{
    std::cerr << (report->filename ? report->filename : "<no filename>") << report->lineno << message;
}

void registerClasses(JSContext* cx)
{
    // prototype roots
    AttrImp::setStaticPrivate(new NativeClass(cx, Attr::getMetaData()));
    CSSRuleImp::setStaticPrivate(new NativeClass(cx, css::CSSRule::getMetaData()));
    CSSStyleDeclarationImp::setStaticPrivate(new NativeClass(cx, css::CSSStyleDeclaration::getMetaData()));
    DOMImplementationImp::setStaticPrivate(new NativeClass(cx, DOMImplementation::getMetaData()));
    EventImp::setStaticPrivate(new NativeClass(cx, events::Event::getMetaData()));
    EventTargetImp::setStaticPrivate(new NativeClass(cx, events::EventTarget::getMetaData()));
    LocationImp::setStaticPrivate(new NativeClass(cx, html::Location::getMetaData()));
    MediaListImp::setStaticPrivate(new NativeClass(cx, stylesheets::MediaList::getMetaData()));
    NodeListImp::setStaticPrivate(new NativeClass(cx, NodeList::getMetaData()));
    StyleSheetImp::setStaticPrivate(new NativeClass(cx, stylesheets::StyleSheet::getMetaData()));

    // dom
    ApplicationCacheImp::setStaticPrivate(new NativeClass(cx, ApplicationCacheImp::getMetaData()));
    BarPropImp::setStaticPrivate(new NativeClass(cx, BarPropImp::getMetaData()));
    BlobImp::setStaticPrivate(new NativeClass(cx, BlobImp::getMetaData()));
    BooleanCallbackImp::setStaticPrivate(new NativeClass(cx, BooleanCallbackImp::getMetaData()));
    CanvasGradientImp::setStaticPrivate(new NativeClass(cx, CanvasGradientImp::getMetaData()));
    CanvasPatternImp::setStaticPrivate(new NativeClass(cx, CanvasPatternImp::getMetaData()));
    CanvasPixelArrayImp::setStaticPrivate(new NativeClass(cx, CanvasPixelArrayImp::getMetaData()));
    CanvasRenderingContext2DImp::setStaticPrivate(new NativeClass(cx, CanvasRenderingContext2DImp::getMetaData()));
    CaretPositionImp::setStaticPrivate(new NativeClass(cx, CaretPositionImp::getMetaData()));
    ClientRectImp::setStaticPrivate(new NativeClass(cx, ClientRectImp::getMetaData()));
    ClientRectListImp::setStaticPrivate(new NativeClass(cx, ClientRectListImp::getMetaData()));
    CounterImp::setStaticPrivate(new NativeClass(cx, CounterImp::getMetaData()));
    DOMConfigurationImp::setStaticPrivate(new NativeClass(cx, DOMConfigurationImp::getMetaData()));
    DOMStringListImp::setStaticPrivate(new NativeClass(cx, DOMStringListImp::getMetaData()));
    DOMStringMapImp::setStaticPrivate(new NativeClass(cx, DOMStringMapImp::getMetaData()));
    DataTransferImp::setStaticPrivate(new NativeClass(cx, DataTransferImp::getMetaData()));
    DocumentRangeImp::setStaticPrivate(new NativeClass(cx, DocumentRangeImp::getMetaData()));
    DocumentTraversalImp::setStaticPrivate(new NativeClass(cx, DocumentTraversalImp::getMetaData()));
    ElementCSSInlineStyleImp::setStaticPrivate(new NativeClass(cx, ElementCSSInlineStyleImp::getMetaData()));
    EventListenerImp::setStaticPrivate(new NativeClass(cx, EventListenerImp::getMetaData()));
    EventSourceImp::setStaticPrivate(new NativeClass(cx, EventSourceImp::getMetaData()));
    FormDataImp::setStaticPrivate(new NativeClass(cx, FormDataImp::getMetaData()));
    HistoryImp::setStaticPrivate(new NativeClass(cx, HistoryImp::getMetaData()));
    ImageDataImp::setStaticPrivate(new NativeClass(cx, ImageDataImp::getMetaData()));
    LinkStyleImp::setStaticPrivate(new NativeClass(cx, LinkStyleImp::getMetaData()));
    MediaErrorImp::setStaticPrivate(new NativeClass(cx, MediaErrorImp::getMetaData()));
    MessageChannelImp::setStaticPrivate(new NativeClass(cx, MessageChannelImp::getMetaData()));
    MessagePortImp::setStaticPrivate(new NativeClass(cx, MessagePortImp::getMetaData()));
    NameListImp::setStaticPrivate(new NativeClass(cx, NameListImp::getMetaData()));
    NavigatorImp::setStaticPrivate(new NativeClass(cx, NavigatorImp::getMetaData()));
    NodeFilterImp::setStaticPrivate(new NativeClass(cx, NodeFilterImp::getMetaData()));
    NodeIteratorImp::setStaticPrivate(new NativeClass(cx, NodeIteratorImp::getMetaData()));
    PropertyNodeListImp::setStaticPrivate(new NativeClass(cx, PropertyNodeListImp::getMetaData()));
    RGBColorImp::setStaticPrivate(new NativeClass(cx, RGBColorImp::getMetaData()));
    RadioNodeListImp::setStaticPrivate(new NativeClass(cx, RadioNodeListImp::getMetaData()));
    RangeImp::setStaticPrivate(new NativeClass(cx, RangeImp::getMetaData()));
    RectImp::setStaticPrivate(new NativeClass(cx, RectImp::getMetaData()));
    ScreenImp::setStaticPrivate(new NativeClass(cx, ScreenImp::getMetaData()));
    SelectionImp::setStaticPrivate(new NativeClass(cx, SelectionImp::getMetaData()));
    StyleMediaImp::setStaticPrivate(new NativeClass(cx, StyleMediaImp::getMetaData()));
    TextMetricsImp::setStaticPrivate(new NativeClass(cx, TextMetricsImp::getMetaData()));
    TimeRangesImp::setStaticPrivate(new NativeClass(cx, TimeRangesImp::getMetaData()));
    TreeWalkerImp::setStaticPrivate(new NativeClass(cx, TreeWalkerImp::getMetaData()));
    URLImp::setStaticPrivate(new NativeClass(cx, URLImp::getMetaData()));
    UndoManagerImp::setStaticPrivate(new NativeClass(cx, UndoManagerImp::getMetaData()));
    ValidityStateImp::setStaticPrivate(new NativeClass(cx, ValidityStateImp::getMetaData()));

    DOMTokenListImp::setStaticPrivate(new NativeClass(cx, DOMTokenListImp::getMetaData()));
    DOMSettableTokenListImp::setStaticPrivate(new NativeClass(cx, DOMSettableTokenListImp::getMetaData()));

    // window
    WindowImp::setStaticPrivate(new NativeClass(cx, html::Window::getMetaData()));

    // node
    NodeImp::setStaticPrivate(new NativeClass(cx, Node::getMetaData()));
    CharacterDataImp::setStaticPrivate(new NativeClass(cx, CharacterData::getMetaData()));
    DocumentImp::setStaticPrivate(new NativeClass(cx, Document::getMetaData()));
    DocumentFragmentImp::setStaticPrivate(new NativeClass(cx, DocumentFragmentImp::getMetaData()));
    DocumentTypeImp::setStaticPrivate(new NativeClass(cx, DocumentType::getMetaData()));
    ElementImp::setStaticPrivate(new NativeClass(cx, Element::getMetaData()));
    ProcessingInstructionImp::setStaticPrivate(new NativeClass(cx, ProcessingInstructionImp::getMetaData()));

    // event
    UIEventImp::setStaticPrivate(new NativeClass(cx, events::UIEvent::getMetaData()));
    MouseEventImp::setStaticPrivate(new NativeClass(cx, MouseEventImp::getMetaData()));
    DragEventImp::setStaticPrivate(new NativeClass(cx, DragEventImp::getMetaData()));
    MutationEventImp::setStaticPrivate(new NativeClass(cx, MutationEventImp::getMetaData()));
    PageTransitionEventImp::setStaticPrivate(new NativeClass(cx, PageTransitionEventImp::getMetaData()));
    PopStateEventImp::setStaticPrivate(new NativeClass(cx, PopStateEventImp::getMetaData()));
    ProgressEventImp::setStaticPrivate(new NativeClass(cx, ProgressEventImp::getMetaData()));
    TextEventImp::setStaticPrivate(new NativeClass(cx, TextEventImp::getMetaData()));
    UndoManagerEventImp::setStaticPrivate(new NativeClass(cx, UndoManagerEventImp::getMetaData()));
    WheelEventImp::setStaticPrivate(new NativeClass(cx, WheelEventImp::getMetaData()));
    BeforeUnloadEventImp::setStaticPrivate(new NativeClass(cx, BeforeUnloadEventImp::getMetaData()));
    CompositionEventImp::setStaticPrivate(new NativeClass(cx, CompositionEventImp::getMetaData()));
    CustomEventImp::setStaticPrivate(new NativeClass(cx, CustomEventImp::getMetaData()));
    FocusEventImp::setStaticPrivate(new NativeClass(cx, FocusEventImp::getMetaData()));
    HashChangeEventImp::setStaticPrivate(new NativeClass(cx, HashChangeEventImp::getMetaData()));
    KeyboardEventImp::setStaticPrivate(new NativeClass(cx, KeyboardEventImp::getMetaData()));
    MessageEventImp::setStaticPrivate(new NativeClass(cx, MessageEventImp::getMetaData()));
    MutationNameEventImp::setStaticPrivate(new NativeClass(cx, MutationNameEventImp::getMetaData()));

    // character data
    CommentImp::setStaticPrivate(new NativeClass(cx, Comment::getMetaData()));
    TextImp::setStaticPrivate(new NativeClass(cx, Text::getMetaData()));

    // html element
    HTMLElementImp::setStaticPrivate(new NativeClass(cx, HTMLElementImp::getMetaData()));
    HTMLAnchorElementImp::setStaticPrivate(new NativeClass(cx, HTMLAnchorElementImp::getMetaData()));
    HTMLAppletElementImp::setStaticPrivate(new NativeClass(cx, HTMLAppletElementImp::getMetaData()));
    HTMLAreaElementImp::setStaticPrivate(new NativeClass(cx, HTMLAreaElementImp::getMetaData()));
    HTMLBRElementImp::setStaticPrivate(new NativeClass(cx, HTMLBRElementImp::getMetaData()));
    HTMLBaseElementImp::setStaticPrivate(new NativeClass(cx, HTMLBaseElementImp::getMetaData()));
    HTMLBaseFontElementImp::setStaticPrivate(new NativeClass(cx, HTMLBaseFontElementImp::getMetaData()));
    HTMLBodyElementImp::setStaticPrivate(new NativeClass(cx, HTMLBodyElementImp::getMetaData()));
    HTMLButtonElementImp::setStaticPrivate(new NativeClass(cx, HTMLButtonElementImp::getMetaData()));
    HTMLCanvasElementImp::setStaticPrivate(new NativeClass(cx, HTMLCanvasElementImp::getMetaData()));
    HTMLCommandElementImp::setStaticPrivate(new NativeClass(cx, HTMLCommandElementImp::getMetaData()));
    HTMLDListElementImp::setStaticPrivate(new NativeClass(cx, HTMLDListElementImp::getMetaData()));
    HTMLDataListElementImp::setStaticPrivate(new NativeClass(cx, HTMLDataListElementImp::getMetaData()));
    HTMLDetailsElementImp::setStaticPrivate(new NativeClass(cx, HTMLDetailsElementImp::getMetaData()));
    HTMLDirectoryElementImp::setStaticPrivate(new NativeClass(cx, HTMLDirectoryElementImp::getMetaData()));
    HTMLDivElementImp::setStaticPrivate(new NativeClass(cx, HTMLDivElementImp::getMetaData()));
    HTMLDocumentImp::setStaticPrivate(new NativeClass(cx, HTMLDocumentImp::getMetaData()));
    HTMLEmbedElementImp::setStaticPrivate(new NativeClass(cx, HTMLEmbedElementImp::getMetaData()));
    HTMLFieldSetElementImp::setStaticPrivate(new NativeClass(cx, HTMLFieldSetElementImp::getMetaData()));
    HTMLFontElementImp::setStaticPrivate(new NativeClass(cx, HTMLFontElementImp::getMetaData()));
    HTMLFormElementImp::setStaticPrivate(new NativeClass(cx, HTMLFormElementImp::getMetaData()));
    HTMLFrameElementImp::setStaticPrivate(new NativeClass(cx, HTMLFrameElementImp::getMetaData()));
    HTMLFrameSetElementImp::setStaticPrivate(new NativeClass(cx, HTMLFrameSetElementImp::getMetaData()));
    HTMLHRElementImp::setStaticPrivate(new NativeClass(cx, HTMLHRElementImp::getMetaData()));
    HTMLHeadElementImp::setStaticPrivate(new NativeClass(cx, HTMLHeadElementImp::getMetaData()));
    HTMLHeadingElementImp::setStaticPrivate(new NativeClass(cx, HTMLHeadingElementImp::getMetaData()));
    HTMLHtmlElementImp::setStaticPrivate(new NativeClass(cx, HTMLHtmlElementImp::getMetaData()));
    HTMLIFrameElementImp::setStaticPrivate(new NativeClass(cx, HTMLIFrameElementImp::getMetaData()));
    HTMLImageElementImp::setStaticPrivate(new NativeClass(cx, HTMLImageElementImp::getMetaData()));
    HTMLInputElementImp::setStaticPrivate(new NativeClass(cx, HTMLInputElementImp::getMetaData()));
    HTMLKeygenElementImp::setStaticPrivate(new NativeClass(cx, HTMLKeygenElementImp::getMetaData()));
    HTMLLIElementImp::setStaticPrivate(new NativeClass(cx, HTMLLIElementImp::getMetaData()));
    HTMLLabelElementImp::setStaticPrivate(new NativeClass(cx, HTMLLabelElementImp::getMetaData()));
    HTMLLegendElementImp::setStaticPrivate(new NativeClass(cx, HTMLLegendElementImp::getMetaData()));
    HTMLLinkElementImp::setStaticPrivate(new NativeClass(cx, HTMLLinkElementImp::getMetaData()));
    HTMLMapElementImp::setStaticPrivate(new NativeClass(cx, HTMLMapElementImp::getMetaData()));
    HTMLMarqueeElementImp::setStaticPrivate(new NativeClass(cx, HTMLMarqueeElementImp::getMetaData()));
    HTMLMenuElementImp::setStaticPrivate(new NativeClass(cx, HTMLMenuElementImp::getMetaData()));
    HTMLMetaElementImp::setStaticPrivate(new NativeClass(cx, HTMLMetaElementImp::getMetaData()));
    HTMLMeterElementImp::setStaticPrivate(new NativeClass(cx, HTMLMeterElementImp::getMetaData()));
    HTMLModElementImp::setStaticPrivate(new NativeClass(cx, HTMLModElementImp::getMetaData()));
    HTMLOListElementImp::setStaticPrivate(new NativeClass(cx, HTMLOListElementImp::getMetaData()));
    HTMLObjectElementImp::setStaticPrivate(new NativeClass(cx, HTMLObjectElementImp::getMetaData()));
    HTMLOptGroupElementImp::setStaticPrivate(new NativeClass(cx, HTMLOptGroupElementImp::getMetaData()));
    HTMLOptionElementImp::setStaticPrivate(new NativeClass(cx, HTMLOptionElementImp::getMetaData()));
    HTMLOutputElementImp::setStaticPrivate(new NativeClass(cx, HTMLOutputElementImp::getMetaData()));
    HTMLParagraphElementImp::setStaticPrivate(new NativeClass(cx, HTMLParagraphElementImp::getMetaData()));
    HTMLParamElementImp::setStaticPrivate(new NativeClass(cx, HTMLParamElementImp::getMetaData()));
    HTMLPreElementImp::setStaticPrivate(new NativeClass(cx, HTMLPreElementImp::getMetaData()));
    HTMLProgressElementImp::setStaticPrivate(new NativeClass(cx, HTMLProgressElementImp::getMetaData()));
    HTMLQuoteElementImp::setStaticPrivate(new NativeClass(cx, HTMLQuoteElementImp::getMetaData()));
    HTMLScriptElementImp::setStaticPrivate(new NativeClass(cx, HTMLScriptElementImp::getMetaData()));
    HTMLSelectElementImp::setStaticPrivate(new NativeClass(cx, HTMLSelectElementImp::getMetaData()));
    HTMLSourceElementImp::setStaticPrivate(new NativeClass(cx, HTMLSourceElementImp::getMetaData()));
    HTMLSpanElementImp::setStaticPrivate(new NativeClass(cx, HTMLSpanElementImp::getMetaData()));
    HTMLStyleElementImp::setStaticPrivate(new NativeClass(cx, HTMLStyleElementImp::getMetaData()));
    HTMLTableCaptionElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableCaptionElementImp::getMetaData()));
    HTMLTableCellElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableCellElementImp::getMetaData()));
    HTMLTableColElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableColElementImp::getMetaData()));
    HTMLTableDataCellElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableDataCellElementImp::getMetaData()));
    HTMLTableElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableElementImp::getMetaData()));
    HTMLTableHeaderCellElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableHeaderCellElementImp::getMetaData()));
    HTMLTableRowElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableRowElementImp::getMetaData()));
    HTMLTableSectionElementImp::setStaticPrivate(new NativeClass(cx, HTMLTableSectionElementImp::getMetaData()));
    HTMLTextAreaElementImp::setStaticPrivate(new NativeClass(cx, HTMLTextAreaElementImp::getMetaData()));
    HTMLTimeElementImp::setStaticPrivate(new NativeClass(cx, HTMLTimeElementImp::getMetaData()));
    HTMLTitleElementImp::setStaticPrivate(new NativeClass(cx, HTMLTitleElementImp::getMetaData()));
    HTMLUListElementImp::setStaticPrivate(new NativeClass(cx, HTMLUListElementImp::getMetaData()));
    HTMLUnknownElementImp::setStaticPrivate(new NativeClass(cx, HTMLUnknownElementImp::getMetaData()));

    // media element
    HTMLMediaElementImp::setStaticPrivate(new NativeClass(cx, HTMLMediaElementImp::getMetaData()));
    HTMLAudioElementImp::setStaticPrivate(new NativeClass(cx, HTMLAudioElementImp::getMetaData()));
    HTMLVideoElementImp::setStaticPrivate(new NativeClass(cx, HTMLVideoElementImp::getMetaData()));

    // html collection
    HTMLCollectionImp::setStaticPrivate(new NativeClass(cx, HTMLCollectionImp::getMetaData()));
    HTMLAllCollectionImp::setStaticPrivate(new NativeClass(cx, HTMLAllCollectionImp::getMetaData()));
    HTMLOptionsCollectionImp::setStaticPrivate(new NativeClass(cx, HTMLOptionsCollectionImp::getMetaData()));
    HTMLFormControlsCollectionImp::setStaticPrivate(new NativeClass(cx, HTMLFormControlsCollectionImp::getMetaData()));
    HTMLPropertiesCollectionImp::setStaticPrivate(new NativeClass(cx, HTMLPropertiesCollectionImp::getMetaData()));

    // style sheet
    CSSStyleSheetImp::setStaticPrivate(new NativeClass(cx, css::CSSStyleSheet::getMetaData()));

    // css rule
    CSSMediaRuleImp::setStaticPrivate(new NativeClass(cx, css::CSSMediaRule::getMetaData()));
    CSSStyleRuleImp::setStaticPrivate(new NativeClass(cx, css::CSSStyleRule::getMetaData()));

    // css value
    CSSValueImp::setStaticPrivate(new NativeClass(cx, CSSValueImp::getMetaData()));

    // css
    DOMImplementationCSSImp::setStaticPrivate(new NativeClass(cx, DOMImplementationCSSImp::getMetaData()));
    DocumentCSSImp::setStaticPrivate(new NativeClass(cx, DocumentCSSImp::getMetaData()));
    CSS2PropertiesImp::setStaticPrivate(new NativeClass(cx, CSS2PropertiesImp::getMetaData()));
    CSSCharsetRuleImp::setStaticPrivate(new NativeClass(cx, CSSCharsetRuleImp::getMetaData()));
    CSSColorComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSColorComponentValueImp::getMetaData()));
    CSSComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSComponentValueImp::getMetaData()));
    CSSFontFaceRuleImp::setStaticPrivate(new NativeClass(cx, CSSFontFaceRuleImp::getMetaData()));
    CSSIdentifierComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSIdentifierComponentValueImp::getMetaData()));
    CSSImportRuleImp::setStaticPrivate(new NativeClass(cx, CSSImportRuleImp::getMetaData()));
    CSSKeywordComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSKeywordComponentValueImp::getMetaData()));
    CSSLengthComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSLengthComponentValueImp::getMetaData()));
    CSSMapValueImp::setStaticPrivate(new NativeClass(cx, CSSMapValueImp::getMetaData()));
    CSSNamespaceRuleImp::setStaticPrivate(new NativeClass(cx, CSSNamespaceRuleImp::getMetaData()));
    CSSPageRuleImp::setStaticPrivate(new NativeClass(cx, CSSPageRuleImp::getMetaData()));
    CSSPercentageComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSPercentageComponentValueImp::getMetaData()));
    CSSPrimitiveValueImp::setStaticPrivate(new NativeClass(cx, CSSPrimitiveValueImp::getMetaData()));
    CSSPropertyValueImp::setStaticPrivate(new NativeClass(cx, CSSPropertyValueImp::getMetaData()));
    CSSPropertyValueListImp::setStaticPrivate(new NativeClass(cx, CSSPropertyValueListImp::getMetaData()));
    CSSStringComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSStringComponentValueImp::getMetaData()));
    CSSStyleDeclarationValueImp::setStaticPrivate(new NativeClass(cx, CSSStyleDeclarationValueImp::getMetaData()));
    CSSURLComponentValueImp::setStaticPrivate(new NativeClass(cx, CSSURLComponentValueImp::getMetaData()));
    CSSUnknownRuleImp::setStaticPrivate(new NativeClass(cx, CSSUnknownRuleImp::getMetaData()));
    CSSValueListImp::setStaticPrivate(new NativeClass(cx, CSSValueListImp::getMetaData()));

    // XMLHttpRequest
    XMLHttpRequestEventTargetImp::setStaticPrivate(new NativeClass(cx, XMLHttpRequestEventTargetImp::getMetaData()));
    XMLHttpRequestImp::setStaticPrivate(new NativeClass(cx, XMLHttpRequestImp::getMetaData()));
    AnonXMLHttpRequestImp::setStaticPrivate(new NativeClass(cx, AnonXMLHttpRequestImp::getMetaData()));
    XMLHttpRequestUploadImp::setStaticPrivate(new NativeClass(cx, XMLHttpRequestUploadImp::getMetaData()));

    // NodeEditVALImp
    NodeEditVALImp::setStaticPrivate(new NativeClass(cx, NodeEditVALImp::getMetaData()));
    CharacterDataEditVALImp::setStaticPrivate(new NativeClass(cx, CharacterDataEditVALImp::getMetaData()));
    DocumentEditVALImp::setStaticPrivate(new NativeClass(cx, DocumentEditVALImp::getMetaData()));
    ElementEditVALImp::setStaticPrivate(new NativeClass(cx, ElementEditVALImp::getMetaData()));

    // xpath
    XPathEvaluatorImp::setStaticPrivate(new NativeClass(cx, XPathEvaluatorImp::getMetaData()));
    XPathExpressionImp::setStaticPrivate(new NativeClass(cx, XPathExpressionImp::getMetaData()));
    XPathNSResolverImp::setStaticPrivate(new NativeClass(cx, XPathNSResolverImp::getMetaData()));
    XPathNamespaceImp::setStaticPrivate(new NativeClass(cx, XPathNamespaceImp::getMetaData()));
    XPathResultImp::setStaticPrivate(new NativeClass(cx, XPathResultImp::getMetaData()));

    // file
    FileErrorImp::setStaticPrivate(new NativeClass(cx, FileErrorImp::getMetaData()));
    FileImp::setStaticPrivate(new NativeClass(cx, FileImp::getMetaData()));
    FileListImp::setStaticPrivate(new NativeClass(cx, FileListImp::getMetaData()));
    FileReaderImp::setStaticPrivate(new NativeClass(cx, FileReaderImp::getMetaData()));
    FileReaderSyncImp::setStaticPrivate(new NativeClass(cx, FileReaderSyncImp::getMetaData()));

#if 0
    // TranceMonkey already has these implementaions?
    ArrayBufferImp::setStaticPrivate(new NativeClass(cx, ArrayBufferImp::getMetaData()));
    Float32ArrayImp::setStaticPrivate(new NativeClass(cx, Float32ArrayImp::getMetaData()));
    Float64ArrayImp::setStaticPrivate(new NativeClass(cx, Float64ArrayImp::getMetaData()));
    Int16ArrayImp::setStaticPrivate(new NativeClass(cx, Int16ArrayImp::getMetaData()));
    Int32ArrayImp::setStaticPrivate(new NativeClass(cx, Int32ArrayImp::getMetaData()));
    Int8ArrayImp::setStaticPrivate(new NativeClass(cx, Int8ArrayImp::getMetaData()));
    Uint16ArrayImp::setStaticPrivate(new NativeClass(cx, Uint16ArrayImp::getMetaData()));
    Uint32ArrayImp::setStaticPrivate(new NativeClass(cx, Uint32ArrayImp::getMetaData()));
    Uint8ArrayImp::setStaticPrivate(new NativeClass(cx, Uint8ArrayImp::getMetaData()));
#endif
    ArrayBufferViewImp::setStaticPrivate(new NativeClass(cx, ArrayBufferViewImp::getMetaData()));
    DataViewImp::setStaticPrivate(new NativeClass(cx, DataViewImp::getMetaData()));

    // WebGL
    WebGLObjectImp::setStaticPrivate(new NativeClass(cx, WebGLObjectImp::getMetaData()));
    WebGLActiveInfoImp::setStaticPrivate(new NativeClass(cx, WebGLActiveInfoImp::getMetaData()));
    WebGLBufferImp::setStaticPrivate(new NativeClass(cx, WebGLBufferImp::getMetaData()));
    WebGLContextAttributesImp::setStaticPrivate(new NativeClass(cx, WebGLContextAttributesImp::getMetaData()));
    WebGLContextEventImp::setStaticPrivate(new NativeClass(cx, WebGLContextEventImp::getMetaData()));
    WebGLFramebufferImp::setStaticPrivate(new NativeClass(cx, WebGLFramebufferImp::getMetaData()));
    WebGLProgramImp::setStaticPrivate(new NativeClass(cx, WebGLProgramImp::getMetaData()));
    WebGLRenderbufferImp::setStaticPrivate(new NativeClass(cx, WebGLRenderbufferImp::getMetaData()));
    WebGLRenderingContextImp::setStaticPrivate(new NativeClass(cx, WebGLRenderingContextImp::getMetaData()));
    WebGLShaderImp::setStaticPrivate(new NativeClass(cx, WebGLShaderImp::getMetaData()));
    WebGLTextureImp::setStaticPrivate(new NativeClass(cx, WebGLTextureImp::getMetaData()));
    WebGLUniformLocationImp::setStaticPrivate(new NativeClass(cx, WebGLUniformLocationImp::getMetaData()));

    // ls
    DOMImplementationLSImp::setStaticPrivate(new NativeClass(cx, DOMImplementationLSImp::getMetaData()));
    LSInputImp::setStaticPrivate(new NativeClass(cx, LSInputImp::getMetaData()));
    LSLoadEventImp::setStaticPrivate(new NativeClass(cx, LSLoadEventImp::getMetaData()));
    LSOutputImp::setStaticPrivate(new NativeClass(cx, LSOutputImp::getMetaData()));
    LSParserFilterImp::setStaticPrivate(new NativeClass(cx, LSParserFilterImp::getMetaData()));
    LSParserImp::setStaticPrivate(new NativeClass(cx, LSParserImp::getMetaData()));
    LSProgressEventImp::setStaticPrivate(new NativeClass(cx, LSProgressEventImp::getMetaData()));
    LSResourceResolverImp::setStaticPrivate(new NativeClass(cx, LSResourceResolverImp::getMetaData()));
    LSSerializerFilterImp::setStaticPrivate(new NativeClass(cx, LSSerializerFilterImp::getMetaData()));
    LSSerializerImp::setStaticPrivate(new NativeClass(cx, LSSerializerImp::getMetaData()));
}

// An implementation of the Window's alert operation.
void WindowImp::alert(std::u16string message)
{
    for (auto i = message.begin(); i != message.end(); ++i) {
        if (*i < 128)
            std::cerr << static_cast<char>(*i);
    }
}

// An implementation of the Window's window attribute.
html::Window WindowImp::getWindow()
{
    return this;
}

const char* script =
    "for (prop in Node.prototype)"
    "    window.alert(prop + '\\n');"
    "alert('Hello, world.\\n');";

int main(int argc, const char* argv[])
{
    JSRuntime* rt = JS_NewRuntime(8L * 1024L * 1024L);
    if (!rt)
        return EXIT_FAILURE;

    jscontext = JS_NewContext(rt, 8192);
    if (!jscontext)
        return EXIT_FAILURE;
    JS_SetOptions(jscontext, JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
    JS_SetVersion(jscontext, JSVERSION_LATEST);
    JS_SetErrorReporter(jscontext, reportError);

    JSObject* global = JS_NewCompartmentAndGlobalObject(jscontext, &globalClass, NULL);
    if (!global)
        return EXIT_FAILURE;
    if (!JS_InitStandardClasses(jscontext, global))
        return EXIT_FAILURE;

    // Register classes
    registerClasses(jscontext);

    // Set up Window as the global object
    Reflect::Interface globalMeta(html::Window::getMetaData());
    std::string name = Reflect::getIdentifier(globalMeta.getName());
    if (0 < name.length()) {
        jsval val;
        if (JS_GetProperty(jscontext, global, name.c_str(), &val) && JSVAL_IS_OBJECT(val)) {
            JSObject* parent = JSVAL_TO_OBJECT(val);
            if (JS_GetProperty(jscontext, parent, "prototype", &val) && JSVAL_IS_OBJECT(val)) {
                JSObject* proto = JSVAL_TO_OBJECT(val);
                JS_SetPrototype(jscontext, global, proto);
            }
        }
    }
    window = new WindowImp;
    static_cast<WindowImp*>(window.self())->setPrivate(global);
    JS_SetPrivate(jscontext, global, window.self());

    jsval rval;
    const char* filename = "";
    int lineno = 0;
    JSBool ok = JS_EvaluateScript(jscontext, global, script, strlen(script), filename, lineno, &rval);

    JS_DestroyContext(jscontext);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
}
