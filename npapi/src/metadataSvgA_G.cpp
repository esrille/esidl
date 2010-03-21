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

void initializeSvgMetaDataA_G()
{
    registerMetaData(svg::SVGAElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAltGlyphDefElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAltGlyphDefElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAltGlyphElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAltGlyphElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAltGlyphItemElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAltGlyphItemElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAngle::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAngle_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimateColorElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimateColorElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedAngle::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedAngle_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedBoolean::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedBoolean_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedEnumeration::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedEnumeration_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedInteger::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedInteger_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedLength::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedLength_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedLengthList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedLengthList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedNumber::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedNumber_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedNumberList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedNumberList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedPathData::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedPathData_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedPoints::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedPoints_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedPreserveAspectRatio::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedPreserveAspectRatio_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedRect::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedRect_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedString::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedString_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimatedTransformList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimatedTransformList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimateElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimateElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimateMotionElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimateMotionElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimateTransformElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimateTransformElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGAnimationElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGAnimationElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGCircleElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGCircleElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGClipPathElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGClipPathElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGColor::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGColor_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGColorProfileElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGColorProfileElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGColorProfileRule::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGColorProfileRule_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGComponentTransferFunctionElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGComponentTransferFunctionElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGCSSRule::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGCSSRule_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGCursorElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGCursorElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGDefinitionSrcElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGDefinitionSrcElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGDefsElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGDefsElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGDescElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGDescElement_Bridge<Any, invoke> >::createInstance));
    // registerMetaData(svg::SVGDocument::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGDocument_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGElementInstance::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGElementInstance_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGElementInstanceList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGElementInstanceList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGEllipseElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGEllipseElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGExternalResourcesRequired::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGExternalResourcesRequired_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEBlendElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEBlendElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEColorMatrixElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEColorMatrixElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEComponentTransferElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEComponentTransferElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFECompositeElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFECompositeElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEConvolveMatrixElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEConvolveMatrixElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEDiffuseLightingElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEDiffuseLightingElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEDisplacementMapElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEDisplacementMapElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEDistantLightElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEDistantLightElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEFloodElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEFloodElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEFuncAElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEFuncAElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEFuncBElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEFuncBElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEFuncGElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEFuncGElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEFuncRElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEFuncRElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEGaussianBlurElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEGaussianBlurElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEImageElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEImageElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEMergeElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEMergeElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEMergeNodeElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEMergeNodeElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEMorphologyElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEMorphologyElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEOffsetElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEOffsetElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFEPointLightElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFEPointLightElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFESpecularLightingElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFESpecularLightingElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFESpotLightElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFESpotLightElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFETileElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFETileElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFETurbulenceElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFETurbulenceElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFilterElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFilterElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFilterPrimitiveStandardAttributes::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFilterPrimitiveStandardAttributes_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFitToViewBox::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFitToViewBox_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontFaceElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontFaceElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontFaceFormatElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontFaceFormatElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontFaceNameElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontFaceNameElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontFaceSrcElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontFaceSrcElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGFontFaceUriElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGFontFaceUriElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGForeignObjectElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGForeignObjectElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGGElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGGElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGGlyphElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGGlyphElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGGlyphRefElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGGlyphRefElement_Bridge<Any, invoke> >::createInstance));
    registerMetaData(svg::SVGGradientElement::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, svg::SVGGradientElement_Bridge<Any, invoke> >::createInstance));
}
