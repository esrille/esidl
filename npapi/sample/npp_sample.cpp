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

#include "sample.h"
#include "proxyImpl.h"

#include <any.h>
#include <reflect.h>
#include <org/w3c/dom.h>

using namespace org::w3c::dom;

#include <new>

char* NPP_GetMIMEDescription()
{
    printf("%s\n", __func__);
    return const_cast<char*>("application/es-npapi-sample:none:ES NPAPI sample plugin");
}

// Invoked from NP_Initialize()
NPError NPP_Initialize()
{
    printf("%s\n", __func__);

    initializeMetaData();
    initializeHtmlMetaData();

    return NPERR_NO_ERROR;
}

// Invoked from NP_Shutdown()
void NPP_Shutdown()
{
    printf("%s\n", __func__);
}

NPError NPP_New(NPMIMEType pluginType, NPP npp, uint16_t mode,
                int16_t argc, char* argn[], char* argv[],
                NPSavedData* saved)
{
    printf("%s\n", __func__);
    if (!npp)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }

    NPObject* window;
    NPN_GetValue(npp, NPNVWindowNPObject, &window);
    SampleInstance* instance = new (std::nothrow) SampleInstance(npp, window);
    npp->pdata = instance;
    if (!instance)
    {
        NPN_ReleaseObject(window);
        return NPERR_INVALID_INSTANCE_ERROR;
    }
    if (ProxyControl* proxyControl = instance->getProxyControl())
    {
        proxyControl->leave();
    }
    return NPERR_NO_ERROR;
}

NPError NPP_Destroy(NPP npp, NPSavedData** save)
{
    printf("%s\n", __func__);
    if (npp == NULL)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }
    PluginInstance* instance = static_cast<SampleInstance*>(npp->pdata);
    if (instance)
    {
        delete instance;
        npp->pdata = 0;
    }
    return NPERR_NO_ERROR;
}

NPError NPP_SetWindow(NPP npp, NPWindow* window)
{
    printf("%s\n", __func__);
    if (!npp)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }
    if (!window)
    {
        return NPERR_GENERIC_ERROR;
    }
    if (!npp->pdata)
    {
        return NPERR_GENERIC_ERROR;
    }
    return NPERR_NO_ERROR;
}

NPObject* NPP_GetScriptableInstance(NPP npp)
{
    printf("%s\n", __func__);
    if (!npp)
    {
        return 0;
    }
    PluginInstance* instance = static_cast<SampleInstance*>(npp->pdata);
    if (instance)
    {
        return instance->getScriptableInstance();
    }
    return 0;
}

NPError NPP_GetValue(NPP npp, NPPVariable variable, void* value)
{
    printf("%s\n", __func__);
    if (!npp)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }
    switch (variable)
    {
    case NPPVpluginNameString:
    case NPPVpluginDescriptionString:
        *reinterpret_cast<const char**>(value) = "ES NPAPI sample plugin";
        break;
    case NPPVpluginScriptableNPObject:
        *reinterpret_cast<NPObject**>(value) = NPP_GetScriptableInstance(npp);
        if (!*reinterpret_cast<NPObject**>(value))
        {
            return NPERR_GENERIC_ERROR;
        }
        break;
    default:
        return NPERR_GENERIC_ERROR;
        break;
    }
    return NPERR_NO_ERROR;
}

int16_t NPP_HandleEvent(NPP npp, void* event)
{
    return 0;
}

void NPP_StreamAsFile(NPP npp, NPStream* stream, const char* fname)
{
    printf("%s\n", __func__);
}
