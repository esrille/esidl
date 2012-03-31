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
#include <org/w3c/dom/bootstrap/AudioTrackImp.h>
#include <org/w3c/dom/bootstrap/AudioTrackListImp.h>
#include <org/w3c/dom/bootstrap/BarPropImp.h>
#include <org/w3c/dom/bootstrap/BeforeUnloadEventImp.h>
#include <org/w3c/dom/bootstrap/BlobCallbackImp.h>
#include <org/w3c/dom/bootstrap/BlobImp.h>
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
#include <org/w3c/dom/bootstrap/CharacterDataImp.h>
#include <org/w3c/dom/bootstrap/ClientRectImp.h>
#include <org/w3c/dom/bootstrap/ClientRectListImp.h>
#include <org/w3c/dom/bootstrap/CommentImp.h>
#include <org/w3c/dom/bootstrap/CompositionEventImp.h>
#include <org/w3c/dom/bootstrap/CounterImp.h>
#include <org/w3c/dom/bootstrap/CustomEventImp.h>
#include <org/w3c/dom/bootstrap/CustomEventInitImp.h>
#include <org/w3c/dom/bootstrap/DOMElementMapImp.h>
#include <org/w3c/dom/bootstrap/DOMImplementationCSSImp.h>
#include <org/w3c/dom/bootstrap/DOMImplementationImp.h>
#include <org/w3c/dom/bootstrap/DOMSettableTokenListImp.h>
#include <org/w3c/dom/bootstrap/DOMStringListImp.h>
#include <org/w3c/dom/bootstrap/DOMStringMapImp.h>
#include <org/w3c/dom/bootstrap/DOMTokenListImp.h>
#include <org/w3c/dom/bootstrap/DataTransferImp.h>
#include <org/w3c/dom/bootstrap/DataTransferItemImp.h>
#include <org/w3c/dom/bootstrap/DataTransferItemListImp.h>
#include <org/w3c/dom/bootstrap/DataViewImp.h>
#include <org/w3c/dom/bootstrap/DocumentCSSImp.h>
#include <org/w3c/dom/bootstrap/DocumentFragmentImp.h>
#include <org/w3c/dom/bootstrap/DocumentImp.h>
#include <org/w3c/dom/bootstrap/DocumentRangeImp.h>
#include <org/w3c/dom/bootstrap/DocumentTraversalImp.h>
#include <org/w3c/dom/bootstrap/DocumentTypeImp.h>
#include <org/w3c/dom/bootstrap/DragEventImp.h>
#include <org/w3c/dom/bootstrap/ElementCSSInlineStyleImp.h>
#include <org/w3c/dom/bootstrap/ElementImp.h>
#include <org/w3c/dom/bootstrap/EventImp.h>
#include <org/w3c/dom/bootstrap/EventInitImp.h>
#include <org/w3c/dom/bootstrap/EventListenerImp.h>
#include <org/w3c/dom/bootstrap/EventSourceImp.h>
#include <org/w3c/dom/bootstrap/EventTargetImp.h>
#include <org/w3c/dom/bootstrap/ExternalImp.h>
#include <org/w3c/dom/bootstrap/FileCallbackImp.h>
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
#include <org/w3c/dom/bootstrap/FunctionStringCallbackImp.h>
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
#include <org/w3c/dom/bootstrap/HTMLTrackElementImp.h>
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
#include <org/w3c/dom/bootstrap/LinkStyleImp.h>
// #include <org/w3c/dom/bootstrap/LocalMediaStreamImp.h>
#include <org/w3c/dom/bootstrap/LocationImp.h>
#include <org/w3c/dom/bootstrap/MediaControllerImp.h>
#include <org/w3c/dom/bootstrap/MediaErrorImp.h>
#include <org/w3c/dom/bootstrap/MediaListImp.h>
#include <org/w3c/dom/bootstrap/MediaQueryListImp.h>
#include <org/w3c/dom/bootstrap/MediaQueryListListenerImp.h>
// #include <org/w3c/dom/bootstrap/MediaStreamImp.h>
#include <org/w3c/dom/bootstrap/MediaStreamRecorderImp.h>
#include <org/w3c/dom/bootstrap/MessageChannelImp.h>
#include <org/w3c/dom/bootstrap/MessageEventImp.h>
#include <org/w3c/dom/bootstrap/MessagePortImp.h>
#include <org/w3c/dom/bootstrap/MouseEventImp.h>
#include <org/w3c/dom/bootstrap/MutableTextTrackImp.h>
#include <org/w3c/dom/bootstrap/MutationEventImp.h>
#include <org/w3c/dom/bootstrap/MutationNameEventImp.h>
#include <org/w3c/dom/bootstrap/NavigatorImp.h>
#include <org/w3c/dom/bootstrap/NavigatorUserMediaErrorCallbackImp.h>
#include <org/w3c/dom/bootstrap/NavigatorUserMediaErrorImp.h>
// #include <org/w3c/dom/bootstrap/NavigatorUserMediaSuccessCallbackImp.h>
#include <org/w3c/dom/bootstrap/NodeFilterImp.h>
#include <org/w3c/dom/bootstrap/NodeImp.h>
#include <org/w3c/dom/bootstrap/NodeIteratorImp.h>
#include <org/w3c/dom/bootstrap/NodeListImp.h>
#include <org/w3c/dom/bootstrap/PageTransitionEventImp.h>
// #include <org/w3c/dom/bootstrap/PeerConnectionImp.h>
#include <org/w3c/dom/bootstrap/PopStateEventImp.h>
#include <org/w3c/dom/bootstrap/ProcessingInstructionImp.h>
#include <org/w3c/dom/bootstrap/ProgressEventImp.h>
#include <org/w3c/dom/bootstrap/PropertyNodeListImp.h>
#include <org/w3c/dom/bootstrap/RGBColorImp.h>
#include <org/w3c/dom/bootstrap/RadioNodeListImp.h>
#include <org/w3c/dom/bootstrap/RangeImp.h>
#include <org/w3c/dom/bootstrap/RectImp.h>
#include <org/w3c/dom/bootstrap/ScreenImp.h>
#include <org/w3c/dom/bootstrap/SignalingCallbackImp.h>
// #include <org/w3c/dom/bootstrap/StreamEventImp.h>
#include <org/w3c/dom/bootstrap/StreamTrackImp.h>
#include <org/w3c/dom/bootstrap/StyleSheetImp.h>
#include <org/w3c/dom/bootstrap/TextEventImp.h>
#include <org/w3c/dom/bootstrap/TextImp.h>
#include <org/w3c/dom/bootstrap/TextMetricsImp.h>
#include <org/w3c/dom/bootstrap/TextTrackCueImp.h>
#include <org/w3c/dom/bootstrap/TextTrackCueListImp.h>
#include <org/w3c/dom/bootstrap/TextTrackImp.h>
#include <org/w3c/dom/bootstrap/TimeRangesImp.h>
#include <org/w3c/dom/bootstrap/TransferableImp.h>
#include <org/w3c/dom/bootstrap/TreeWalkerImp.h>
#include <org/w3c/dom/bootstrap/UIEventImp.h>
#include <org/w3c/dom/bootstrap/Uint16ArrayImp.h>
#include <org/w3c/dom/bootstrap/Uint32ArrayImp.h>
#include <org/w3c/dom/bootstrap/Uint8ArrayImp.h>
#include <org/w3c/dom/bootstrap/UndoManagerEventImp.h>
#include <org/w3c/dom/bootstrap/UndoManagerImp.h>
#include <org/w3c/dom/bootstrap/ValidityStateImp.h>
#include <org/w3c/dom/bootstrap/VideoTrackImp.h>
#include <org/w3c/dom/bootstrap/VideoTrackListImp.h>
#include <org/w3c/dom/bootstrap/WheelEventImp.h>
#include <org/w3c/dom/bootstrap/WindowImp.h>
#include <org/w3c/dom/bootstrap/WindowModalImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestEventTargetImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestImp.h>
#include <org/w3c/dom/bootstrap/XMLHttpRequestUploadImp.h>

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

