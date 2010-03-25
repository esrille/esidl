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

void initializeSvgMetaDataV_Z()
{
    registerMetaData(svg::SVGViewElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGViewElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGViewSpec::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGViewSpec_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGVKernElement::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGVKernElement_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGZoomAndPan::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGZoomAndPan_Bridge<Any, invoke> >::createInstance);
    registerMetaData(svg::SVGZoomEvent::getMetaData(), Proxy_Impl<ProxyObject, svg::SVGZoomEvent_Bridge<Any, invoke> >::createInstance);
}
