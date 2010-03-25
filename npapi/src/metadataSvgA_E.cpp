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

void initializeSvgMetaDataA_E()
{
    registerMetaData(svg::SVGAElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAltGlyphDefElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAltGlyphDefElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAltGlyphElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAltGlyphElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAltGlyphItemElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAltGlyphItemElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAngle::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAngle_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimateColorElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimateColorElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedAngle::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedAngle_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedBoolean::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedBoolean_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedEnumeration::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedEnumeration_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedInteger::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedInteger_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedLength::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedLength_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedLengthList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedLengthList_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedNumber::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedNumber_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedNumberList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedNumberList_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedPathData::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedPathData_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedPoints::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedPoints_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedPreserveAspectRatio::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedPreserveAspectRatio_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedRect::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedRect_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedString::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedString_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimatedTransformList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimatedTransformList_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimateElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimateElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimateMotionElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimateMotionElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimateTransformElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimateTransformElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGAnimationElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGAnimationElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGCircleElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGCircleElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGClipPathElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGClipPathElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGColor::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGColor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGColorProfileElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGColorProfileElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGColorProfileRule::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGColorProfileRule_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGComponentTransferFunctionElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGComponentTransferFunctionElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGCSSRule::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGCSSRule_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGCursorElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGCursorElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGDefinitionSrcElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGDefinitionSrcElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGDefsElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGDefsElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGDescElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGDescElement_Bridge<Any, invoke> >::createInstance);
    // registerMetaData(svg::SVGDocument::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGDocument_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGElementInstance::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGElementInstance_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGElementInstanceList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGElementInstanceList_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGEllipseElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGEllipseElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGEvent::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGEvent_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGExternalResourcesRequired::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGExternalResourcesRequired_Bridge<Any, invoke> >::createInstance);
}
