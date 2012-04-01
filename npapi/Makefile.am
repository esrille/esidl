SUBDIRS = . sample

SUFFIXES = .idl

vpath %.idl = $(srcdir)/../dom

if NACL
esidl=esidl
else
esidl=../esidl
endif

idl_files = \
	arrays.idl \
	cssom.idl \
	cssomview.idl \
	dom2css.idl \
	dom2ranges.idl \
	dom2traversal.idl \
	dom3events.idl \
	domcore.idl \
	eventsource.idl \
	file.idl \
	html5.idl \
	progress-events.idl \
	selectors.idl \
	typedarray.idl \
	xhr2.idl

generated_headers = \
	org/w3c/dom/AnonXMLHttpRequest.h \
	org/w3c/dom/Attr.h \
	org/w3c/dom/CaretPosition.h \
	org/w3c/dom/CharacterData.h \
	org/w3c/dom/Comment.h \
	org/w3c/dom/DocumentFragment.h \
	org/w3c/dom/Document.h \
	org/w3c/dom/DocumentType.h \
	org/w3c/dom/DOMException.h \
	org/w3c/dom/DOMImplementation.h \
	org/w3c/dom/DOMSettableTokenList.h \
	org/w3c/dom/DOMStringList.h \
	org/w3c/dom/DOMStringMap.h \
	org/w3c/dom/DOMTokenList.h \
	org/w3c/dom/Element.h \
	org/w3c/dom/FormData.h \
	org/w3c/dom/Node.h \
	org/w3c/dom/NodeList.h \
	org/w3c/dom/ProcessingInstruction.h \
	org/w3c/dom/Text.h \
	org/w3c/dom/XMLHttpRequestEventTarget.h \
	org/w3c/dom/XMLHttpRequest.h \
	org/w3c/dom/XMLHttpRequestUpload.h \
	org/w3c/dom/css/Counter.h \
	org/w3c/dom/css/CSS2Properties.h \
	org/w3c/dom/css/CSSCharsetRule.h \
	org/w3c/dom/css/CSSColorComponentValue.h \
	org/w3c/dom/css/CSSComponentValue.h \
	org/w3c/dom/css/CSSFontFaceRule.h \
	org/w3c/dom/css/CSSImportRule.h \
	org/w3c/dom/css/CSSLengthComponentValue.h \
	org/w3c/dom/css/CSSMapValue.h \
	org/w3c/dom/css/CSSMediaRule.h \
	org/w3c/dom/css/CSSNamespaceRule.h \
	org/w3c/dom/css/CSSPageRule.h \
	org/w3c/dom/css/CSSPercentageComponentValue.h \
	org/w3c/dom/css/CSSPrimitiveValue.h \
	org/w3c/dom/css/CSSPropertyValueList.h \
	org/w3c/dom/css/CSSRule.h \
	org/w3c/dom/css/CSSStyleDeclaration.h \
	org/w3c/dom/css/CSSStyleDeclarationValue.h \
	org/w3c/dom/css/CSSStyleRule.h \
	org/w3c/dom/css/CSSStyleSheet.h \
	org/w3c/dom/css/CSSUnknownRule.h \
	org/w3c/dom/css/CSSURLComponentValue.h \
	org/w3c/dom/css/CSSValue.h \
	org/w3c/dom/css/CSSValueList.h \
	org/w3c/dom/css/DocumentCSS.h \
	org/w3c/dom/css/DOMImplementationCSS.h \
	org/w3c/dom/css/ElementCSSInlineStyle.h \
	org/w3c/dom/css/Rect.h \
	org/w3c/dom/css/RGBColor.h \
	org/w3c/dom/events/CompositionEvent.h \
	org/w3c/dom/events/CustomEvent.h \
	org/w3c/dom/events/EventException.h \
	org/w3c/dom/events/Event.h \
	org/w3c/dom/events/EventListener.h \
	org/w3c/dom/events/EventTarget.h \
	org/w3c/dom/events/FocusEvent.h \
	org/w3c/dom/events/KeyboardEvent.h \
	org/w3c/dom/events/MouseEvent.h \
	org/w3c/dom/events/MutationEvent.h \
	org/w3c/dom/events/MutationNameEvent.h \
	org/w3c/dom/eventsource/EventSource.h \
	org/w3c/dom/events/ProgressEvent.h \
	org/w3c/dom/events/TextEvent.h \
	org/w3c/dom/events/UIEvent.h \
	org/w3c/dom/events/WheelEvent.h \
	org/w3c/dom/file/Blob.h \
	org/w3c/dom/file/FileError.h \
	org/w3c/dom/file/FileException.h \
	org/w3c/dom/file/File.h \
	org/w3c/dom/file/FileReader.h \
	org/w3c/dom/file/FileReaderSync.h \
	org/w3c/dom/html/ApplicationCache.h \
	org/w3c/dom/html/BarProp.h \
	org/w3c/dom/html/BeforeUnloadEvent.h \
	org/w3c/dom/html/CanvasGradient.h \
	org/w3c/dom/html/CanvasPattern.h \
	org/w3c/dom/html/CanvasPixelArray.h \
	org/w3c/dom/html/CanvasRenderingContext2D.h \
	org/w3c/dom/html/DataTransfer.h \
	org/w3c/dom/html/DragEvent.h \
	org/w3c/dom/html/Function.h \
	org/w3c/dom/html/HashChangeEvent.h \
	org/w3c/dom/html/History.h \
	org/w3c/dom/html/HTMLAllCollection.h \
	org/w3c/dom/html/HTMLAnchorElement.h \
	org/w3c/dom/html/HTMLAppletElement.h \
	org/w3c/dom/html/HTMLAreaElement.h \
	org/w3c/dom/html/HTMLAudioElement.h \
	org/w3c/dom/html/HTMLBaseElement.h \
	org/w3c/dom/html/HTMLBaseFontElement.h \
	org/w3c/dom/html/HTMLBodyElement.h \
	org/w3c/dom/html/HTMLBRElement.h \
	org/w3c/dom/html/HTMLButtonElement.h \
	org/w3c/dom/html/HTMLCanvasElement.h \
	org/w3c/dom/html/HTMLCollection.h \
	org/w3c/dom/html/HTMLCommandElement.h \
	org/w3c/dom/html/HTMLDataListElement.h \
	org/w3c/dom/html/HTMLDetailsElement.h \
	org/w3c/dom/html/HTMLDirectoryElement.h \
	org/w3c/dom/html/HTMLDivElement.h \
	org/w3c/dom/html/HTMLDListElement.h \
	org/w3c/dom/html/HTMLDocument.h \
	org/w3c/dom/html/HTMLElement.h \
	org/w3c/dom/html/HTMLEmbedElement.h \
	org/w3c/dom/html/HTMLFieldSetElement.h \
	org/w3c/dom/html/HTMLFontElement.h \
	org/w3c/dom/html/HTMLFormControlsCollection.h \
	org/w3c/dom/html/HTMLFormElement.h \
	org/w3c/dom/html/HTMLFrameElement.h \
	org/w3c/dom/html/HTMLFrameSetElement.h \
	org/w3c/dom/html/HTMLHeadElement.h \
	org/w3c/dom/html/HTMLHeadingElement.h \
	org/w3c/dom/html/HTMLHRElement.h \
	org/w3c/dom/html/HTMLHtmlElement.h \
	org/w3c/dom/html/HTMLIFrameElement.h \
	org/w3c/dom/html/HTMLImageElement.h \
	org/w3c/dom/html/HTMLInputElement.h \
	org/w3c/dom/html/HTMLKeygenElement.h \
	org/w3c/dom/html/HTMLLabelElement.h \
	org/w3c/dom/html/HTMLLegendElement.h \
	org/w3c/dom/html/HTMLLIElement.h \
	org/w3c/dom/html/HTMLLinkElement.h \
	org/w3c/dom/html/HTMLMapElement.h \
	org/w3c/dom/html/HTMLMarqueeElement.h \
	org/w3c/dom/html/HTMLMediaElement.h \
	org/w3c/dom/html/HTMLMenuElement.h \
	org/w3c/dom/html/HTMLMetaElement.h \
	org/w3c/dom/html/HTMLMeterElement.h \
	org/w3c/dom/html/HTMLModElement.h \
	org/w3c/dom/html/HTMLObjectElement.h \
	org/w3c/dom/html/HTMLOListElement.h \
	org/w3c/dom/html/HTMLOptGroupElement.h \
	org/w3c/dom/html/HTMLOptionElement.h \
	org/w3c/dom/html/HTMLOptionsCollection.h \
	org/w3c/dom/html/HTMLOutputElement.h \
	org/w3c/dom/html/HTMLParagraphElement.h \
	org/w3c/dom/html/HTMLParamElement.h \
	org/w3c/dom/html/HTMLPreElement.h \
	org/w3c/dom/html/HTMLProgressElement.h \
	org/w3c/dom/html/HTMLPropertiesCollection.h \
	org/w3c/dom/html/HTMLQuoteElement.h \
	org/w3c/dom/html/HTMLScriptElement.h \
	org/w3c/dom/html/HTMLSelectElement.h \
	org/w3c/dom/html/HTMLSourceElement.h \
	org/w3c/dom/html/HTMLSpanElement.h \
	org/w3c/dom/html/HTMLStyleElement.h \
	org/w3c/dom/html/HTMLTableCaptionElement.h \
	org/w3c/dom/html/HTMLTableCellElement.h \
	org/w3c/dom/html/HTMLTableColElement.h \
	org/w3c/dom/html/HTMLTableDataCellElement.h \
	org/w3c/dom/html/HTMLTableElement.h \
	org/w3c/dom/html/HTMLTableHeaderCellElement.h \
	org/w3c/dom/html/HTMLTableRowElement.h \
	org/w3c/dom/html/HTMLTableSectionElement.h \
	org/w3c/dom/html/HTMLTextAreaElement.h \
	org/w3c/dom/html/HTMLTimeElement.h \
	org/w3c/dom/html/HTMLTitleElement.h \
	org/w3c/dom/html/HTMLUListElement.h \
	org/w3c/dom/html/HTMLUnknownElement.h \
	org/w3c/dom/html/HTMLVideoElement.h \
	org/w3c/dom/html/ImageData.h \
	org/w3c/dom/html/Location.h \
	org/w3c/dom/html/MediaError.h \
	org/w3c/dom/html/MediaQueryList.h \
	org/w3c/dom/html/MediaQueryListListener.h \
	org/w3c/dom/html/MessageChannel.h \
	org/w3c/dom/html/MessageEvent.h \
	org/w3c/dom/html/MessagePort.h \
	org/w3c/dom/html/Navigator.h \
	org/w3c/dom/html/PageTransitionEvent.h \
	org/w3c/dom/html/PopStateEvent.h \
	org/w3c/dom/html/PropertyNodeList.h \
	org/w3c/dom/html/RadioNodeList.h \
	org/w3c/dom/html/Screen.h \
	org/w3c/dom/html/TextMetrics.h \
	org/w3c/dom/html/TimeRanges.h \
	org/w3c/dom/html/UndoManagerEvent.h \
	org/w3c/dom/html/UndoManager.h \
	org/w3c/dom/html/ValidityState.h \
	org/w3c/dom/html/Window.h \
	org/w3c/dom/ranges/DocumentRange.h \
	org/w3c/dom/ranges/RangeException.h \
	org/w3c/dom/ranges/Range.h \
	org/w3c/dom/stylesheets/LinkStyle.h \
	org/w3c/dom/stylesheets/MediaList.h \
	org/w3c/dom/stylesheets/StyleSheet.h \
	org/w3c/dom/traversal/DocumentTraversal.h \
	org/w3c/dom/traversal/NodeFilter.h \
	org/w3c/dom/traversal/NodeIterator.h \
	org/w3c/dom/traversal/TreeWalker.h \
	org/w3c/dom/typedarray/ArrayBuffer.h \
	org/w3c/dom/typedarray/ArrayBufferView.h \
	org/w3c/dom/typedarray/DataView.h \
	org/w3c/dom/typedarray/Float32Array.h \
	org/w3c/dom/typedarray/Float64Array.h \
	org/w3c/dom/typedarray/Int16Array.h \
	org/w3c/dom/typedarray/Int32Array.h \
	org/w3c/dom/typedarray/Int8Array.h \
	org/w3c/dom/typedarray/Uint16Array.h \
	org/w3c/dom/typedarray/Uint32Array.h \
	org/w3c/dom/typedarray/Uint8Array.h \
	org/w3c/dom/views/ClientRect.h \
	org/w3c/dom/views/ClientRectList.h

