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
#include <stdint.h>
#include <stdlib.h>

template <typename T>
bool testConstant(const char* info, char type, const char* name, double value)
{
    Reflect::Constant constant(info);
    printf("%c '%s' %.20g\n", constant.getType().getType(), constant.getName().c_str(), constant.getValue());
    if (constant.getType().getType() != type)
        return false;
    if (constant.getName() != name)
        return false;
    if (constant.getValue() != value)
        return false;
    return true;
}

int main()
{
    if (!testConstant<int16_t>("Cs1A-2 ", 's', "A", -2))
    {
        return 1;
    }
    if (!testConstant<double>("Cd2PI3.14159265358979 ", 'd', "PI", 3.14159265358979))
    {
        return 1;
    }
    return 0;
}
