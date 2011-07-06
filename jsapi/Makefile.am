SUFFIXES = .idl

vpath %.idl = $(srcdir)/../dom

esidl=../esidl

idl_files = \
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
	org/w3c/dom/DOMElementMap.h \
	org/w3c/dom/DOMException.h \
	org/w3c/dom/DOMImplementation.h \
	org/w3c/dom/DOMSettableTokenList.h \
	org/w3c/dom/DOMStringList.h \
	org/w3c/dom/DOMStringMap.h \
	org/w3c/dom/DOMTokenList.h \
	org/w3c/dom/Document.h \
	org/w3c/dom/DocumentFragment.h \
	org/w3c/dom/DocumentType.h \
	org/w3c/dom/Element.h \
	org/w3c/dom/FormData.h \
	org/w3c/dom/Node.h \
	org/w3c/dom/NodeList.h \
	org/w3c/dom/ProcessingInstruction.h \
	org/w3c/dom/Text.h \
	org/w3c/dom/XMLHttpRequest.h \
	org/w3c/dom/XMLHttpRequestEventTarget.h \
	org/w3c/dom/XMLHttpRequestUpload.h \
	org/w3c/dom/css/CSS2Properties.h \
	org/w3c/dom/css/CSSCharsetRule.h \
	org/w3c/dom/css/CSSColorComponentValue.h \
	org/w3c/dom/css/CSSComponentValue.h \
	org/w3c/dom/css/CSSFontFaceRule.h \
	org/w3c/dom/css/CSSIdentifierComponentValue.h \
	org/w3c/dom/css/CSSImportRule.h \
	org/w3c/dom/css/CSSKeywordComponentValue.h \
	org/w3c/dom/css/CSSLengthComponentValue.h \
	org/w3c/dom/css/CSSMapValue.h \
	org/w3c/dom/css/CSSMediaRule.h \
	org/w3c/dom/css/CSSNamespaceRule.h \
	org/w3c/dom/css/CSSPageRule.h \
	org/w3c/dom/css/CSSPercentageComponentValue.h \
	org/w3c/dom/css/CSSPrimitiveValue.h \
	org/w3c/dom/css/CSSPropertyValue.h \
	org/w3c/dom/css/CSSPropertyValueList.h \
	org/w3c/dom/css/CSSRule.h \
	org/w3c/dom/css/CSSStringComponentValue.h \
	org/w3c/dom/css/CSSStyleDeclaration.h \
	org/w3c/dom/css/CSSStyleDeclarationValue.h \
	org/w3c/dom/css/CSSStyleRule.h \
	org/w3c/dom/css/CSSStyleSheet.h \
	org/w3c/dom/css/CSSURLComponentValue.h \
	org/w3c/dom/css/CSSUnknownRule.h \
	org/w3c/dom/css/CSSValue.h \
	org/w3c/dom/css/CSSValueList.h \
	org/w3c/dom/css/Counter.h \
	org/w3c/dom/css/DOMImplementationCSS.h \
	org/w3c/dom/css/DocumentCSS.h \
	org/w3c/dom/css/ElementCSSInlineStyle.h \
	org/w3c/dom/css/RGBColor.h \
	org/w3c/dom/css/Rect.h \
	org/w3c/dom/events/CompositionEvent.h \
	org/w3c/dom/events/CustomEvent.h \
	org/w3c/dom/events/CustomEventInit.h \
	org/w3c/dom/events/Event.h \
	org/w3c/dom/events/EventException.h \
	org/w3c/dom/events/EventInit.h \
	org/w3c/dom/events/EventListener.h \
	org/w3c/dom/events/EventTarget.h \
	org/w3c/dom/events/FocusEvent.h \
	org/w3c/dom/events/KeyboardEvent.h \
	org/w3c/dom/events/MouseEvent.h \
	org/w3c/dom/events/MutationEvent.h \
	org/w3c/dom/events/MutationNameEvent.h \
	org/w3c/dom/events/ProgressEvent.h \
	org/w3c/dom/events/TextEvent.h \
	org/w3c/dom/events/UIEvent.h \
	org/w3c/dom/events/WheelEvent.h \
	org/w3c/dom/eventsource/EventSource.h \
	org/w3c/dom/file/Blob.h \
	org/w3c/dom/file/File.h \
	org/w3c/dom/file/FileCallback.h \
	org/w3c/dom/file/FileError.h \
	org/w3c/dom/file/FileException.h \
	org/w3c/dom/file/FileList.h \
	org/w3c/dom/file/FileReader.h \
	org/w3c/dom/file/FileReaderSync.h \
	org/w3c/dom/html/ApplicationCache.h \
	org/w3c/dom/html/AudioTrack.h \
	org/w3c/dom/html/AudioTrackList.h \
	org/w3c/dom/html/BarProp.h \
	org/w3c/dom/html/BeforeUnloadEvent.h \
	org/w3c/dom/html/BlobCallback.h \
	org/w3c/dom/html/BooleanCallback.h \
	org/w3c/dom/html/CanvasGradient.h \
	org/w3c/dom/html/CanvasPattern.h \
	org/w3c/dom/html/CanvasPixelArray.h \
	org/w3c/dom/html/CanvasRenderingContext2D.h \
	org/w3c/dom/html/DataTransfer.h \
	org/w3c/dom/html/DataTransferItem.h \
	org/w3c/dom/html/DataTransferItemList.h \
	org/w3c/dom/html/DragEvent.h \
	org/w3c/dom/html/External.h \
	org/w3c/dom/html/Function.h \
	org/w3c/dom/html/FunctionStringCallback.h \
	org/w3c/dom/html/HTMLAllCollection.h \
	org/w3c/dom/html/HTMLAnchorElement.h \
	org/w3c/dom/html/HTMLAppletElement.h \
	org/w3c/dom/html/HTMLAreaElement.h \
	org/w3c/dom/html/HTMLAudioElement.h \
	org/w3c/dom/html/HTMLBRElement.h \
	org/w3c/dom/html/HTMLBaseElement.h \
	org/w3c/dom/html/HTMLBaseFontElement.h \
	org/w3c/dom/html/HTMLBodyElement.h \
	org/w3c/dom/html/HTMLButtonElement.h \
	org/w3c/dom/html/HTMLCanvasElement.h \
	org/w3c/dom/html/HTMLCollection.h \
	org/w3c/dom/html/HTMLCommandElement.h \
	org/w3c/dom/html/HTMLDListElement.h \
	org/w3c/dom/html/HTMLDataListElement.h \
	org/w3c/dom/html/HTMLDetailsElement.h \
	org/w3c/dom/html/HTMLDirectoryElement.h \
	org/w3c/dom/html/HTMLDivElement.h \
	org/w3c/dom/html/HTMLDocument.h \
	org/w3c/dom/html/HTMLElement.h \
	org/w3c/dom/html/HTMLEmbedElement.h \
	org/w3c/dom/html/HTMLFieldSetElement.h \
	org/w3c/dom/html/HTMLFontElement.h \
	org/w3c/dom/html/HTMLFormControlsCollection.h \
	org/w3c/dom/html/HTMLFormElement.h \
	org/w3c/dom/html/HTMLFrameElement.h \
	org/w3c/dom/html/HTMLFrameSetElement.h \
	org/w3c/dom/html/HTMLHRElement.h \
	org/w3c/dom/html/HTMLHeadElement.h \
	org/w3c/dom/html/HTMLHeadingElement.h \
	org/w3c/dom/html/HTMLHtmlElement.h \
	org/w3c/dom/html/HTMLIFrameElement.h \
	org/w3c/dom/html/HTMLImageElement.h \
	org/w3c/dom/html/HTMLInputElement.h \
	org/w3c/dom/html/HTMLKeygenElement.h \
	org/w3c/dom/html/HTMLLIElement.h \
	org/w3c/dom/html/HTMLLabelElement.h \
	org/w3c/dom/html/HTMLLegendElement.h \
	org/w3c/dom/html/HTMLLinkElement.h \
	org/w3c/dom/html/HTMLMapElement.h \
	org/w3c/dom/html/HTMLMarqueeElement.h \
	org/w3c/dom/html/HTMLMediaElement.h \
	org/w3c/dom/html/HTMLMenuElement.h \
	org/w3c/dom/html/HTMLMetaElement.h \
	org/w3c/dom/html/HTMLMeterElement.h \
	org/w3c/dom/html/HTMLModElement.h \
	org/w3c/dom/html/HTMLOListElement.h \
	org/w3c/dom/html/HTMLObjectElement.h \
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
	org/w3c/dom/html/HTMLTrackElement.h \
	org/w3c/dom/html/HTMLUListElement.h \
	org/w3c/dom/html/HTMLUnknownElement.h \
	org/w3c/dom/html/HTMLVideoElement.h \
	org/w3c/dom/html/HashChangeEvent.h \
	org/w3c/dom/html/History.h \
	org/w3c/dom/html/ImageData.h \
	org/w3c/dom/html/LocalMediaStream.h \
	org/w3c/dom/html/Location.h \
	org/w3c/dom/html/MediaController.h \
	org/w3c/dom/html/MediaError.h \
	org/w3c/dom/html/MediaStream.h \
	org/w3c/dom/html/MediaStreamRecorder.h \
	org/w3c/dom/html/MessageChannel.h \
	org/w3c/dom/html/MessageEvent.h \
	org/w3c/dom/html/MessagePort.h \
	org/w3c/dom/html/MutableTextTrack.h \
	org/w3c/dom/html/Navigator.h \
	org/w3c/dom/html/NavigatorUserMediaError.h \
	org/w3c/dom/html/NavigatorUserMediaErrorCallback.h \
	org/w3c/dom/html/NavigatorUserMediaSuccessCallback.h \
	org/w3c/dom/html/PageTransitionEvent.h \
	org/w3c/dom/html/PeerConnection.h \
	org/w3c/dom/html/PopStateEvent.h \
	org/w3c/dom/html/PropertyNodeList.h \
	org/w3c/dom/html/RadioNodeList.h \
	org/w3c/dom/html/Screen.h \
	org/w3c/dom/html/SignalingCallback.h \
	org/w3c/dom/html/StreamEvent.h \
	org/w3c/dom/html/StreamTrack.h \
	org/w3c/dom/html/StyleMedia.h \
	org/w3c/dom/html/TextMetrics.h \
	org/w3c/dom/html/TextTrack.h \
	org/w3c/dom/html/TextTrackCue.h \
	org/w3c/dom/html/TextTrackCueList.h \
	org/w3c/dom/html/TimeRanges.h \
	org/w3c/dom/html/Transferable.h \
	org/w3c/dom/html/UndoManager.h \
	org/w3c/dom/html/UndoManagerEvent.h \
	org/w3c/dom/html/ValidityState.h \
	org/w3c/dom/html/VideoTrack.h \
	org/w3c/dom/html/VideoTrackList.h \
	org/w3c/dom/html/Window.h \
	org/w3c/dom/html/WindowModal.h \
	org/w3c/dom/ranges/DocumentRange.h \
	org/w3c/dom/ranges/Range.h \
	org/w3c/dom/ranges/RangeException.h \
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
	org/w3c/dom/DOMElementMap.cpp \
	org/w3c/dom/DOMImplementation.cpp \
	org/w3c/dom/DOMSettableTokenList.cpp \
	org/w3c/dom/DOMStringList.cpp \
	org/w3c/dom/DOMStringMap.cpp \
	org/w3c/dom/DOMTokenList.cpp \
	org/w3c/dom/Document.cpp \
	org/w3c/dom/DocumentFragment.cpp \
	org/w3c/dom/DocumentType.cpp \
	org/w3c/dom/Element.cpp \
	org/w3c/dom/FormData.cpp \
	org/w3c/dom/Node.cpp \
	org/w3c/dom/NodeList.cpp \
	org/w3c/dom/ProcessingInstruction.cpp \
	org/w3c/dom/Text.cpp \
	org/w3c/dom/XMLHttpRequest.cpp \
	org/w3c/dom/XMLHttpRequestEventTarget.cpp \
	org/w3c/dom/XMLHttpRequestUpload.cpp \
	org/w3c/dom/css/CSS2Properties.cpp \
	org/w3c/dom/css/CSSCharsetRule.cpp \
	org/w3c/dom/css/CSSColorComponentValue.cpp \
	org/w3c/dom/css/CSSComponentValue.cpp \
	org/w3c/dom/css/CSSFontFaceRule.cpp \
	org/w3c/dom/css/CSSIdentifierComponentValue.cpp \
	org/w3c/dom/css/CSSImportRule.cpp \
	org/w3c/dom/css/CSSKeywordComponentValue.cpp \
	org/w3c/dom/css/CSSLengthComponentValue.cpp \
	org/w3c/dom/css/CSSMapValue.cpp \
	org/w3c/dom/css/CSSMediaRule.cpp \
	org/w3c/dom/css/CSSNamespaceRule.cpp \
	org/w3c/dom/css/CSSPageRule.cpp \
	org/w3c/dom/css/CSSPercentageComponentValue.cpp \
	org/w3c/dom/css/CSSPrimitiveValue.cpp \
	org/w3c/dom/css/CSSPropertyValue.cpp \
	org/w3c/dom/css/CSSPropertyValueList.cpp \
	org/w3c/dom/css/CSSRule.cpp \
	org/w3c/dom/css/CSSStringComponentValue.cpp \
	org/w3c/dom/css/CSSStyleDeclaration.cpp \
	org/w3c/dom/css/CSSStyleDeclarationValue.cpp \
	org/w3c/dom/css/CSSStyleRule.cpp \
	org/w3c/dom/css/CSSStyleSheet.cpp \
	org/w3c/dom/css/CSSURLComponentValue.cpp \
	org/w3c/dom/css/CSSUnknownRule.cpp \
	org/w3c/dom/css/CSSValue.cpp \
	org/w3c/dom/css/CSSValueList.cpp \
	org/w3c/dom/css/Counter.cpp \
	org/w3c/dom/css/DOMImplementationCSS.cpp \
	org/w3c/dom/css/DocumentCSS.cpp \
	org/w3c/dom/css/ElementCSSInlineStyle.cpp \
	org/w3c/dom/css/RGBColor.cpp \
	org/w3c/dom/css/Rect.cpp \
	org/w3c/dom/events/CompositionEvent.cpp \
	org/w3c/dom/events/CustomEvent.cpp \
	org/w3c/dom/events/CustomEventInit.cpp \
	org/w3c/dom/events/Event.cpp \
	org/w3c/dom/events/EventInit.cpp \
	org/w3c/dom/events/EventListener.cpp \
	org/w3c/dom/events/EventTarget.cpp \
	org/w3c/dom/events/FocusEvent.cpp \
	org/w3c/dom/events/KeyboardEvent.cpp \
	org/w3c/dom/events/MouseEvent.cpp \
	org/w3c/dom/events/MutationEvent.cpp \
	org/w3c/dom/events/MutationNameEvent.cpp \
	org/w3c/dom/events/ProgressEvent.cpp \
	org/w3c/dom/events/TextEvent.cpp \
	org/w3c/dom/events/UIEvent.cpp \
	org/w3c/dom/events/WheelEvent.cpp \
	org/w3c/dom/eventsource/EventSource.cpp \
	org/w3c/dom/file/Blob.cpp \
	org/w3c/dom/file/File.cpp \
	org/w3c/dom/file/FileCallback.cpp \
	org/w3c/dom/file/FileError.cpp \
	org/w3c/dom/file/FileList.cpp \
	org/w3c/dom/file/FileReader.cpp \
	org/w3c/dom/file/FileReaderSync.cpp \
	org/w3c/dom/html/ApplicationCache.cpp \
	org/w3c/dom/html/AudioTrack.cpp \
	org/w3c/dom/html/AudioTrackList.cpp \
	org/w3c/dom/html/BarProp.cpp \
	org/w3c/dom/html/BeforeUnloadEvent.cpp \
	org/w3c/dom/html/BlobCallback.cpp \
	org/w3c/dom/html/BooleanCallback.cpp \
	org/w3c/dom/html/CanvasGradient.cpp \
	org/w3c/dom/html/CanvasPattern.cpp \
	org/w3c/dom/html/CanvasPixelArray.cpp \
	org/w3c/dom/html/CanvasRenderingContext2D.cpp \
	org/w3c/dom/html/DataTransfer.cpp \
	org/w3c/dom/html/DataTransferItem.cpp \
	org/w3c/dom/html/DataTransferItemList.cpp \
	org/w3c/dom/html/DragEvent.cpp \
	org/w3c/dom/html/External.cpp \
	org/w3c/dom/html/Function.cpp \
	org/w3c/dom/html/FunctionStringCallback.cpp \
	org/w3c/dom/html/HTMLAllCollection.cpp \
	org/w3c/dom/html/HTMLAnchorElement.cpp \
	org/w3c/dom/html/HTMLAppletElement.cpp \
	org/w3c/dom/html/HTMLAreaElement.cpp \
	org/w3c/dom/html/HTMLAudioElement.cpp \
	org/w3c/dom/html/HTMLBRElement.cpp \
	org/w3c/dom/html/HTMLBaseElement.cpp \
	org/w3c/dom/html/HTMLBaseFontElement.cpp \
	org/w3c/dom/html/HTMLBodyElement.cpp \
	org/w3c/dom/html/HTMLButtonElement.cpp \
	org/w3c/dom/html/HTMLCanvasElement.cpp \
	org/w3c/dom/html/HTMLCollection.cpp \
	org/w3c/dom/html/HTMLCommandElement.cpp \
	org/w3c/dom/html/HTMLDListElement.cpp \
	org/w3c/dom/html/HTMLDataListElement.cpp \
	org/w3c/dom/html/HTMLDetailsElement.cpp \
	org/w3c/dom/html/HTMLDirectoryElement.cpp \
	org/w3c/dom/html/HTMLDivElement.cpp \
	org/w3c/dom/html/HTMLDocument.cpp \
	org/w3c/dom/html/HTMLElement.cpp \
	org/w3c/dom/html/HTMLEmbedElement.cpp \
	org/w3c/dom/html/HTMLFieldSetElement.cpp \
	org/w3c/dom/html/HTMLFontElement.cpp \
	org/w3c/dom/html/HTMLFormControlsCollection.cpp \
	org/w3c/dom/html/HTMLFormElement.cpp \
	org/w3c/dom/html/HTMLFrameElement.cpp \
	org/w3c/dom/html/HTMLFrameSetElement.cpp \
	org/w3c/dom/html/HTMLHRElement.cpp \
	org/w3c/dom/html/HTMLHeadElement.cpp \
	org/w3c/dom/html/HTMLHeadingElement.cpp \
	org/w3c/dom/html/HTMLHtmlElement.cpp \
	org/w3c/dom/html/HTMLIFrameElement.cpp \
	org/w3c/dom/html/HTMLImageElement.cpp \
	org/w3c/dom/html/HTMLInputElement.cpp \
	org/w3c/dom/html/HTMLKeygenElement.cpp \
	org/w3c/dom/html/HTMLLIElement.cpp \
	org/w3c/dom/html/HTMLLabelElement.cpp \
	org/w3c/dom/html/HTMLLegendElement.cpp \
	org/w3c/dom/html/HTMLLinkElement.cpp \
	org/w3c/dom/html/HTMLMapElement.cpp \
	org/w3c/dom/html/HTMLMarqueeElement.cpp \
	org/w3c/dom/html/HTMLMediaElement.cpp \
	org/w3c/dom/html/HTMLMenuElement.cpp \
	org/w3c/dom/html/HTMLMetaElement.cpp \
	org/w3c/dom/html/HTMLMeterElement.cpp \
	org/w3c/dom/html/HTMLModElement.cpp \
	org/w3c/dom/html/HTMLOListElement.cpp \
	org/w3c/dom/html/HTMLObjectElement.cpp \
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
	org/w3c/dom/html/HTMLTrackElement.cpp \
	org/w3c/dom/html/HTMLUListElement.cpp \
	org/w3c/dom/html/HTMLUnknownElement.cpp \
	org/w3c/dom/html/HTMLVideoElement.cpp \
	org/w3c/dom/html/HashChangeEvent.cpp \
	org/w3c/dom/html/History.cpp \
	org/w3c/dom/html/ImageData.cpp \
	org/w3c/dom/html/Location.cpp \
	org/w3c/dom/html/MediaController.cpp \
	org/w3c/dom/html/MediaError.cpp \
	org/w3c/dom/html/MediaStreamRecorder.cpp \
	org/w3c/dom/html/MessageChannel.cpp \
	org/w3c/dom/html/MessageEvent.cpp \
	org/w3c/dom/html/MessagePort.cpp \
	org/w3c/dom/html/MutableTextTrack.cpp \
	org/w3c/dom/html/Navigator.cpp \
	org/w3c/dom/html/NavigatorUserMediaError.cpp \
	org/w3c/dom/html/NavigatorUserMediaErrorCallback.cpp \
	org/w3c/dom/html/PageTransitionEvent.cpp \
	org/w3c/dom/html/PopStateEvent.cpp \
	org/w3c/dom/html/PropertyNodeList.cpp \
	org/w3c/dom/html/RadioNodeList.cpp \
	org/w3c/dom/html/Screen.cpp \
	org/w3c/dom/html/SignalingCallback.cpp \
	org/w3c/dom/html/StreamTrack.cpp \
	org/w3c/dom/html/StyleMedia.cpp \
	org/w3c/dom/html/TextMetrics.cpp \
	org/w3c/dom/html/TextTrack.cpp \
	org/w3c/dom/html/TextTrackCue.cpp \
	org/w3c/dom/html/TextTrackCueList.cpp \
	org/w3c/dom/html/TimeRanges.cpp \
	org/w3c/dom/html/Transferable.cpp \
	org/w3c/dom/html/UndoManager.cpp \
	org/w3c/dom/html/UndoManagerEvent.cpp \
	org/w3c/dom/html/ValidityState.cpp \
	org/w3c/dom/html/VideoTrack.cpp \
	org/w3c/dom/html/VideoTrackList.cpp \
	org/w3c/dom/html/Window.cpp \
	org/w3c/dom/html/WindowModal.cpp \
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

