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

void initializeWebGLMetaData()
{
    registerMetaData(webgl::WebGLActiveInfo::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLActiveInfo_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLArrayBuffer::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLArrayBuffer_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLBuffer::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLBuffer_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLByteArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLByteArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLContextAttributes::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLContextAttributes_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLFloatArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLFloatArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLFramebuffer::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLFramebuffer_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLIntArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLIntArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLObjectArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLObjectArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLObject::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLObject_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLProgram::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLProgram_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLRenderbuffer::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLRenderbuffer_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLRenderingContext::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLRenderingContext_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLResourceLostEvent::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLResourceLostEvent_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLShader::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLShader_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLShortArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLShortArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLTexture::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLTexture_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUniformLocation::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUniformLocation_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedByteArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedByteArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedIntArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedIntArray_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedShortArray::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedShortArray_Bridge<Any, invoke> >::createInstance);

    registerMetaData(webgl::WebGLArrayBuffer_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLArrayBuffer_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLByteArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLByteArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLFloatArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLFloatArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLIntArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLIntArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLShortArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLShortArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedByteArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedByteArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedIntArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedIntArray_Constructor_Bridge<Any, invoke> >::createInstance);
    registerMetaData(webgl::WebGLUnsignedShortArray_Constructor::getMetaData(), Proxy_Impl<ProxyObject, webgl::WebGLUnsignedShortArray_Constructor_Bridge<Any, invoke> >::createInstance);
}
