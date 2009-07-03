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

bool testMethod(const char* info, char type, char returnType, const char* name, unsigned paramCount)
{
    Reflect::Method method(info);
    printf("[%s %s %s %s]\n",
           method.isSpecialGetter() ? "getter" : "",
           method.isSpecialSetter() ? "setter" : "",
           method.isSpecialCreator() ? "creator" : "",
           method.isSpecialDeleter() ? "deleter" : "");
    printf("%c %c '%s' %d",
           method.getType(), method.getReturnType().getType(), method.getName().c_str(),
           method.getParameterCount());
    if (method.getType() != type)
        return false;
    if (method.getReturnType().getType() != returnType)
        return false;
    if (method.getName() != name)
        return false;
    if (method.getParameterCount() != paramCount)
        return false;
    printf(" (");
    Reflect::Parameter param = method.listParameter();
    unsigned count = 0;
    while (param.next())
    {
        if (count != 0)
            printf(", ");
        printf("%c '%s'", param.getType().getType(), param.getName().c_str());
        ++count;
    }
    printf(")\n");
    if (count != paramCount)
        return false;
    return true;
}

int main()
{
    if (!testMethod("F0v3foo", Reflect::kOperation, Reflect::kVoid, "foo", 0))
    {
        return 1;
    }
    if (!testMethod("F0O3Foo3bar", Reflect::kOperation, Reflect::kObject, "bar", 0))
    {
        return 1;
    }
    if (!testMethod("F1v3bazb2hi", Reflect::kOperation, Reflect::kVoid, "baz", 1))
    {
        return 1;
    }
    if (!testMethod("F2v3bazb2hif5there", Reflect::kOperation, Reflect::kVoid, "baz", 2))
    {
        return 1;
    }
    if (!testMethod("F1v8setFocusp5focus", Reflect::kOperation, Reflect::kVoid, "setFocus", 1))
    {
        return 1;
    }

    if (!testMethod("Fg1A10getByIndexm5index", Reflect::kOperation, Reflect::kAny, "getByIndex", 1))
    {
        return 1;
    }
    if (!testMethod("Fs2v10setByIndexm5indexA5value", Reflect::kOperation, Reflect::kVoid, "setByIndex", 2))
    {
        return 1;
    }
    if (!testMethod("Fd1v13removeByIndexm5index", Reflect::kOperation, Reflect::kVoid, "removeByIndex", 1))
    {
        return 1;
    }
    if (!testMethod("Fg1A3getD4name", Reflect::kOperation, Reflect::kAny, "get", 1))
    {
        return 1;
    }
    if (!testMethod("Fs2v3setD4nameA5value", Reflect::kOperation, Reflect::kVoid, "set", 2))
    {
        return 1;
    }
    if (!testMethod("Fd1v6removeD4name", Reflect::kOperation, Reflect::kVoid, "remove", 1))
    {
        return 1;
    }
    return 0;
}