# Interfaces with compile time problems
#	org/w3c/dom/html/LocalMediaStream.cpp
#	org/w3c/dom/html/MediaStream.cpp
#	org/w3c/dom/html/NavigatorUserMediaSuccessCallback.cpp
#	org/w3c/dom/html/PeerConnection.cpp
#	org/w3c/dom/html/StreamEvent.cpp

BUILT_SOURCES = $(generated_sources) $(generated_headers)

nobase_nodist_include_HEADERS = $(generated_headers)

$(generated_headers) : org/w3c/dom/Node.h

org/w3c/dom/Node.h : $(idl_files)
	$(esidl) -messenger -string std::u16string -object Object -fno-multipleinheritance $^

lib_LIBRARIES = libesjsapi.a

LDADD = libesjsapi.a

AM_CPPFLAGS = \
	-std=gnu++0x -Wall \
	-I $(srcdir)/include \
	-I $(srcdir)/../cplusplus \
	-DXP_UNIX -DHAVE_CONFIG_H

AM_LDFLAGS = \
	-lpthread

org/w3c/dom/Node.cpp : $(idl_files) 
	$(esidl) -messenger-src -string std::u16string -object Object -fno-multipleinheritance $^

$(generated_sources) : org/w3c/dom/Node.cpp

nodist_libesjsapi_a_SOURCES = $(generated_sources) $(generated_headers)

