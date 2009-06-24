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

bool testInterface(const char* info, const char* qualifiedName, const char* name)
{
    Reflect::Interface interface(info, qualifiedName);
    printf("'%s' '%s' '%s' %d %d %d\n",
           interface.getQualifiedModuleName().c_str(),
           interface.getName().c_str(),
           interface.getQualifiedSuperName().c_str(),
           interface.getConstantCount(),
           interface.getMethodCount(),
           interface.getConstructorCount());
    if (interface.getName() != name)
        return false;
    for (unsigned i = 0; i < interface.getConstantCount(); ++i)
    {
        Reflect::Constant constant = interface.getConstant(i);
        printf("  %s = %g\n", constant.getName().c_str(), constant.getValue());
    }
    for (unsigned i = 0; i < interface.getMethodCount(); ++i)
    {
        Reflect::Method method = interface.getMethod(i);
        printf("  %s()\n", method.getName().c_str());
    }
    for (unsigned i = 0; i < interface.getConstructorCount(); ++i)
    {
        Reflect::Method method = interface.getConstructor(i);
        printf("  %s()\n", method.getName().c_str());
    }
    return true;
}

int main()
{
    if (!testInterface("I1Y" "X1X" "F0v1x" "F0v1y" "Cs1K-2 " "N0v14createInstance", "::A::Y", "Y"))
    {
        return 1;
    }
    return 0;
}