generated_sources = \
	org/w3c/dom/AnonXMLHttpRequest.cpp \
	org/w3c/dom/Attr.cpp \
	org/w3c/dom/CaretPosition.cpp \
	org/w3c/dom/CharacterData.cpp \
	org/w3c/dom/Comment.cpp \
	org/w3c/dom/Document.cpp \
	org/w3c/dom/DocumentFragment.cpp \
	org/w3c/dom/DocumentType.cpp \
	org/w3c/dom/DOMImplementation.cpp \
	org/w3c/dom/DOMSettableTokenList.cpp \
	org/w3c/dom/DOMStringList.cpp \
	org/w3c/dom/DOMStringMap.cpp \
	org/w3c/dom/DOMTokenList.cpp \
	org/w3c/dom/Element.cpp \
	org/w3c/dom/FormData.cpp \
	org/w3c/dom/Node.cpp \
	org/w3c/dom/NodeList.cpp \
	org/w3c/dom/ProcessingInstruction.cpp \
	org/w3c/dom/Text.cpp \
	org/w3c/dom/XMLHttpRequest.cpp \
	org/w3c/dom/XMLHttpRequestEventTarget.cpp \
	org/w3c/dom/XMLHttpRequestUpload.cpp \
	org/w3c/dom/css/Counter.cpp \
	org/w3c/dom/css/CSS2Properties.cpp \
	org/w3c/dom/css/CSSCharsetRule.cpp \
	org/w3c/dom/css/CSSColorComponentValue.cpp \
	org/w3c/dom/css/CSSComponentValue.cpp \
	org/w3c/dom/css/CSSFontFaceRule.cpp \
	org/w3c/dom/css/CSSImportRule.cpp \
	org/w3c/dom/css/CSSLengthComponentValue.cpp \
	org/w3c/dom/css/CSSMapValue.cpp \
	org/w3c/dom/css/CSSMediaRule.cpp \
	org/w3c/dom/css/CSSNamespaceRule.cpp \
	org/w3c/dom/css/CSSPageRule.cpp \
	org/w3c/dom/css/CSSPercentageComponentValue.cpp \
	org/w3c/dom/css/CSSPrimitiveValue.cpp \
	org/w3c/dom/css/CSSPropertyValueList.cpp \
	org/w3c/dom/css/CSSRule.cpp \
	org/w3c/dom/css/CSSStyleDeclaration.cpp \
	org/w3c/dom/css/CSSStyleDeclarationValue.cpp \
	org/w3c/dom/css/CSSStyleRule.cpp \
	org/w3c/dom/css/CSSStyleSheet.cpp \
	org/w3c/dom/css/CSSUnknownRule.cpp \
	org/w3c/dom/css/CSSURLComponentValue.cpp \
	org/w3c/dom/css/CSSValue.cpp \
	org/w3c/dom/css/CSSValueList.cpp \
	org/w3c/dom/css/DocumentCSS.cpp \
	org/w3c/dom/css/DOMImplementationCSS.cpp \
	org/w3c/dom/css/ElementCSSInlineStyle.cpp \
	org/w3c/dom/css/Rect.cpp \
	org/w3c/dom/css/RGBColor.cpp \
	org/w3c/dom/events/CompositionEvent.cpp \
	org/w3c/dom/events/CustomEvent.cpp \
	org/w3c/dom/events/Event.cpp \
	org/w3c/dom/events/EventListener.cpp \
	org/w3c/dom/events/EventTarget.cpp \
	org/w3c/dom/events/FocusEvent.cpp \
	org/w3c/dom/events/KeyboardEvent.cpp \
	org/w3c/dom/events/MouseEvent.cpp \
	org/w3c/dom/events/MutationEvent.cpp \
	org/w3c/dom/events/MutationNameEvent.cpp \
	org/w3c/dom/eventsource/EventSource.cpp \
	org/w3c/dom/events/ProgressEvent.cpp \
	org/w3c/dom/events/TextEvent.cpp \
	org/w3c/dom/events/UIEvent.cpp \
	org/w3c/dom/events/WheelEvent.cpp \
	org/w3c/dom/file/Blob.cpp \
	org/w3c/dom/file/File.cpp \
	org/w3c/dom/file/FileError.cpp \
	org/w3c/dom/file/FileReader.cpp \
	org/w3c/dom/file/FileReaderSync.cpp \
	org/w3c/dom/html/ApplicationCache.cpp \
	org/w3c/dom/html/BarProp.cpp \
	org/w3c/dom/html/BeforeUnloadEvent.cpp \
	org/w3c/dom/html/CanvasGradient.cpp \
	org/w3c/dom/html/CanvasPattern.cpp \
	org/w3c/dom/html/CanvasPixelArray.cpp \
	org/w3c/dom/html/CanvasRenderingContext2D.cpp \
	org/w3c/dom/html/DataTransfer.cpp \
	org/w3c/dom/html/DragEvent.cpp \
	org/w3c/dom/html/Function.cpp \
	org/w3c/dom/html/HashChangeEvent.cpp \
	org/w3c/dom/html/History.cpp \
	org/w3c/dom/html/HTMLAllCollection.cpp \
	org/w3c/dom/html/HTMLAnchorElement.cpp \
	org/w3c/dom/html/HTMLAppletElement.cpp \
	org/w3c/dom/html/HTMLAreaElement.cpp \
	org/w3c/dom/html/HTMLAudioElement.cpp \
	org/w3c/dom/html/HTMLBaseElement.cpp \
	org/w3c/dom/html/HTMLBaseFontElement.cpp \
	org/w3c/dom/html/HTMLBodyElement.cpp \
	org/w3c/dom/html/HTMLBRElement.cpp \
	org/w3c/dom/html/HTMLButtonElement.cpp \
	org/w3c/dom/html/HTMLCanvasElement.cpp \
	org/w3c/dom/html/HTMLCollection.cpp \
	org/w3c/dom/html/HTMLCommandElement.cpp \
	org/w3c/dom/html/HTMLDataListElement.cpp \
	org/w3c/dom/html/HTMLDetailsElement.cpp \
	org/w3c/dom/html/HTMLDirectoryElement.cpp \
	org/w3c/dom/html/HTMLDivElement.cpp \
	org/w3c/dom/html/HTMLDListElement.cpp \
	org/w3c/dom/html/HTMLDocument.cpp \
	org/w3c/dom/html/HTMLElement.cpp \
	org/w3c/dom/html/HTMLEmbedElement.cpp \
	org/w3c/dom/html/HTMLFieldSetElement.cpp \
	org/w3c/dom/html/HTMLFontElement.cpp \
	org/w3c/dom/html/HTMLFormControlsCollection.cpp \
	org/w3c/dom/html/HTMLFormElement.cpp \
	org/w3c/dom/html/HTMLFrameElement.cpp \
	org/w3c/dom/html/HTMLFrameSetElement.cpp \
	org/w3c/dom/html/HTMLHeadElement.cpp \
	org/w3c/dom/html/HTMLHeadingElement.cpp \
	org/w3c/dom/html/HTMLHRElement.cpp \
	org/w3c/dom/html/HTMLHtmlElement.cpp \
	org/w3c/dom/html/HTMLIFrameElement.cpp \
	org/w3c/dom/html/HTMLImageElement.cpp \
	org/w3c/dom/html/HTMLInputElement.cpp \
	org/w3c/dom/html/HTMLKeygenElement.cpp \
	org/w3c/dom/html/HTMLLabelElement.cpp \
	org/w3c/dom/html/HTMLLegendElement.cpp \
	org/w3c/dom/html/HTMLLIElement.cpp \
	org/w3c/dom/html/HTMLLinkElement.cpp \
	org/w3c/dom/html/HTMLMapElement.cpp \
	org/w3c/dom/html/HTMLMarqueeElement.cpp \
	org/w3c/dom/html/HTMLMediaElement.cpp \
	org/w3c/dom/html/HTMLMenuElement.cpp \
	org/w3c/dom/html/HTMLMetaElement.cpp \
	org/w3c/dom/html/HTMLMeterElement.cpp \
	org/w3c/dom/html/HTMLModElement.cpp \
	org/w3c/dom/html/HTMLObjectElement.cpp \
	org/w3c/dom/html/HTMLOListElement.cpp \
	org/w3c/dom/html/HTMLOptGroupElement.cpp \
	org/w3c/dom/html/HTMLOptionElement.cpp \
	org/w3c/dom/html/HTMLOptionsCollection.cpp \
	org/w3c/dom/html/HTMLOutputElement.cpp \
	org/w3c/dom/html/HTMLParagraphElement.cpp \
	org/w3c/dom/html/HTMLParamElement.cpp \
	org/w3c/dom/html/HTMLPreElement.cpp \
	org/w3c/dom/html/HTMLProgressElement.cpp \
	org/w3c/dom/html/HTMLPropertiesCollection.cpp \
	org/w3c/dom/html/HTMLQuoteElement.cpp \
	org/w3c/dom/html/HTMLScriptElement.cpp \
	org/w3c/dom/html/HTMLSelectElement.cpp \
	org/w3c/dom/html/HTMLSourceElement.cpp \
	org/w3c/dom/html/HTMLSpanElement.cpp \
	org/w3c/dom/html/HTMLStyleElement.cpp \
	org/w3c/dom/html/HTMLTableCaptionElement.cpp \
	org/w3c/dom/html/HTMLTableCellElement.cpp \
	org/w3c/dom/html/HTMLTableColElement.cpp \
	org/w3c/dom/html/HTMLTableDataCellElement.cpp \
	org/w3c/dom/html/HTMLTableElement.cpp \
	org/w3c/dom/html/HTMLTableHeaderCellElement.cpp \
	org/w3c/dom/html/HTMLTableRowElement.cpp \
	org/w3c/dom/html/HTMLTableSectionElement.cpp \
	org/w3c/dom/html/HTMLTextAreaElement.cpp \
	org/w3c/dom/html/HTMLTimeElement.cpp \
	org/w3c/dom/html/HTMLTitleElement.cpp \
	org/w3c/dom/html/HTMLUListElement.cpp \
	org/w3c/dom/html/HTMLUnknownElement.cpp \
	org/w3c/dom/html/HTMLVideoElement.cpp \
	org/w3c/dom/html/ImageData.cpp \
	org/w3c/dom/html/Location.cpp \
	org/w3c/dom/html/MediaError.cpp \
	org/w3c/dom/html/MediaQueryList.cpp \
	org/w3c/dom/html/MediaQueryListListener.cpp \
	org/w3c/dom/html/MessageChannel.cpp \
	org/w3c/dom/html/MessageEvent.cpp \
	org/w3c/dom/html/MessagePort.cpp \
	org/w3c/dom/html/Navigator.cpp \
	org/w3c/dom/html/PageTransitionEvent.cpp \
	org/w3c/dom/html/PopStateEvent.cpp \
	org/w3c/dom/html/PropertyNodeList.cpp \
	org/w3c/dom/html/RadioNodeList.cpp \
	org/w3c/dom/html/Screen.cpp \
	org/w3c/dom/html/TextMetrics.cpp \
	org/w3c/dom/html/TimeRanges.cpp \
	org/w3c/dom/html/UndoManager.cpp \
	org/w3c/dom/html/UndoManagerEvent.cpp \
	org/w3c/dom/html/ValidityState.cpp \
	org/w3c/dom/html/Window.cpp \
	org/w3c/dom/ranges/DocumentRange.cpp \
	org/w3c/dom/ranges/Range.cpp \
	org/w3c/dom/stylesheets/LinkStyle.cpp \
	org/w3c/dom/stylesheets/MediaList.cpp \
	org/w3c/dom/stylesheets/StyleSheet.cpp \
	org/w3c/dom/traversal/DocumentTraversal.cpp \
	org/w3c/dom/traversal/NodeFilter.cpp \
	org/w3c/dom/traversal/NodeIterator.cpp \
	org/w3c/dom/traversal/TreeWalker.cpp \
	org/w3c/dom/typedarray/ArrayBuffer.cpp \
	org/w3c/dom/typedarray/ArrayBufferView.cpp \
	org/w3c/dom/typedarray/DataView.cpp \
	org/w3c/dom/typedarray/Float32Array.cpp \
	org/w3c/dom/typedarray/Float64Array.cpp \
	org/w3c/dom/typedarray/Int16Array.cpp \
	org/w3c/dom/typedarray/Int32Array.cpp \
	org/w3c/dom/typedarray/Int8Array.cpp \
	org/w3c/dom/typedarray/Uint16Array.cpp \
	org/w3c/dom/typedarray/Uint32Array.cpp \
	org/w3c/dom/typedarray/Uint8Array.cpp \
	org/w3c/dom/views/ClientRect.cpp \
	org/w3c/dom/views/ClientRectList.cpp