libesjsapi_a_SOURCES = \
	include/esjsapi.h \
	include/object.h \
	src/bridge.cpp \
	src/Any.cpp

generated_imp_headers = \
	org/w3c/dom/bootstrap/AnonXMLHttpRequestImp.h \
	org/w3c/dom/bootstrap/ApplicationCacheImp.h \
	org/w3c/dom/bootstrap/ArrayBufferImp.h \
	org/w3c/dom/bootstrap/ArrayBufferViewImp.h \
	org/w3c/dom/bootstrap/AttrImp.h \
	org/w3c/dom/bootstrap/AudioTrackImp.h \
	org/w3c/dom/bootstrap/AudioTrackListImp.h \
	org/w3c/dom/bootstrap/BarPropImp.h \
	org/w3c/dom/bootstrap/BeforeUnloadEventImp.h \
	org/w3c/dom/bootstrap/BlobCallbackImp.h \
	org/w3c/dom/bootstrap/BlobImp.h \
	org/w3c/dom/bootstrap/BooleanCallbackImp.h \
	org/w3c/dom/bootstrap/CSS2PropertiesImp.h \
	org/w3c/dom/bootstrap/CSSCharsetRuleImp.h \
	org/w3c/dom/bootstrap/CSSColorComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSFontFaceRuleImp.h \
	org/w3c/dom/bootstrap/CSSIdentifierComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSImportRuleImp.h \
	org/w3c/dom/bootstrap/CSSKeywordComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSLengthComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSMapValueImp.h \
	org/w3c/dom/bootstrap/CSSMediaRuleImp.h \
	org/w3c/dom/bootstrap/CSSNamespaceRuleImp.h \
	org/w3c/dom/bootstrap/CSSPageRuleImp.h \
	org/w3c/dom/bootstrap/CSSPercentageComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSPrimitiveValueImp.h \
	org/w3c/dom/bootstrap/CSSPropertyValueImp.h \
	org/w3c/dom/bootstrap/CSSPropertyValueListImp.h \
	org/w3c/dom/bootstrap/CSSRuleImp.h \
	org/w3c/dom/bootstrap/CSSStringComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSStyleDeclarationImp.h \
	org/w3c/dom/bootstrap/CSSStyleDeclarationValueImp.h \
	org/w3c/dom/bootstrap/CSSStyleRuleImp.h \
	org/w3c/dom/bootstrap/CSSStyleSheetImp.h \
	org/w3c/dom/bootstrap/CSSURLComponentValueImp.h \
	org/w3c/dom/bootstrap/CSSUnknownRuleImp.h \
	org/w3c/dom/bootstrap/CSSValueImp.h \
	org/w3c/dom/bootstrap/CSSValueListImp.h \
	org/w3c/dom/bootstrap/CanvasGradientImp.h \
	org/w3c/dom/bootstrap/CanvasPatternImp.h \
	org/w3c/dom/bootstrap/CanvasPixelArrayImp.h \
	org/w3c/dom/bootstrap/CanvasRenderingContext2DImp.h \
	org/w3c/dom/bootstrap/CaretPositionImp.h \
	org/w3c/dom/bootstrap/CharacterDataImp.h \
	org/w3c/dom/bootstrap/ClientRectImp.h \
	org/w3c/dom/bootstrap/ClientRectListImp.h \
	org/w3c/dom/bootstrap/CommentImp.h \
	org/w3c/dom/bootstrap/CompositionEventImp.h \
	org/w3c/dom/bootstrap/CounterImp.h \
	org/w3c/dom/bootstrap/CustomEventImp.h \
	org/w3c/dom/bootstrap/CustomEventInitImp.h \
	org/w3c/dom/bootstrap/DOMElementMapImp.h \
	org/w3c/dom/bootstrap/DOMImplementationCSSImp.h \
	org/w3c/dom/bootstrap/DOMImplementationImp.h \
	org/w3c/dom/bootstrap/DOMSettableTokenListImp.h \
	org/w3c/dom/bootstrap/DOMStringListImp.h \
	org/w3c/dom/bootstrap/DOMStringMapImp.h \
	org/w3c/dom/bootstrap/DOMTokenListImp.h \
	org/w3c/dom/bootstrap/DataTransferImp.h \
	org/w3c/dom/bootstrap/DataTransferItemImp.h \
	org/w3c/dom/bootstrap/DataTransferItemListImp.h \
	org/w3c/dom/bootstrap/DataViewImp.h \
	org/w3c/dom/bootstrap/DocumentCSSImp.h \
	org/w3c/dom/bootstrap/DocumentFragmentImp.h \
	org/w3c/dom/bootstrap/DocumentImp.h \
	org/w3c/dom/bootstrap/DocumentRangeImp.h \
	org/w3c/dom/bootstrap/DocumentTraversalImp.h \
	org/w3c/dom/bootstrap/DocumentTypeImp.h \
	org/w3c/dom/bootstrap/DragEventImp.h \
	org/w3c/dom/bootstrap/ElementCSSInlineStyleImp.h \
	org/w3c/dom/bootstrap/ElementImp.h \
	org/w3c/dom/bootstrap/EventImp.h \
	org/w3c/dom/bootstrap/EventInitImp.h \
	org/w3c/dom/bootstrap/EventListenerImp.h \
	org/w3c/dom/bootstrap/EventSourceImp.h \
	org/w3c/dom/bootstrap/EventTargetImp.h \
	org/w3c/dom/bootstrap/ExternalImp.h \
	org/w3c/dom/bootstrap/FileCallbackImp.h \
	org/w3c/dom/bootstrap/FileErrorImp.h \
	org/w3c/dom/bootstrap/FileImp.h \
	org/w3c/dom/bootstrap/FileListImp.h \
	org/w3c/dom/bootstrap/FileReaderImp.h \
	org/w3c/dom/bootstrap/FileReaderSyncImp.h \
	org/w3c/dom/bootstrap/Float32ArrayImp.h \
	org/w3c/dom/bootstrap/Float64ArrayImp.h \
	org/w3c/dom/bootstrap/FocusEventImp.h \
	org/w3c/dom/bootstrap/FormDataImp.h \
	org/w3c/dom/bootstrap/FunctionImp.h \
	org/w3c/dom/bootstrap/FunctionStringCallbackImp.h \
	org/w3c/dom/bootstrap/HTMLAllCollectionImp.h \
	org/w3c/dom/bootstrap/HTMLAnchorElementImp.h \
	org/w3c/dom/bootstrap/HTMLAppletElementImp.h \
	org/w3c/dom/bootstrap/HTMLAreaElementImp.h \
	org/w3c/dom/bootstrap/HTMLAudioElementImp.h \
	org/w3c/dom/bootstrap/HTMLBRElementImp.h \
	org/w3c/dom/bootstrap/HTMLBaseElementImp.h \
	org/w3c/dom/bootstrap/HTMLBaseFontElementImp.h \
	org/w3c/dom/bootstrap/HTMLBodyElementImp.h \
	org/w3c/dom/bootstrap/HTMLButtonElementImp.h \
	org/w3c/dom/bootstrap/HTMLCanvasElementImp.h \
	org/w3c/dom/bootstrap/HTMLCollectionImp.h \
	org/w3c/dom/bootstrap/HTMLCommandElementImp.h \
	org/w3c/dom/bootstrap/HTMLDListElementImp.h \
	org/w3c/dom/bootstrap/HTMLDataListElementImp.h \
	org/w3c/dom/bootstrap/HTMLDetailsElementImp.h \
	org/w3c/dom/bootstrap/HTMLDirectoryElementImp.h \
	org/w3c/dom/bootstrap/HTMLDivElementImp.h \
	org/w3c/dom/bootstrap/HTMLDocumentImp.h \
	org/w3c/dom/bootstrap/HTMLElementImp.h \
	org/w3c/dom/bootstrap/HTMLEmbedElementImp.h \
	org/w3c/dom/bootstrap/HTMLFieldSetElementImp.h \
	org/w3c/dom/bootstrap/HTMLFontElementImp.h \
	org/w3c/dom/bootstrap/HTMLFormControlsCollectionImp.h \
	org/w3c/dom/bootstrap/HTMLFormElementImp.h \
	org/w3c/dom/bootstrap/HTMLFrameElementImp.h \
	org/w3c/dom/bootstrap/HTMLFrameSetElementImp.h \
	org/w3c/dom/bootstrap/HTMLHRElementImp.h \
	org/w3c/dom/bootstrap/HTMLHeadElementImp.h \
	org/w3c/dom/bootstrap/HTMLHeadingElementImp.h \
	org/w3c/dom/bootstrap/HTMLHtmlElementImp.h \
	org/w3c/dom/bootstrap/HTMLIFrameElementImp.h \
	org/w3c/dom/bootstrap/HTMLImageElementImp.h \
	org/w3c/dom/bootstrap/HTMLInputElementImp.h \
	org/w3c/dom/bootstrap/HTMLKeygenElementImp.h \
	org/w3c/dom/bootstrap/HTMLLIElementImp.h \
	org/w3c/dom/bootstrap/HTMLLabelElementImp.h \
	org/w3c/dom/bootstrap/HTMLLegendElementImp.h \
	org/w3c/dom/bootstrap/HTMLLinkElementImp.h \
	org/w3c/dom/bootstrap/HTMLMapElementImp.h \
	org/w3c/dom/bootstrap/HTMLMarqueeElementImp.h \
	org/w3c/dom/bootstrap/HTMLMediaElementImp.h \
	org/w3c/dom/bootstrap/HTMLMenuElementImp.h \
	org/w3c/dom/bootstrap/HTMLMetaElementImp.h \
	org/w3c/dom/bootstrap/HTMLMeterElementImp.h \
	org/w3c/dom/bootstrap/HTMLModElementImp.h \
	org/w3c/dom/bootstrap/HTMLOListElementImp.h \
	org/w3c/dom/bootstrap/HTMLObjectElementImp.h \
	org/w3c/dom/bootstrap/HTMLOptGroupElementImp.h \
	org/w3c/dom/bootstrap/HTMLOptionElementImp.h \
	org/w3c/dom/bootstrap/HTMLOptionsCollectionImp.h \
	org/w3c/dom/bootstrap/HTMLOutputElementImp.h \
	org/w3c/dom/bootstrap/HTMLParagraphElementImp.h \
	org/w3c/dom/bootstrap/HTMLParamElementImp.h \
	org/w3c/dom/bootstrap/HTMLPreElementImp.h \
	org/w3c/dom/bootstrap/HTMLProgressElementImp.h \
	org/w3c/dom/bootstrap/HTMLPropertiesCollectionImp.h \
	org/w3c/dom/bootstrap/HTMLQuoteElementImp.h \
	org/w3c/dom/bootstrap/HTMLScriptElementImp.h \
	org/w3c/dom/bootstrap/HTMLSelectElementImp.h \
	org/w3c/dom/bootstrap/HTMLSourceElementImp.h \
	org/w3c/dom/bootstrap/HTMLSpanElementImp.h \
	org/w3c/dom/bootstrap/HTMLStyleElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableCaptionElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableCellElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableColElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableDataCellElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableHeaderCellElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableRowElementImp.h \
	org/w3c/dom/bootstrap/HTMLTableSectionElementImp.h \
	org/w3c/dom/bootstrap/HTMLTextAreaElementImp.h \
	org/w3c/dom/bootstrap/HTMLTimeElementImp.h \
	org/w3c/dom/bootstrap/HTMLTitleElementImp.h \
	org/w3c/dom/bootstrap/HTMLTrackElementImp.h \
	org/w3c/dom/bootstrap/HTMLUListElementImp.h \
	org/w3c/dom/bootstrap/HTMLUnknownElementImp.h \
	org/w3c/dom/bootstrap/HTMLVideoElementImp.h \
	org/w3c/dom/bootstrap/HashChangeEventImp.h \
	org/w3c/dom/bootstrap/HistoryImp.h \
	org/w3c/dom/bootstrap/ImageDataImp.h \
	org/w3c/dom/bootstrap/Int16ArrayImp.h \
	org/w3c/dom/bootstrap/Int32ArrayImp.h \
	org/w3c/dom/bootstrap/Int8ArrayImp.h \
	org/w3c/dom/bootstrap/KeyboardEventImp.h \
	org/w3c/dom/bootstrap/LinkStyleImp.h \
	org/w3c/dom/bootstrap/LocalMediaStreamImp.h \
	org/w3c/dom/bootstrap/LocationImp.h \
	org/w3c/dom/bootstrap/MediaControllerImp.h \
	org/w3c/dom/bootstrap/MediaErrorImp.h \
	org/w3c/dom/bootstrap/MediaListImp.h \
	org/w3c/dom/bootstrap/MediaStreamImp.h \
	org/w3c/dom/bootstrap/MediaStreamRecorderImp.h \
	org/w3c/dom/bootstrap/MessageChannelImp.h \
	org/w3c/dom/bootstrap/MessageEventImp.h \
	org/w3c/dom/bootstrap/MessagePortImp.h \
	org/w3c/dom/bootstrap/MouseEventImp.h \
	org/w3c/dom/bootstrap/MutableTextTrackImp.h \
	org/w3c/dom/bootstrap/MutationEventImp.h \
	org/w3c/dom/bootstrap/MutationNameEventImp.h \
	org/w3c/dom/bootstrap/NavigatorImp.h \
	org/w3c/dom/bootstrap/NavigatorUserMediaErrorCallbackImp.h \
	org/w3c/dom/bootstrap/NavigatorUserMediaErrorImp.h \
	org/w3c/dom/bootstrap/NavigatorUserMediaSuccessCallbackImp.h \
	org/w3c/dom/bootstrap/NodeFilterImp.h \
	org/w3c/dom/bootstrap/NodeImp.h \
	org/w3c/dom/bootstrap/NodeIteratorImp.h \
	org/w3c/dom/bootstrap/NodeListImp.h \
	org/w3c/dom/bootstrap/PageTransitionEventImp.h \
	org/w3c/dom/bootstrap/PeerConnectionImp.h \
	org/w3c/dom/bootstrap/PopStateEventImp.h \
	org/w3c/dom/bootstrap/ProcessingInstructionImp.h \
	org/w3c/dom/bootstrap/ProgressEventImp.h \
	org/w3c/dom/bootstrap/PropertyNodeListImp.h \
	org/w3c/dom/bootstrap/RGBColorImp.h \
	org/w3c/dom/bootstrap/RadioNodeListImp.h \
	org/w3c/dom/bootstrap/RangeImp.h \
	org/w3c/dom/bootstrap/RectImp.h \
	org/w3c/dom/bootstrap/ScreenImp.h \
	org/w3c/dom/bootstrap/SignalingCallbackImp.h \
	org/w3c/dom/bootstrap/StreamEventImp.h \
	org/w3c/dom/bootstrap/StreamTrackImp.h \
	org/w3c/dom/bootstrap/StyleMediaImp.h \
	org/w3c/dom/bootstrap/StyleSheetImp.h \
	org/w3c/dom/bootstrap/TextEventImp.h \
	org/w3c/dom/bootstrap/TextImp.h \
	org/w3c/dom/bootstrap/TextMetricsImp.h \
	org/w3c/dom/bootstrap/TextTrackCueImp.h \
	org/w3c/dom/bootstrap/TextTrackCueListImp.h \
	org/w3c/dom/bootstrap/TextTrackImp.h \
	org/w3c/dom/bootstrap/TimeRangesImp.h \
	org/w3c/dom/bootstrap/TransferableImp.h \
	org/w3c/dom/bootstrap/TreeWalkerImp.h \
	org/w3c/dom/bootstrap/UIEventImp.h \
	org/w3c/dom/bootstrap/Uint16ArrayImp.h \
	org/w3c/dom/bootstrap/Uint32ArrayImp.h \
	org/w3c/dom/bootstrap/Uint8ArrayImp.h \
	org/w3c/dom/bootstrap/UndoManagerEventImp.h \
	org/w3c/dom/bootstrap/UndoManagerImp.h \
	org/w3c/dom/bootstrap/ValidityStateImp.h \
	org/w3c/dom/bootstrap/VideoTrackImp.h \
	org/w3c/dom/bootstrap/VideoTrackListImp.h \
	org/w3c/dom/bootstrap/WheelEventImp.h \
	org/w3c/dom/bootstrap/WindowImp.h \
	org/w3c/dom/bootstrap/WindowModalImp.h \
	org/w3c/dom/bootstrap/XMLHttpRequestEventTargetImp.h \
	org/w3c/dom/bootstrap/XMLHttpRequestImp.h \
	org/w3c/dom/bootstrap/XMLHttpRequestUploadImp.h

