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

void initializeWebDatabaseMetaData()
{
    registerMetaData(webdatabase::DatabaseCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::DatabaseCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::Database::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::Database_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::DatabaseSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::DatabaseSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLError::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLError_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLResultSet::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLResultSet_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLResultSetRowList::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLResultSetRowList_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLStatementCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLStatementCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLStatementErrorCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLStatementErrorCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLTransactionCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLTransactionCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLTransactionErrorCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLTransactionErrorCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLTransaction::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLTransaction_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLTransactionSyncCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLTransactionSyncCallback_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLTransactionSync::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLTransactionSync_Bridge<Any, invoke> >::createInstance));
    registerMetaData(webdatabase::SQLVoidCallback::getMetaData(), reinterpret_cast<Object* (*)(ProxyObject)>(Proxy_Impl<ProxyObject, webdatabase::SQLVoidCallback_Bridge<Any, invoke> >::createInstance));
}
