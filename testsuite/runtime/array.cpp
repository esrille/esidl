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

bool testArray(const char* info, char type, unsigned rank)
{
    Reflect::Array array(info);
    printf("%c %u\n", array.getType().getType(), array.getRank());
    if (array.getType().getType() != type)
        return false;
    if (array.getRank() != rank)
        return false;
    return true;
}

int main()
{
    if (!testArray("Y10l", 'l', 10))
    {
        return 1;
    }
    if (!testArray("Y300O3Bar", 'O', 300))
    {
        return 1;
    }
    return 0;
}