void registerClasses(JSContext* cx, JSObject* global)
{
    // Base classes have to be registered firstly.
    DOMTokenListImp::setStaticPrivate(new NativeClass(cx, global, DOMTokenListImp::getMetaData()));
    EventTargetImp::setStaticPrivate(new NativeClass(cx, global, EventTargetImp::getMetaData()));
    XMLHttpRequestEventTargetImp::setStaticPrivate(new NativeClass(cx, global, XMLHttpRequestEventTargetImp::getMetaData()));
    EventImp::setStaticPrivate(new NativeClass(cx, global, EventImp::getMetaData()));
    UIEventImp::setStaticPrivate(new NativeClass(cx, global, UIEventImp::getMetaData()));
    MouseEventImp::setStaticPrivate(new NativeClass(cx, global, MouseEventImp::getMetaData()));
    EventInitImp::setStaticPrivate(new NativeClass(cx, global, EventInitImp::getMetaData()));
    NodeImp::setStaticPrivate(new NativeClass(cx, global, NodeImp::getMetaData()));
    ElementImp::setStaticPrivate(new NativeClass(cx, global, ElementImp::getMetaData()));
    HTMLElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLElementImp::getMetaData()));
    HTMLMediaElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMediaElementImp::getMetaData()));
    HTMLCollectionImp::setStaticPrivate(new NativeClass(cx, global, HTMLCollectionImp::getMetaData()));
    StyleSheetImp::setStaticPrivate(new NativeClass(cx, global, StyleSheetImp::getMetaData()));
    CSSRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSRuleImp::getMetaData()));
    CSSValueImp::setStaticPrivate(new NativeClass(cx, global, CSSValueImp::getMetaData()));
    XMLHttpRequestImp::setStaticPrivate(new NativeClass(cx, global, XMLHttpRequestImp::getMetaData()));
    TextTrackImp::setStaticPrivate(new NativeClass(cx, global, TextTrackImp::getMetaData()));

    AnonXMLHttpRequestImp::setStaticPrivate(new NativeClass(cx, global, AnonXMLHttpRequestImp::getMetaData()));
    ApplicationCacheImp::setStaticPrivate(new NativeClass(cx, global, ApplicationCacheImp::getMetaData()));
    ArrayBufferImp::setStaticPrivate(new NativeClass(cx, global, ArrayBufferImp::getMetaData()));
    ArrayBufferViewImp::setStaticPrivate(new NativeClass(cx, global, ArrayBufferViewImp::getMetaData()));
    AttrImp::setStaticPrivate(new NativeClass(cx, global, AttrImp::getMetaData()));
    AudioTrackImp::setStaticPrivate(new NativeClass(cx, global, AudioTrackImp::getMetaData()));
    AudioTrackListImp::setStaticPrivate(new NativeClass(cx, global, AudioTrackListImp::getMetaData()));
    BarPropImp::setStaticPrivate(new NativeClass(cx, global, BarPropImp::getMetaData()));
    BeforeUnloadEventImp::setStaticPrivate(new NativeClass(cx, global, BeforeUnloadEventImp::getMetaData()));
    BlobCallbackImp::setStaticPrivate(new NativeClass(cx, global, BlobCallbackImp::getMetaData()));
    BlobImp::setStaticPrivate(new NativeClass(cx, global, BlobImp::getMetaData()));
    CSS2PropertiesImp::setStaticPrivate(new NativeClass(cx, global, CSS2PropertiesImp::getMetaData()));
    CSSCharsetRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSCharsetRuleImp::getMetaData()));
    CSSColorComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSColorComponentValueImp::getMetaData()));
    CSSComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSComponentValueImp::getMetaData()));
    CSSFontFaceRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSFontFaceRuleImp::getMetaData()));
    CSSIdentifierComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSIdentifierComponentValueImp::getMetaData()));
    CSSImportRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSImportRuleImp::getMetaData()));
    CSSKeywordComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSKeywordComponentValueImp::getMetaData()));
    CSSLengthComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSLengthComponentValueImp::getMetaData()));
    CSSMapValueImp::setStaticPrivate(new NativeClass(cx, global, CSSMapValueImp::getMetaData()));
    CSSMediaRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSMediaRuleImp::getMetaData()));
    CSSNamespaceRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSNamespaceRuleImp::getMetaData()));
    CSSPageRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSPageRuleImp::getMetaData()));
    CSSPercentageComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSPercentageComponentValueImp::getMetaData()));
    CSSPrimitiveValueImp::setStaticPrivate(new NativeClass(cx, global, CSSPrimitiveValueImp::getMetaData()));
    CSSPropertyValueImp::setStaticPrivate(new NativeClass(cx, global, CSSPropertyValueImp::getMetaData()));
    CSSPropertyValueListImp::setStaticPrivate(new NativeClass(cx, global, CSSPropertyValueListImp::getMetaData()));
    CSSStringComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSStringComponentValueImp::getMetaData()));
    CSSStyleDeclarationImp::setStaticPrivate(new NativeClass(cx, global, CSSStyleDeclarationImp::getMetaData()));
    CSSStyleDeclarationValueImp::setStaticPrivate(new NativeClass(cx, global, CSSStyleDeclarationValueImp::getMetaData()));
    CSSStyleRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSStyleRuleImp::getMetaData()));
    CSSStyleSheetImp::setStaticPrivate(new NativeClass(cx, global, CSSStyleSheetImp::getMetaData()));
    CSSURLComponentValueImp::setStaticPrivate(new NativeClass(cx, global, CSSURLComponentValueImp::getMetaData()));
    CSSUnknownRuleImp::setStaticPrivate(new NativeClass(cx, global, CSSUnknownRuleImp::getMetaData()));
    CSSValueListImp::setStaticPrivate(new NativeClass(cx, global, CSSValueListImp::getMetaData()));
    CanvasGradientImp::setStaticPrivate(new NativeClass(cx, global, CanvasGradientImp::getMetaData()));
    CanvasPatternImp::setStaticPrivate(new NativeClass(cx, global, CanvasPatternImp::getMetaData()));
    CanvasPixelArrayImp::setStaticPrivate(new NativeClass(cx, global, CanvasPixelArrayImp::getMetaData()));
    CanvasRenderingContext2DImp::setStaticPrivate(new NativeClass(cx, global, CanvasRenderingContext2DImp::getMetaData()));
    CaretPositionImp::setStaticPrivate(new NativeClass(cx, global, CaretPositionImp::getMetaData()));
    CharacterDataImp::setStaticPrivate(new NativeClass(cx, global, CharacterDataImp::getMetaData()));
    ClientRectImp::setStaticPrivate(new NativeClass(cx, global, ClientRectImp::getMetaData()));
    ClientRectListImp::setStaticPrivate(new NativeClass(cx, global, ClientRectListImp::getMetaData()));
    CommentImp::setStaticPrivate(new NativeClass(cx, global, CommentImp::getMetaData()));
    CompositionEventImp::setStaticPrivate(new NativeClass(cx, global, CompositionEventImp::getMetaData()));
    CounterImp::setStaticPrivate(new NativeClass(cx, global, CounterImp::getMetaData()));
    CustomEventImp::setStaticPrivate(new NativeClass(cx, global, CustomEventImp::getMetaData()));
    CustomEventInitImp::setStaticPrivate(new NativeClass(cx, global, CustomEventInitImp::getMetaData()));
    DOMElementMapImp::setStaticPrivate(new NativeClass(cx, global, DOMElementMapImp::getMetaData()));
    DOMImplementationCSSImp::setStaticPrivate(new NativeClass(cx, global, DOMImplementationCSSImp::getMetaData()));
    DOMImplementationImp::setStaticPrivate(new NativeClass(cx, global, DOMImplementationImp::getMetaData()));
    DOMSettableTokenListImp::setStaticPrivate(new NativeClass(cx, global, DOMSettableTokenListImp::getMetaData()));
    DOMStringListImp::setStaticPrivate(new NativeClass(cx, global, DOMStringListImp::getMetaData()));
    DOMStringMapImp::setStaticPrivate(new NativeClass(cx, global, DOMStringMapImp::getMetaData()));
    DataTransferImp::setStaticPrivate(new NativeClass(cx, global, DataTransferImp::getMetaData()));
    DataTransferItemImp::setStaticPrivate(new NativeClass(cx, global, DataTransferItemImp::getMetaData()));
    DataTransferItemListImp::setStaticPrivate(new NativeClass(cx, global, DataTransferItemListImp::getMetaData()));
    DataViewImp::setStaticPrivate(new NativeClass(cx, global, DataViewImp::getMetaData()));
    DocumentCSSImp::setStaticPrivate(new NativeClass(cx, global, DocumentCSSImp::getMetaData()));
    DocumentFragmentImp::setStaticPrivate(new NativeClass(cx, global, DocumentFragmentImp::getMetaData()));
    DocumentImp::setStaticPrivate(new NativeClass(cx, global, DocumentImp::getMetaData()));
    DocumentRangeImp::setStaticPrivate(new NativeClass(cx, global, DocumentRangeImp::getMetaData()));
    DocumentTraversalImp::setStaticPrivate(new NativeClass(cx, global, DocumentTraversalImp::getMetaData()));
    DocumentTypeImp::setStaticPrivate(new NativeClass(cx, global, DocumentTypeImp::getMetaData()));
    DragEventImp::setStaticPrivate(new NativeClass(cx, global, DragEventImp::getMetaData()));
    ElementCSSInlineStyleImp::setStaticPrivate(new NativeClass(cx, global, ElementCSSInlineStyleImp::getMetaData()));
    EventListenerImp::setStaticPrivate(new NativeClass(cx, global, EventListenerImp::getMetaData()));
    EventSourceImp::setStaticPrivate(new NativeClass(cx, global, EventSourceImp::getMetaData()));
    ExternalImp::setStaticPrivate(new NativeClass(cx, global, ExternalImp::getMetaData()));
    FileCallbackImp::setStaticPrivate(new NativeClass(cx, global, FileCallbackImp::getMetaData()));
    FileErrorImp::setStaticPrivate(new NativeClass(cx, global, FileErrorImp::getMetaData()));
    FileImp::setStaticPrivate(new NativeClass(cx, global, FileImp::getMetaData()));
    FileListImp::setStaticPrivate(new NativeClass(cx, global, FileListImp::getMetaData()));
    FileReaderImp::setStaticPrivate(new NativeClass(cx, global, FileReaderImp::getMetaData()));
    FileReaderSyncImp::setStaticPrivate(new NativeClass(cx, global, FileReaderSyncImp::getMetaData()));
    Float32ArrayImp::setStaticPrivate(new NativeClass(cx, global, Float32ArrayImp::getMetaData()));
    Float64ArrayImp::setStaticPrivate(new NativeClass(cx, global, Float64ArrayImp::getMetaData()));
    FocusEventImp::setStaticPrivate(new NativeClass(cx, global, FocusEventImp::getMetaData()));
    FormDataImp::setStaticPrivate(new NativeClass(cx, global, FormDataImp::getMetaData()));
    FunctionImp::setStaticPrivate(new NativeClass(cx, global, FunctionImp::getMetaData()));
    FunctionStringCallbackImp::setStaticPrivate(new NativeClass(cx, global, FunctionStringCallbackImp::getMetaData()));
    HTMLAllCollectionImp::setStaticPrivate(new NativeClass(cx, global, HTMLAllCollectionImp::getMetaData()));
    HTMLAnchorElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLAnchorElementImp::getMetaData()));
    HTMLAppletElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLAppletElementImp::getMetaData()));
    HTMLAreaElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLAreaElementImp::getMetaData()));
    HTMLAudioElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLAudioElementImp::getMetaData()));
    HTMLBRElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLBRElementImp::getMetaData()));
    HTMLBaseElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLBaseElementImp::getMetaData()));
    HTMLBaseFontElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLBaseFontElementImp::getMetaData()));
    HTMLBodyElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLBodyElementImp::getMetaData()));
    HTMLButtonElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLButtonElementImp::getMetaData()));
    HTMLCanvasElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLCanvasElementImp::getMetaData()));
    HTMLCommandElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLCommandElementImp::getMetaData()));
    HTMLDListElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLDListElementImp::getMetaData()));
    HTMLDataListElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLDataListElementImp::getMetaData()));
    HTMLDetailsElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLDetailsElementImp::getMetaData()));
    HTMLDirectoryElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLDirectoryElementImp::getMetaData()));
    HTMLDivElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLDivElementImp::getMetaData()));
    HTMLDocumentImp::setStaticPrivate(new NativeClass(cx, global, HTMLDocumentImp::getMetaData()));
    HTMLEmbedElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLEmbedElementImp::getMetaData()));
    HTMLFieldSetElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLFieldSetElementImp::getMetaData()));
    HTMLFontElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLFontElementImp::getMetaData()));
    HTMLFormControlsCollectionImp::setStaticPrivate(new NativeClass(cx, global, HTMLFormControlsCollectionImp::getMetaData()));
    HTMLFormElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLFormElementImp::getMetaData()));
    HTMLFrameElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLFrameElementImp::getMetaData()));
    HTMLFrameSetElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLFrameSetElementImp::getMetaData()));
    HTMLHRElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLHRElementImp::getMetaData()));
    HTMLHeadElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLHeadElementImp::getMetaData()));
    HTMLHeadingElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLHeadingElementImp::getMetaData()));
    HTMLHtmlElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLHtmlElementImp::getMetaData()));
    HTMLIFrameElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLIFrameElementImp::getMetaData()));
    HTMLImageElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLImageElementImp::getMetaData()));
    HTMLInputElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLInputElementImp::getMetaData()));
    HTMLKeygenElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLKeygenElementImp::getMetaData()));
    HTMLLIElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLLIElementImp::getMetaData()));
    HTMLLabelElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLLabelElementImp::getMetaData()));
    HTMLLegendElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLLegendElementImp::getMetaData()));
    HTMLLinkElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLLinkElementImp::getMetaData()));
    HTMLMapElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMapElementImp::getMetaData()));
    HTMLMarqueeElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMarqueeElementImp::getMetaData()));
    HTMLMenuElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMenuElementImp::getMetaData()));
    HTMLMetaElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMetaElementImp::getMetaData()));
    HTMLMeterElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLMeterElementImp::getMetaData()));
    HTMLModElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLModElementImp::getMetaData()));
    HTMLOListElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLOListElementImp::getMetaData()));
    HTMLObjectElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLObjectElementImp::getMetaData()));
    HTMLOptGroupElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLOptGroupElementImp::getMetaData()));
    HTMLOptionElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLOptionElementImp::getMetaData()));
    HTMLOptionsCollectionImp::setStaticPrivate(new NativeClass(cx, global, HTMLOptionsCollectionImp::getMetaData()));
    HTMLOutputElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLOutputElementImp::getMetaData()));
    HTMLParagraphElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLParagraphElementImp::getMetaData()));
    HTMLParamElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLParamElementImp::getMetaData()));
    HTMLPreElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLPreElementImp::getMetaData()));
    HTMLProgressElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLProgressElementImp::getMetaData()));
    HTMLPropertiesCollectionImp::setStaticPrivate(new NativeClass(cx, global, HTMLPropertiesCollectionImp::getMetaData()));
    HTMLQuoteElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLQuoteElementImp::getMetaData()));
    HTMLScriptElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLScriptElementImp::getMetaData()));
    HTMLSelectElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLSelectElementImp::getMetaData()));
    HTMLSourceElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLSourceElementImp::getMetaData()));
    HTMLSpanElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLSpanElementImp::getMetaData()));
    HTMLStyleElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLStyleElementImp::getMetaData()));
    HTMLTableCaptionElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableCaptionElementImp::getMetaData()));
    HTMLTableCellElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableCellElementImp::getMetaData()));
    HTMLTableColElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableColElementImp::getMetaData()));
    HTMLTableDataCellElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableDataCellElementImp::getMetaData()));
    HTMLTableElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableElementImp::getMetaData()));
    HTMLTableHeaderCellElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableHeaderCellElementImp::getMetaData()));
    HTMLTableRowElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableRowElementImp::getMetaData()));
    HTMLTableSectionElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTableSectionElementImp::getMetaData()));
    HTMLTextAreaElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTextAreaElementImp::getMetaData()));
    HTMLTimeElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTimeElementImp::getMetaData()));
    HTMLTitleElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTitleElementImp::getMetaData()));
    HTMLTrackElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLTrackElementImp::getMetaData()));
    HTMLUListElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLUListElementImp::getMetaData()));
    HTMLUnknownElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLUnknownElementImp::getMetaData()));
    HTMLVideoElementImp::setStaticPrivate(new NativeClass(cx, global, HTMLVideoElementImp::getMetaData()));
    HashChangeEventImp::setStaticPrivate(new NativeClass(cx, global, HashChangeEventImp::getMetaData()));
    HistoryImp::setStaticPrivate(new NativeClass(cx, global, HistoryImp::getMetaData()));
    ImageDataImp::setStaticPrivate(new NativeClass(cx, global, ImageDataImp::getMetaData()));
    Int16ArrayImp::setStaticPrivate(new NativeClass(cx, global, Int16ArrayImp::getMetaData()));
    Int32ArrayImp::setStaticPrivate(new NativeClass(cx, global, Int32ArrayImp::getMetaData()));
    Int8ArrayImp::setStaticPrivate(new NativeClass(cx, global, Int8ArrayImp::getMetaData()));
    KeyboardEventImp::setStaticPrivate(new NativeClass(cx, global, KeyboardEventImp::getMetaData()));
    LinkStyleImp::setStaticPrivate(new NativeClass(cx, global, LinkStyleImp::getMetaData()));
    // LocalMediaStreamImp::setStaticPrivate(new NativeClass(cx, global, LocalMediaStreamImp::getMetaData()));
    LocationImp::setStaticPrivate(new NativeClass(cx, global, LocationImp::getMetaData()));
    MediaControllerImp::setStaticPrivate(new NativeClass(cx, global, MediaControllerImp::getMetaData()));
    MediaErrorImp::setStaticPrivate(new NativeClass(cx, global, MediaErrorImp::getMetaData()));
    MediaListImp::setStaticPrivate(new NativeClass(cx, global, MediaListImp::getMetaData()));
    MediaQueryListImp::setStaticPrivate(new NativeClass(cx, global, MediaQueryListImp::getMetaData()));
    MediaQueryListListenerImp::setStaticPrivate(new NativeClass(cx, global, MediaQueryListListenerImp::getMetaData()));
    // MediaStreamImp::setStaticPrivate(new NativeClass(cx, global, MediaStreamImp::getMetaData()));
    MediaStreamRecorderImp::setStaticPrivate(new NativeClass(cx, global, MediaStreamRecorderImp::getMetaData()));
    MessageChannelImp::setStaticPrivate(new NativeClass(cx, global, MessageChannelImp::getMetaData()));
    MessageEventImp::setStaticPrivate(new NativeClass(cx, global, MessageEventImp::getMetaData()));
    MessagePortImp::setStaticPrivate(new NativeClass(cx, global, MessagePortImp::getMetaData()));
    MutableTextTrackImp::setStaticPrivate(new NativeClass(cx, global, MutableTextTrackImp::getMetaData()));
    MutationEventImp::setStaticPrivate(new NativeClass(cx, global, MutationEventImp::getMetaData()));
    MutationNameEventImp::setStaticPrivate(new NativeClass(cx, global, MutationNameEventImp::getMetaData()));
    NavigatorImp::setStaticPrivate(new NativeClass(cx, global, NavigatorImp::getMetaData()));
    NavigatorUserMediaErrorCallbackImp::setStaticPrivate(new NativeClass(cx, global, NavigatorUserMediaErrorCallbackImp::getMetaData()));
    NavigatorUserMediaErrorImp::setStaticPrivate(new NativeClass(cx, global, NavigatorUserMediaErrorImp::getMetaData()));
    // NavigatorUserMediaSuccessCallbackImp::setStaticPrivate(new NativeClass(cx, global, NavigatorUserMediaSuccessCallbackImp::getMetaData()));
    NodeFilterImp::setStaticPrivate(new NativeClass(cx, global, NodeFilterImp::getMetaData()));
    NodeIteratorImp::setStaticPrivate(new NativeClass(cx, global, NodeIteratorImp::getMetaData()));
    NodeListImp::setStaticPrivate(new NativeClass(cx, global, NodeListImp::getMetaData()));
    PageTransitionEventImp::setStaticPrivate(new NativeClass(cx, global, PageTransitionEventImp::getMetaData()));
    // PeerConnectionImp::setStaticPrivate(new NativeClass(cx, global, PeerConnectionImp::getMetaData()));
    PopStateEventImp::setStaticPrivate(new NativeClass(cx, global, PopStateEventImp::getMetaData()));
    ProcessingInstructionImp::setStaticPrivate(new NativeClass(cx, global, ProcessingInstructionImp::getMetaData()));
    ProgressEventImp::setStaticPrivate(new NativeClass(cx, global, ProgressEventImp::getMetaData()));
    PropertyNodeListImp::setStaticPrivate(new NativeClass(cx, global, PropertyNodeListImp::getMetaData()));
    RGBColorImp::setStaticPrivate(new NativeClass(cx, global, RGBColorImp::getMetaData()));
    RadioNodeListImp::setStaticPrivate(new NativeClass(cx, global, RadioNodeListImp::getMetaData()));
    RangeImp::setStaticPrivate(new NativeClass(cx, global, RangeImp::getMetaData()));
    RectImp::setStaticPrivate(new NativeClass(cx, global, RectImp::getMetaData()));
    ScreenImp::setStaticPrivate(new NativeClass(cx, global, ScreenImp::getMetaData()));
    SignalingCallbackImp::setStaticPrivate(new NativeClass(cx, global, SignalingCallbackImp::getMetaData()));
    // StreamEventImp::setStaticPrivate(new NativeClass(cx, global, StreamEventImp::getMetaData()));
    StreamTrackImp::setStaticPrivate(new NativeClass(cx, global, StreamTrackImp::getMetaData()));
    TextEventImp::setStaticPrivate(new NativeClass(cx, global, TextEventImp::getMetaData()));
    TextImp::setStaticPrivate(new NativeClass(cx, global, TextImp::getMetaData()));
    TextMetricsImp::setStaticPrivate(new NativeClass(cx, global, TextMetricsImp::getMetaData()));
    TextTrackCueImp::setStaticPrivate(new NativeClass(cx, global, TextTrackCueImp::getMetaData()));
    TextTrackCueListImp::setStaticPrivate(new NativeClass(cx, global, TextTrackCueListImp::getMetaData()));
    TimeRangesImp::setStaticPrivate(new NativeClass(cx, global, TimeRangesImp::getMetaData()));
    TransferableImp::setStaticPrivate(new NativeClass(cx, global, TransferableImp::getMetaData()));
    TreeWalkerImp::setStaticPrivate(new NativeClass(cx, global, TreeWalkerImp::getMetaData()));
    Uint16ArrayImp::setStaticPrivate(new NativeClass(cx, global, Uint16ArrayImp::getMetaData()));
    Uint32ArrayImp::setStaticPrivate(new NativeClass(cx, global, Uint32ArrayImp::getMetaData()));
    Uint8ArrayImp::setStaticPrivate(new NativeClass(cx, global, Uint8ArrayImp::getMetaData()));
    UndoManagerEventImp::setStaticPrivate(new NativeClass(cx, global, UndoManagerEventImp::getMetaData()));
    UndoManagerImp::setStaticPrivate(new NativeClass(cx, global, UndoManagerImp::getMetaData()));
    ValidityStateImp::setStaticPrivate(new NativeClass(cx, global, ValidityStateImp::getMetaData()));
    VideoTrackImp::setStaticPrivate(new NativeClass(cx, global, VideoTrackImp::getMetaData()));
    VideoTrackListImp::setStaticPrivate(new NativeClass(cx, global, VideoTrackListImp::getMetaData()));
    WheelEventImp::setStaticPrivate(new NativeClass(cx, global, WheelEventImp::getMetaData()));
    WindowImp::setStaticPrivate(new NativeClass(cx, global, WindowImp::getMetaData()));
    WindowModalImp::setStaticPrivate(new NativeClass(cx, global, WindowModalImp::getMetaData()));
    XMLHttpRequestUploadImp::setStaticPrivate(new NativeClass(cx, global, XMLHttpRequestUploadImp::getMetaData()));
}

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
    registerClasses(jscontext, global);

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

    JS_DestroyContext(jscontext);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
}