generated_imp_sources = \
	org/w3c/dom/bootstrap/AnonXMLHttpRequestImp.cpp \
	org/w3c/dom/bootstrap/ApplicationCacheImp.cpp \
	org/w3c/dom/bootstrap/ArrayBufferImp.cpp \
	org/w3c/dom/bootstrap/ArrayBufferViewImp.cpp \
	org/w3c/dom/bootstrap/AttrImp.cpp \
	org/w3c/dom/bootstrap/AudioTrackImp.cpp \
	org/w3c/dom/bootstrap/AudioTrackListImp.cpp \
	org/w3c/dom/bootstrap/BarPropImp.cpp \
	org/w3c/dom/bootstrap/BeforeUnloadEventImp.cpp \
	org/w3c/dom/bootstrap/BlobCallbackImp.cpp \
	org/w3c/dom/bootstrap/BlobImp.cpp \
	org/w3c/dom/bootstrap/BooleanCallbackImp.cpp \
	org/w3c/dom/bootstrap/CSS2PropertiesImp.cpp \
	org/w3c/dom/bootstrap/CSSCharsetRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSColorComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSFontFaceRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSIdentifierComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSImportRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSKeywordComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSLengthComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSMapValueImp.cpp \
	org/w3c/dom/bootstrap/CSSMediaRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSNamespaceRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSPageRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSPercentageComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSPrimitiveValueImp.cpp \
	org/w3c/dom/bootstrap/CSSPropertyValueImp.cpp \
	org/w3c/dom/bootstrap/CSSPropertyValueListImp.cpp \
	org/w3c/dom/bootstrap/CSSRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSStringComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSStyleDeclarationImp.cpp \
	org/w3c/dom/bootstrap/CSSStyleDeclarationValueImp.cpp \
	org/w3c/dom/bootstrap/CSSStyleRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSStyleSheetImp.cpp \
	org/w3c/dom/bootstrap/CSSURLComponentValueImp.cpp \
	org/w3c/dom/bootstrap/CSSUnknownRuleImp.cpp \
	org/w3c/dom/bootstrap/CSSValueImp.cpp \
	org/w3c/dom/bootstrap/CSSValueListImp.cpp \
	org/w3c/dom/bootstrap/CanvasGradientImp.cpp \
	org/w3c/dom/bootstrap/CanvasPatternImp.cpp \
	org/w3c/dom/bootstrap/CanvasPixelArrayImp.cpp \
	org/w3c/dom/bootstrap/CanvasRenderingContext2DImp.cpp \
	org/w3c/dom/bootstrap/CaretPositionImp.cpp \
	org/w3c/dom/bootstrap/CharacterDataImp.cpp \
	org/w3c/dom/bootstrap/ClientRectImp.cpp \
	org/w3c/dom/bootstrap/ClientRectListImp.cpp \
	org/w3c/dom/bootstrap/CommentImp.cpp \
	org/w3c/dom/bootstrap/CompositionEventImp.cpp \
	org/w3c/dom/bootstrap/CounterImp.cpp \
	org/w3c/dom/bootstrap/CustomEventImp.cpp \
	org/w3c/dom/bootstrap/CustomEventInitImp.cpp \
	org/w3c/dom/bootstrap/DOMElementMapImp.cpp \
	org/w3c/dom/bootstrap/DOMImplementationCSSImp.cpp \
	org/w3c/dom/bootstrap/DOMImplementationImp.cpp \
	org/w3c/dom/bootstrap/DOMSettableTokenListImp.cpp \
	org/w3c/dom/bootstrap/DOMStringListImp.cpp \
	org/w3c/dom/bootstrap/DOMStringMapImp.cpp \
	org/w3c/dom/bootstrap/DOMTokenListImp.cpp \
	org/w3c/dom/bootstrap/DataTransferImp.cpp \
	org/w3c/dom/bootstrap/DataTransferItemImp.cpp \
	org/w3c/dom/bootstrap/DataTransferItemListImp.cpp \
	org/w3c/dom/bootstrap/DataViewImp.cpp \
	org/w3c/dom/bootstrap/DocumentCSSImp.cpp \
	org/w3c/dom/bootstrap/DocumentFragmentImp.cpp \
	org/w3c/dom/bootstrap/DocumentImp.cpp \
	org/w3c/dom/bootstrap/DocumentRangeImp.cpp \
	org/w3c/dom/bootstrap/DocumentTraversalImp.cpp \
	org/w3c/dom/bootstrap/DocumentTypeImp.cpp \
	org/w3c/dom/bootstrap/DragEventImp.cpp \
	org/w3c/dom/bootstrap/ElementCSSInlineStyleImp.cpp \
	org/w3c/dom/bootstrap/ElementImp.cpp \
	org/w3c/dom/bootstrap/EventImp.cpp \
	org/w3c/dom/bootstrap/EventInitImp.cpp \
	org/w3c/dom/bootstrap/EventListenerImp.cpp \
	org/w3c/dom/bootstrap/EventSourceImp.cpp \
	org/w3c/dom/bootstrap/EventTargetImp.cpp \
	org/w3c/dom/bootstrap/ExternalImp.cpp \
	org/w3c/dom/bootstrap/FileCallbackImp.cpp \
	org/w3c/dom/bootstrap/FileErrorImp.cpp \
	org/w3c/dom/bootstrap/FileImp.cpp \
	org/w3c/dom/bootstrap/FileListImp.cpp \
	org/w3c/dom/bootstrap/FileReaderImp.cpp \
	org/w3c/dom/bootstrap/FileReaderSyncImp.cpp \
	org/w3c/dom/bootstrap/Float32ArrayImp.cpp \
	org/w3c/dom/bootstrap/Float64ArrayImp.cpp \
	org/w3c/dom/bootstrap/FocusEventImp.cpp \
	org/w3c/dom/bootstrap/FormDataImp.cpp \
	org/w3c/dom/bootstrap/FunctionImp.cpp \
	org/w3c/dom/bootstrap/FunctionStringCallbackImp.cpp \
	org/w3c/dom/bootstrap/HTMLAllCollectionImp.cpp \
	org/w3c/dom/bootstrap/HTMLAnchorElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLAppletElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLAreaElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLAudioElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLBRElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLBaseElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLBaseFontElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLBodyElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLButtonElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLCanvasElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLCollectionImp.cpp \
	org/w3c/dom/bootstrap/HTMLCommandElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDListElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDataListElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDetailsElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDirectoryElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDivElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLDocumentImp.cpp \
	org/w3c/dom/bootstrap/HTMLElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLEmbedElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLFieldSetElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLFontElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLFormControlsCollectionImp.cpp \
	org/w3c/dom/bootstrap/HTMLFormElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLFrameElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLFrameSetElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLHRElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLHeadElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLHeadingElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLHtmlElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLIFrameElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLImageElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLInputElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLKeygenElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLLIElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLLabelElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLLegendElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLLinkElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMapElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMarqueeElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMediaElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMenuElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMetaElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLMeterElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLModElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLOListElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLObjectElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLOptGroupElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLOptionElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLOptionsCollectionImp.cpp \
	org/w3c/dom/bootstrap/HTMLOutputElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLParagraphElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLParamElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLPreElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLProgressElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLPropertiesCollectionImp.cpp \
	org/w3c/dom/bootstrap/HTMLQuoteElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLScriptElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLSelectElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLSourceElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLSpanElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLStyleElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableCaptionElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableCellElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableColElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableDataCellElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableHeaderCellElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableRowElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTableSectionElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTextAreaElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTimeElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTitleElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLTrackElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLUListElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLUnknownElementImp.cpp \
	org/w3c/dom/bootstrap/HTMLVideoElementImp.cpp \
	org/w3c/dom/bootstrap/HashChangeEventImp.cpp \
	org/w3c/dom/bootstrap/HistoryImp.cpp \
	org/w3c/dom/bootstrap/ImageDataImp.cpp \
	org/w3c/dom/bootstrap/Int16ArrayImp.cpp \
	org/w3c/dom/bootstrap/Int32ArrayImp.cpp \
	org/w3c/dom/bootstrap/Int8ArrayImp.cpp \
	org/w3c/dom/bootstrap/KeyboardEventImp.cpp \
	org/w3c/dom/bootstrap/LinkStyleImp.cpp \
	org/w3c/dom/bootstrap/LocationImp.cpp \
	org/w3c/dom/bootstrap/MediaControllerImp.cpp \
	org/w3c/dom/bootstrap/MediaErrorImp.cpp \
	org/w3c/dom/bootstrap/MediaListImp.cpp \
	org/w3c/dom/bootstrap/MediaStreamRecorderImp.cpp \
	org/w3c/dom/bootstrap/MessageChannelImp.cpp \
	org/w3c/dom/bootstrap/MessageEventImp.cpp \
	org/w3c/dom/bootstrap/MessagePortImp.cpp \
	org/w3c/dom/bootstrap/MouseEventImp.cpp \
	org/w3c/dom/bootstrap/MutableTextTrackImp.cpp \
	org/w3c/dom/bootstrap/MutationEventImp.cpp \
	org/w3c/dom/bootstrap/MutationNameEventImp.cpp \
	org/w3c/dom/bootstrap/NavigatorImp.cpp \
	org/w3c/dom/bootstrap/NavigatorUserMediaErrorCallbackImp.cpp \
	org/w3c/dom/bootstrap/NavigatorUserMediaErrorImp.cpp \
	org/w3c/dom/bootstrap/NodeFilterImp.cpp \
	org/w3c/dom/bootstrap/NodeImp.cpp \
	org/w3c/dom/bootstrap/NodeIteratorImp.cpp \
	org/w3c/dom/bootstrap/NodeListImp.cpp \
	org/w3c/dom/bootstrap/PageTransitionEventImp.cpp \
	org/w3c/dom/bootstrap/PopStateEventImp.cpp \
	org/w3c/dom/bootstrap/ProcessingInstructionImp.cpp \
	org/w3c/dom/bootstrap/ProgressEventImp.cpp \
	org/w3c/dom/bootstrap/PropertyNodeListImp.cpp \
	org/w3c/dom/bootstrap/RGBColorImp.cpp \
	org/w3c/dom/bootstrap/RadioNodeListImp.cpp \
	org/w3c/dom/bootstrap/RangeImp.cpp \
	org/w3c/dom/bootstrap/RectImp.cpp \
	org/w3c/dom/bootstrap/ScreenImp.cpp \
	org/w3c/dom/bootstrap/SignalingCallbackImp.cpp \
	org/w3c/dom/bootstrap/StreamTrackImp.cpp \
	org/w3c/dom/bootstrap/StyleMediaImp.cpp \
	org/w3c/dom/bootstrap/StyleSheetImp.cpp \
	org/w3c/dom/bootstrap/TextEventImp.cpp \
	org/w3c/dom/bootstrap/TextImp.cpp \
	org/w3c/dom/bootstrap/TextMetricsImp.cpp \
	org/w3c/dom/bootstrap/TextTrackCueImp.cpp \
	org/w3c/dom/bootstrap/TextTrackCueListImp.cpp \
	org/w3c/dom/bootstrap/TextTrackImp.cpp \
	org/w3c/dom/bootstrap/TimeRangesImp.cpp \
	org/w3c/dom/bootstrap/TransferableImp.cpp \
	org/w3c/dom/bootstrap/TreeWalkerImp.cpp \
	org/w3c/dom/bootstrap/UIEventImp.cpp \
	org/w3c/dom/bootstrap/Uint16ArrayImp.cpp \
	org/w3c/dom/bootstrap/Uint32ArrayImp.cpp \
	org/w3c/dom/bootstrap/Uint8ArrayImp.cpp \
	org/w3c/dom/bootstrap/UndoManagerEventImp.cpp \
	org/w3c/dom/bootstrap/UndoManagerImp.cpp \
	org/w3c/dom/bootstrap/ValidityStateImp.cpp \
	org/w3c/dom/bootstrap/VideoTrackImp.cpp \
	org/w3c/dom/bootstrap/VideoTrackListImp.cpp \
	org/w3c/dom/bootstrap/WheelEventImp.cpp \
	org/w3c/dom/bootstrap/WindowImp.cpp \
	org/w3c/dom/bootstrap/WindowModalImp.cpp \
	org/w3c/dom/bootstrap/XMLHttpRequestEventTargetImp.cpp \
	org/w3c/dom/bootstrap/XMLHttpRequestImp.cpp \
	org/w3c/dom/bootstrap/XMLHttpRequestUploadImp.cpp

