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
    ProxyControl::registerMetaData(workers::AbstractWorker::getMetaData(), Proxy_Impl<ProxyObject, workers::AbstractWorker_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::ErrorEvent::getMetaData(), Proxy_Impl<ProxyObject, workers::ErrorEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::SharedWorker::getMetaData(), Proxy_Impl<ProxyObject, workers::SharedWorker_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::Worker::getMetaData(), Proxy_Impl<ProxyObject, workers::Worker_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::WorkerGlobalScope::getMetaData(), Proxy_Impl<ProxyObject, workers::WorkerGlobalScope_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::WorkerLocation::getMetaData(), Proxy_Impl<ProxyObject, workers::WorkerLocation_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::WorkerNavigator::getMetaData(), Proxy_Impl<ProxyObject, workers::WorkerNavigator_Bridge<Any, invoke> >::createInstance);

    ProxyControl::registerMetaData(workers::SharedWorker_Constructor::getMetaData(), Proxy_Impl<ProxyObject, workers::SharedWorker_Constructor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(workers::Worker_Constructor::getMetaData(), Proxy_Impl<ProxyObject, workers::Worker_Constructor_Bridge<Any, invoke> >::createInstance);
}
