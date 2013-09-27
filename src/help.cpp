/*
 * Copyright 2013 Esrille Inc.
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

#include "esidl.h"

#include <iostream>

namespace {

// cf. http://www.gnu.org/software/help2man/
const char* helpText =
"Usage: esidl [OPTION]... [FILE]...\n"
"Generate C++ header and source files from Web IDL definition files.\n"
"\n"
"  -messenger                         generate C++ header files\n"
"  -messenger-src                     generate C++ source files\n"
"  -messenger-imp                     generate C++ header skeleton files for implementation\n"
"  -messenger-imp-src                 generate C++ source skeleton files for implementation\n"
"  -object NAME                       specify the name of C++ object class\n"
"  -string NAME                       specify the name of C++ string class\n"
"  --help                             display this help and exit\n"
"  --version                          output version information and exit\n"
"\n"
"Examples:\n"
"  esidl -messenger dom.idl html.idl  Generate C++ header files from dom.idl and html.idl.\n"
"\n"
"Report esidl bugs to <https://github.com/esrille/esidl/issues>\n"
"ES Operating System home page: <http://code.google.com/p/es-operating-system/>\n";

}

void help()
{
    std::cout << helpText;
}

void version()
{
    std::cout << PACKAGE_NAME << ' ' << PACKAGE_VERSION << '\n';
}
