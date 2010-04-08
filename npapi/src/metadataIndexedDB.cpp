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

void initializeIndexedDBMetaData()
{
    ProxyControl::registerMetaData(indexeddb::Cursor::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::Cursor_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::CursorRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::CursorRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::CursorSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::CursorSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::DatabaseError::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::DatabaseError_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::Database::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::Database_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::DatabaseRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::DatabaseRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::DatabaseSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::DatabaseSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::DBRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::DBRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::Environment::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::Environment_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::EnvironmentSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::EnvironmentSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IndexedDatabase::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IndexedDatabase_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IndexedDatabaseRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IndexedDatabaseRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::Index::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::Index_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IndexRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IndexRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::IndexSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::IndexSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::KeyRange::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::KeyRange_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::ObjectStore::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::ObjectStore_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::ObjectStoreRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::ObjectStoreRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::ObjectStoreSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::ObjectStoreSync_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::Transaction::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::Transaction_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::TransactionRequest::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::TransactionRequest_Bridge<Any, invoke> >::createInstance);
    ProxyControl::registerMetaData(indexeddb::TransactionSync::getMetaData(), Proxy_Impl<ProxyObject, indexeddb::TransactionSync_Bridge<Any, invoke> >::createInstance);
}
