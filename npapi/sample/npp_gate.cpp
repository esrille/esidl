/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

////////////////////////////////////////////////////////////
//
// Implementation of plugin entry points (NPP_*)
// most are just empty stubs for this particular plugin
//

#include "npapi/npapi.h"
#include "npapi/npruntime.h"

NPError NPP_NewStream(NPP instance,
                      NPMIMEType type,
                      NPStream* stream,
                      NPBool seekable,
                      uint16_t* stype)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  *stype = NP_ASFILEONLY;

  NPError rv = NPERR_NO_ERROR;
  return rv;
}

int32_t NPP_WriteReady (NPP instance, NPStream *stream)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  int32_t rv = 0x0fffffff;
  return rv;
}

int32_t NPP_Write (NPP instance, NPStream *stream, int32_t offset, int32_t len, void *buffer)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  int32_t rv = len;
  return rv;
}

NPError NPP_DestroyStream (NPP instance, NPStream *stream, NPError reason)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  NPError rv = NPERR_NO_ERROR;
  return rv;
}

void NPP_Print (NPP instance, NPPrint* printInfo)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return;
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return;
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
  printf("%s\n", __func__);

  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  NPError rv = NPERR_NO_ERROR;
  return rv;
}

#ifdef OJI

jref NPP_GetJavaClass (void)
{
  printf("%s\n", __func__);

  return NULL;
}

#endif  // OJI
