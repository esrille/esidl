/*
 * Copyright 2011-2013 Esrille Inc.
 * Copyright 2008-2010 Google Inc.
 * Copyright 2007 Nintendo Co., Ltd.
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

/*
 * These coded instructions, statements, and computer programs contain
 * software derived from the following specifications:
 *
 * W3C,
 * Web IDL,
 * W3C Editor’s Draft 22 March 2012.
 * http://www.w3.org/TR/WebIDL/
 *
 * Common Object Request Broker Architecture: Core Specification,
 * Version 3.0.3, Object Management Group, Inc., March 2004.
 * http://www.omg.org/technology/documents/formal/corba_iiop.htm.
 */

%{

#include "esidl.h"
#include "parser.hh"

#ifndef VERBOSE
#define PRINTF(...)     (__VA_ARGS__)
#else
#define PRINTF(...)     printf(__VA_ARGS__)
#endif

void yyerror(const char* str);

extern "C" int yywrap();

void stepLocation()
{
    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column + 1;
    yylloc.last_column += yyleng;
}

static bool poundMode = false;

%}

/* regular definitions */

USP                     (\xe1\x9a\x80)|(\xe1\xa0\x8e)|(\xe2\x80[\x80-\x8a])|(\xe2\x80\xaf)|(\xe2\x81\x9f)|(\xe3\x80\x80)
LS                      (\xe2\x80\xa8)
PS                      (\xe2\x80\xa9)
NBSP                    (\xc2\xa0)
/* Lu up to \u00ff */
Lu                      (\xc3[\x80-\x9e])
/* Ll up to \u00ff */
Ll                      (\xc2[\xaa\xb5\xba])|(\xc3[\x9f-\xbf])
/* Unicode excluding USP, LS, PS, Lu and Li */
Unicode                 ([\xc4-\xdf][\x80-\xbf])|(\xe0[\xa0-\xbf][\x80-\xbf])|(\e1[\x80-\x99][\x80-\xbf])|(\e1\9a[\x81-\xbf])|(\e1[\x9b-\x9f][\x80-\xbf])|(\e1\xa0[\x80-\x8d\x8f-\xbf])|(\e1[\xa1-\xbf][\x80-\xbf])|(\e2\x80[\x8b-\xa7\xaa-\xbf])|(\e2\x81[\x80-\x9e\xa0-\xbf])|(\e2[\x82-\xbf][\x80-\xbf])|(\e3\x80[\x81-\xbf])|(\e3[\x81-\xbf][\x80-\xbf])|([\xe4-\xec][\x80-\xbf][\x80-\xbf])|(\xed[\x80-\x9f][\x80-\xbf])|([\xee-\xef][\x80-\xbf][\x80-\xbf])|(\xf0[\x90-\xbf][\x80-\xbf][\x80-\xbf])|([\xf1-\xf3][\x80-\xbf][\x80-\xbf][\x80-\xbf])|(\xf4[\x80-\x8f][\x80-\xbf][\x80-\xbf])

WhiteSpace              ([ \t\v\f]|{NBSP}|{USP})
LineTerminator          ([\n\r]|{LS}|{PS})
HexDigit                [0-9A-Fa-f]
DecimalDigit            [0-9]
HexIntegerLiteral       0(x|X){HexDigit}+
DecimalLiteral          ({DecimalIntegerLiteral}\.[0-9]*{ExponentPart}?)|(\.[0-9]+{ExponentPart}?)|({DecimalIntegerLiteral}{ExponentPart}?)

ExponentPart            (e|E)[\+\-]?[0-9]+
DecimalIntegerLiteral   0|([1-9][0-9]*)
OctalIntegerLiteral     (0[0-8]+)

