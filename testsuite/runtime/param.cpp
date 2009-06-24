/*
 * Copyright 2009 Google Inc.
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

#include "reflect.h"

#include <stdio.h>
#include <stdlib.h>

bool testParameter(const char* info, char type, const char* name)
{
    Reflect::Parameter param(info);
    printf("%c '%s'\n", param.getType().getType(), param.getName().c_str());
    if (param.getType().getType() != type)
        return false;
    if (param.getName() != name)
        return false;
    return true;
}

int main()
{
    if (!testParameter("h3foo", 'h', "foo"))
    {
        return 1;
    }
    if (!testParameter("O3Foo3foo", 'O', "foo"))
    {
        return 1;
    }
    return 0;
}