BUILT_SOURCES = $(generated_sources) $(generated_headers)

nobase_nodist_include_HEADERS = $(generated_headers)

$(generated_headers) : org/w3c/dom/Node.h

org/w3c/dom/Node.h : $(idl_files)
	$(esidl) -messenger -string std::string -object Object -fno-multipleinheritance $^

lib_LIBRARIES = libesnpapi.a

AM_CPPFLAGS = \
	-std=gnu++0x -Wall \
	-I include \
	-I $(srcdir)/include \
	-I $(srcdir)/../include \
	-I $(srcdir)/../cplusplus \
	-DXP_UNIX -DHAVE_CONFIG_H

if !NACL
AM_CPPFLAGS += -fPIC
endif

org/w3c/dom/Node.cpp : $(idl_files) 
	$(esidl) -messenger-src -string std::string -object Object -fno-multipleinheritance $^

$(generated_sources) : org/w3c/dom/Node.cpp

nodist_libesnpapi_a_SOURCES = $(generated_sources) $(generated_headers)

libesnpapi_a_SOURCES = \
	include/esnpapi.h \
	include/npapi/npapi.h \
	include/npapi/npruntime.h \
	include/npapi/nptypes.h \
	include/npapi/npupp.h \
	src/Any.cpp \
	src/proxy.cpp \
	src/invoke.cpp \
	src/stub.cpp \
	src/bridge.cpp \
	src/constructors.cpp

clean-local:
	rm -rf org com *.h

EXTRA_DIST = config.h
