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

void initializeSvgMetaDataH_N()
{
    ProxyControl::registerMetaData(svg::SVGHKernElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGHKernElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGICCColor::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGICCColor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGImageElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGImageElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLangSpace::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLangSpace_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLength::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLength_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLengthList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLengthList_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLinearGradientElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLinearGradientElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLineElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLineElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGLocatable::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGLocatable_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMarkerElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMarkerElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMaskElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMaskElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMatrix::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMatrix_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMetadataElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMetadataElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMissingGlyphElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMissingGlyphElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGMPathElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGMPathElement_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGNumber::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGNumber_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(svg::SVGNumberList::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGNumberList_Bridge<Any, invoke> >::createInstance);
}