# Interfaces with compile time problems
#	org/w3c/dom/bootstrap/LocalMediaStreamImp.cpp 
#	org/w3c/dom/bootstrap/MediaStreamImp.cpp 
#	org/w3c/dom/bootstrap/NavigatorUserMediaSuccessCallbackImp.cpp 
#	org/w3c/dom/bootstrap/PeerConnectionImp.cpp 
#	org/w3c/dom/bootstrap/StreamEventImp.cpp 

org/w3c/dom/bootstrap/NodeImp.h : $(idl_files)
	$(esidl) -messenger-imp -string std::u16string -object Object -fno-multipleinheritance $^

org/w3c/dom/bootstrap/NodeImp.cpp : $(idl_files) 
	$(esidl) -messenger-imp-src -string std::u16string -object Object -fno-multipleinheritance $^

$(generated_imp_headers) : org/w3c/dom/bootstrap/NodeImp.h

$(generated_imp_sources) : org/w3c/dom/bootstrap/NodeImp.cpp

bin_PROGRAMS = \
	Reflect.test \
	bridge.test \
	dom.test

bin_PROGRAMS : libesjsapi.a

generated_test_headers = \
	org/w3c/dom/test/Event.h \
	org/w3c/dom/test/EventInit.h \
	org/w3c/dom/test/CustomEventInit.h \
	org/w3c/dom/test/EventListener.h \
	org/w3c/dom/test/EventTarget.h

