/*
 * Copyright 2008 Google Inc.
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
 * software derived from the following specification:
 *
 * Common Object Request Broker Architecture: Core Specification,
 * Version 3.0.3, Object Management Group, Inc., March 2004.
 * http://www.omg.org/technology/documents/formal/corba_iiop.htm.
 */

%{

#include "esidl.h"
#include "parser.h"

// #define VERBOSE

#ifndef VERBOSE
#define PRINTF(...)     (__VA_ARGS__)
#else
#define PRINTF(...)     printf(__VA_ARGS__)
#endif

extern "C" int yyparse(void);

#undef YY_DECL
#define YY_DECL int yylex(YYSTYPE* yylval)

void yyerror(const char* str);

extern "C" int yywrap();

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
SingleStringCharacter   ([^'\\\n\r]|\\{EscapeSequence})
DoubleStringCharacter   ([^\"\\\n\r]|\\{EscapeSequence})
IdentifierStart         ([A-Za-z$_]|{Lu}|{Ll}|{Unicode})
IdentifierPart          ([0-9]|{IdentifierStart}|(\\{UnicodeEscapeSequence}))
Identifier              {IdentifierStart}{IdentifierPart}*

FixedPointLiteral       (({DecimalIntegerLiteral}\.[0-9]*)|(\.[0-9]+)|({DecimalIntegerLiteral}))[dD]

MultiLineComment        \/\*(([^*])|(\*[^/]))*\*\/
SingleLineComment       \/\/

PoundSign               ^{WhiteSpace}*#

%%

{WhiteSpace}        { /* No action, and no return */ }
{LineTerminator}    { /* No action, and no return */ }

abstract            { return ABSTRACT; }
any                 { return ANY; }
attribute           { return ATTRIBUTE; }
boolean             { return BOOLEAN; }
case                { return CASE; }
char                { return CHAR; }
const               { return CONST; }
context             { return CONTEXT; }
custom              { return CUSTOM; }
default             { return DEFAULT; }
double              { return DOUBLE; }
exception           { return EXCEPTION; }
enum                { return ENUM; }
FALSE               { return FALSE; }
fixed               { return FIXED; }
float               { return FLOAT; }
getraises           { return GETRAISES; }
import              { return IMPORT; }
in                  { return IN; }
inout               { return INOUT; }
interface           { return INTERFACE; }
local               { return LOCAL; }
long                { return LONG; }
module              { return MODULE; }
native              { return NATIVE; }
Object              { return OBJECT; }
octet               { return OCTET; }
oneway              { return ONEWAY; }
out                 { return OUT; }
private             { return PRIVATE; }
public              { return PUBLIC; }
raises              { return RAISES; }
readonly            { return READONLY; }
setraises           { return SETRAISES; }
sequence            { return SEQUENCE; }
short               { return SHORT; }
string              { return STRING; }
struct              { return STRUCT; }
supports            { return SUPPORTS; }
switch              { return SWITCH; }
TRUE                { return TRUE; }
truncatable         { return TRUNCATABLE; }
typedef             { return TYPEDEF; }
typeid              { return TYPEID; }
typeprefix          { return TYPEPREFIX; }
unsigned            { return UNSIGNED; }
union               { return UNION; }
ValueBase           { return VALUEBASE; }
valuetype           { return VALUETYPE; }
void                { return VOID; }
wchar               { return WCHAR; }
wstring             { return WSTRING; }

"::"                { return OP_SCOPE; }
"<<"                { return OP_SHL; }
">>"                { return OP_SHR; }

uuid                { return UUID; }

{Identifier}        {
                        yylval->name = strdup(yytext);
                        return IDENTIFIER;
                    }

{DecimalIntegerLiteral} {
                        yylval->name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

{OctalIntegerLiteral}   {
                        yylval->name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

{HexIntegerLiteral} {
                        yylval->name = strdup(yytext);
                        return INTEGER_LITERAL;
                    }

'{SingleStringCharacter}'   {
                        yylval->name = strdup(yytext);
                        return CHARACTER_LITERAL;
                    }

L'{SingleStringCharacter}*' {
                        yylval->name = strdup(yytext);
                        return WIDE_CHARACTER_LITERAL;
                    }

{DecimalLiteral}    {
                        yylval->name = strdup(yytext);
                        return FLOATING_PT_LITERAL;
                    }

\"{DoubleStringCharacter}*\"    {
                        yylval->name = strdup(yytext);
                        return STRING_LITERAL;
                    }

L\"{DoubleStringCharacter}*\"   {
                        yylval->name = strdup(yytext);
                        return WIDE_STRING_LITERAL;
                    }

{FixedPointLiteral} {
                        yylval->name = strdup(yytext);
                        return FIXED_PT_LITERAL;
                    }

{MultiLineComment}  {
                        /* MultiLineComment */
                        if (strncmp(yytext, "/**", 3) == 0)
                        {
                            /* Javadoc style comment */
                            yylval->name = strdup(yytext);
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
                    }

{PoundSign}         {
                        return POUND_SIGN;
                    }

{PoundSign}pragma({WhiteSpace})+ID  {
                        return PRAGMA_ID;
                    }

.                   { return (int) yytext[0]; }

%%
