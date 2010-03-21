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

void initializeWorkersMetaData()
{
    registerMetaData(workers::AbstractWorker::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::AbstractWorker_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::ErrorEvent::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::ErrorEvent_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::SharedWorker::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::SharedWorker_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::Worker::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::Worker_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::WorkerGlobalScope::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::WorkerGlobalScope_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::WorkerLocation::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::WorkerLocation_Bridge<Any, invoke> >::createInstance));
    registerMetaData(workers::WorkerNavigator::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, workers::WorkerNavigator_Bridge<Any, invoke> >::createInstance));
}