$(generated_test_headers) : org/w3c/dom/test/Event.h

generated_test_sources = \
	org/w3c/dom/test/Event.cpp \
	org/w3c/dom/test/EventInit.cpp \
	org/w3c/dom/test/CustomEventInit.cpp \
	org/w3c/dom/test/EventListener.cpp \
	org/w3c/dom/test/EventTarget.cpp

$(generated_test_sources) : org/w3c/dom/test/Event.cpp

BUILT_SOURCES += \
	$(generated_imp_sources) $(generated_imp_headers) \
	$(generated_test_headers) $(generated_test_sources)

Reflect_test_SOURCES = \
	test/Reflect.test.cpp

bridge_test_SOURCES = \
	test/bridge.test.cpp \
	test/EventImp.h \
	test/EventListenerImp.h \
	test/EventTargetImp.h \
	test/test.idl

bridge_test_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I $(srcdir)/test

nodist_bridge_test_SOURCES = \
	$(generated_test_headers) $(generated_test_sources)

dom_test_SOURCES = \
	test/dom.test.cpp

nodist_dom_test_SOURCES = \
	$(generated_imp_sources) $(generated_imp_headers)

clean-local:
	rm -rf org *.h

EXTRA_DIST = config.h

org/w3c/dom/test/Event.h : test/test.idl
	$(esidl) -messenger -string std::u16string -object Object -fno-multipleinheritance $^

org/w3c/dom/test/Event.cpp : test/test.idl
	$(esidl) -messenger-src -string std::u16string -object Object -fno-multipleinheritance $^

