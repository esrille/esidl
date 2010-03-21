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

void initializeIndexedDBMetaData()
{
    registerMetaData(indexeddb::Cursor::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::Cursor_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::CursorRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::CursorRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::CursorSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::CursorSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::DatabaseError::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::DatabaseError_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::Database::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::Database_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::DatabaseRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::DatabaseRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::DatabaseSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::DatabaseSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::DBRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::DBRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::Environment::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::Environment_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::EnvironmentSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::EnvironmentSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::IndexedDatabase::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::IndexedDatabase_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::IndexedDatabaseRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::IndexedDatabaseRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::Index::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::Index_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::IndexRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::IndexRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::IndexSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::IndexSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::KeyRange::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::KeyRange_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::ObjectStore::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::ObjectStore_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::ObjectStoreRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::ObjectStoreRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::ObjectStoreSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::ObjectStoreSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::Transaction::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::Transaction_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::TransactionRequest::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::TransactionRequest_Bridge<Any, invoke> >::createInstance));
    registerMetaData(indexeddb::TransactionSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, indexeddb::TransactionSync_Bridge<Any, invoke> >::createInstance));
}
