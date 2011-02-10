/*
 * Copyright 2011 Esrille Inc.
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

void initializeIndexedDBMetaData()
{
    ProxyControl::registerMetaData(indexeddb::IDBCursor::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBCursor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBCursorSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBCursorSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBDatabase::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBDatabase_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBDatabaseSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBDatabaseSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBEnvironment::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBEnvironment_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBEnvironmentSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBEnvironmentSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBErrorEvent::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBErrorEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBEvent::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBFactory::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBFactory_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBFactorySync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBFactorySync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBIndex::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBIndex_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBIndexRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBIndexRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBKeyRange::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBKeyRange_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBKeyRangeConstructors::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBKeyRangeConstructors_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBObjectStore::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBObjectStore_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBObjectStoreSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBObjectStoreSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBSuccessEvent::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBSuccessEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBTransaction::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBTransaction_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBTransactionEvent::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBTransactionEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBTransactionSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBTransactionSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBVersionChangeEvent::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBVersionChangeEvent_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IDBVersionChangeRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IDBVersionChangeRequest_Bridge<Any, invoke> >::createInstance);
}
