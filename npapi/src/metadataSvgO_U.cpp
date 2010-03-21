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

void initializeSvgMetaDataO_U()
{
    registerMetaData(svg::SVGPaint::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPaint_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegArcAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegArcAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegArcRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegArcRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegClosePath::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegClosePath_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoCubicAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoCubicAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoCubicRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoCubicRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoCubicSmoothAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoCubicSmoothAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoCubicSmoothRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoCubicSmoothRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoQuadraticAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoQuadraticAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoQuadraticRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoQuadraticRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoQuadraticSmoothAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoQuadraticSmoothAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegCurvetoQuadraticSmoothRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegCurvetoQuadraticSmoothRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSeg::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSeg_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoHorizontalAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoHorizontalAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoHorizontalRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoHorizontalRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoVerticalAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoVerticalAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegLinetoVerticalRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegLinetoVerticalRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegMovetoAbs::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegMovetoAbs_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPathSegMovetoRel::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPathSegMovetoRel_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPatternElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPatternElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPoint::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPoint_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPointList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPointList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPolygonElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPolygonElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPolylineElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPolylineElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGPreserveAspectRatio::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGPreserveAspectRatio_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGRadialGradientElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGRadialGradientElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGRectElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGRectElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGRect::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGRect_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGRenderingIntent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGRenderingIntent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGScriptElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGScriptElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGSetElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGSetElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGStopElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGStopElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGStringList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGStringList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGStylable::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGStylable_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGStyleElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGStyleElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGSVGElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGSVGElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGSwitchElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGSwitchElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGSymbolElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGSymbolElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTests::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTests_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTextContentElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTextContentElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTextElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTextElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTextPathElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTextPathElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTextPositioningElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTextPositioningElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTitleElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTitleElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTransformable::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTransformable_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTransform::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTransform_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTransformList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTransformList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTRefElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTRefElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGTSpanElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGTSpanElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGUnitTypes::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGUnitTypes_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGURIReference::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGURIReference_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGUseElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGUseElement_Bridge<Any, invoke> >::createInstance));
}