SingleEscapeCharacter   ['\"\\bfnrtv]
NonEscapeCharacter      [^'\"\\bfnrtv\n\r]
HexEscapeSequence       x{HexDigit}{2}
UnicodeEscapeSequence   u{HexDigit}{4}
CharacterEscapeSequence {SingleEscapeCharacter}|{NonEscapeCharacter}
EscapeSequence          {CharacterEscapeSequence}|0|{HexEscapeSequence}|{UnicodeEscapeSequence}
DoubleStringCharacter   ([^\"\\\n\r]|\\{EscapeSequence})
Identifier              [A-Z_a-z][0-9A-Z_a-z]*

MultiLineComment        \/\*(([^*])|(\*[^/]))*\*\/
SingleLineComment       \/\/

PoundSign               ^{WhiteSpace}*#

%%

{WhiteSpace}        {
                        ++yylloc.last_column;
                    }
{LineTerminator}    {
                        if (*yytext == '\n')
                        {
                            ++yylloc.last_line;
                            yylloc.last_column = 0;
                            if (poundMode)
                            {
                                poundMode = false;
                                return EOL;
                            }
                        }
                    }

any                 {
                        stepLocation();
                        return ANY;
                    }
attribute           {
                        stepLocation();
                        return ATTRIBUTE;
                    }
byte                {
                        stepLocation();
                        return BYTE;
                    }
ByteString          {   /* cf. http://dev.w3.org/2006/webapi/WebIDL/#idl-ByteString */
                        stepLocation();
                        return STRING;
                    }
boolean             {
                        stepLocation();
                        return BOOLEAN;
                    }
callback            {
                        stepLocation();
                        return CALLBACK;
                    }
const               {
                        stepLocation();
                        return CONST;
                    }
creator             {
                        stepLocation();
                        return CREATOR;
                    }
deleter             {
                        stepLocation();
                        return DELETER;
                    }
dictionary          {
                        stepLocation();
                        return DICTIONARY;
                    }
double              {
                        stepLocation();
                        return DOUBLE;
                    }
Date                {
                        stepLocation();
                        return DATE;
                    }
DOMString           {
                        stepLocation();
                        return STRING;
                    }
enum                {
                        stepLocation();
                        return ENUM;
                    }
exception           {
                        stepLocation();
                        return EXCEPTION;
                    }
false               {
                        stepLocation();
                        return FALSE;
                    }
float               {
                        stepLocation();
                        return FLOAT;
                    }
getraises           {
                        stepLocation();
                        return GETRAISES;
                    }
getter              {
                        stepLocation();
                        return GETTER;
                    }
implements          {
                        stepLocation();
                        return IMPLEMENTS;
                    }
in                  {
                        stepLocation();
                        return IN;
                    }
Infinity            {
                        stepLocation();
                        return INFINITY;
                    }
inherit            {
                        stepLocation();
                        return INHERIT;
                    }
interface           {
                        stepLocation();
                        return INTERFACE;
                    }
legacycaller        {
                        stepLocation();
                        return CALLER;
                    }
long                {
                        stepLocation();
                        return LONG;
                    }
module              {
                        stepLocation();
                        return MODULE;
                    }
NaN                 {
                        stepLocation();
                        return NAN;
                    }
native              {
                        stepLocation();
                        return NATIVE;
                    }
null                {
                        stepLocation();
                        return NULL_LITERAL;
                    }
object              {
                        stepLocation();
                        yylval.name = strdup(Node::getBaseObjectName());
                        return IDENTIFIER;
                    }
octet               {
                        stepLocation();
                        return OCTET;
                    }
omittable           {
                        stepLocation();
                        return OMITTABLE;
                    }
optional            {
                        stepLocation();
                        return OPTIONAL;
                    }
or                  {
                        stepLocation();
                        return OR;
                    }
partial              {
                        stepLocation();
                        return PARTIAL;
                    }

raises              {
                        stepLocation();
                        return RAISES;
                    }
readonly            {
                        stepLocation();
                        return READONLY;
                    }
sequence            {
                        stepLocation();
                        return SEQUENCE;
                    }
setraises           {
                        stepLocation();
                        return SETRAISES;
                    }
setter              {
                        stepLocation();
                        return SETTER;
                    }
short               {
                        stepLocation();
                        return SHORT;
                    }
static              {
                        stepLocation();
                        return STATIC;
                    }
string              {
                        stepLocation();
                        return STRING;
                    }
stringifier         {
                        stepLocation();
                        return STRINGIFIER;
                    }
true                {
                        stepLocation();
                        return TRUE;
                    }
typedef             {
                        stepLocation();
                        return TYPEDEF;
                    }
unrestricted        {
                        stepLocation();
                        return UNRESTRICTED;
                    }
unsigned            {
                        stepLocation();
                        return UNSIGNED;
                    }
void                {
                        stepLocation();
                        return VOID;
                    }

"::"                {
                        stepLocation();
                        return OP_SCOPE;
                    }
"<<"                {
                        stepLocation();
                        return OP_SHL;
                    }
">>"                {
                        stepLocation();
                        return OP_SHR;
                    }
"..."               {
                        stepLocation();
                        return ELLIPSIS;
                    }

{Identifier}        {
                        stepLocation();
                        // A leading "_" is used to escape an identifier from looking like a reserved word terminal.
                        yylval.name = (yytext[0] == '_') ? strdup(yytext + 1) : strdup(yytext);
                        return IDENTIFIER;
                    }

{DecimalIntegerLiteral} {
                        stepLocation();
                        yylval.name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

{OctalIntegerLiteral}   {
                        stepLocation();
                        yylval.name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

{HexIntegerLiteral} {
                        stepLocation();
                        yylval.name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

{DecimalLiteral}    {
                        stepLocation();
                        yylval.name = strdup(yytext);
                        return FLOATING_PT_LITERAL;
                    }

\"{DoubleStringCharacter}*\"    {
                        stepLocation();
                        yylval.name = strdup(yytext);
                        return STRING_LITERAL;
                    }

{MultiLineComment}  {
                        /* MultiLineComment */
                        for (char* s = yytext; *s; ++s)
                        {
                            if (*s == '\n')
                            {
                                ++yylloc.last_line;
                                yylloc.last_column = 0;
                            }
                            else
                            {
                                ++yylloc.last_column;
                            }
                        }
                        if (strncmp(yytext, "/**", 3) == 0)
                        {
                            /* Javadoc style comment */
                            yylval.name = strdup(yytext);
                            return JAVADOC;
                        }
                    }

{SingleLineComment} {
                        /* SingleLineComment */
                        int c;

                        do
                        {
                            c = yyinput();
                        } while (c != '\n' && c != '\r' && c != EOF);
                        ++yylloc.last_line;
                        yylloc.last_column = 0;
                    }

{PoundSign}         {
                        stepLocation();
                        poundMode = true;
                        return POUND_SIGN;
                    }

.                   {
                        stepLocation();
                        return (int) yytext[0];
                    }

%%
