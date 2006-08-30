/*
 * Copyright (c) 2006
 * Nintendo Co., Ltd.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Nintendo makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

%x import_state

%{

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <set>
#include <stack>
#include "esidl.h"
#include "parser.h"

std::stack<YY_BUFFER_STATE> includeStack;
std::set<std::string> importedFiles;

extern "C" int yyparse(void);

#undef YY_DECL
#define YY_DECL int yylex(YYSTYPE* yylval)

void yyerror(const char* str);

extern "C" int yywrap();

int includeLevel()
{
    return includeStack.size();
}

extern char* includePath;

%}

/* regular definitions */
delim               [ \t\n\f\r]
ws                  {delim}
letter              [A-Za-z_]
digit               [0-9]
hex_digit           [0-9A-Fa-f]
id                  {letter}({letter}|{digit})*
ll_suffix           (ll|LL)
l_suffix            (l|L)
u_suffix            (u|U)
f_suffix            (f|F)
suffix              (({u_suffix}{ll_suffix})|({ll_suffix}{u_suffix})|({u_suffix}{l_suffix})|({l_suffix}{u_suffix})|{ll_suffix}|{l_suffix}|{u_suffix})?
integer             -?{digit}+{suffix}
hex_integer         (0(x|X){hex_digit}+){suffix}
float               {digit}+\.{digit}+({f_suffix}|{l_suffix})?
octet               {hex_digit}{hex_digit}
octet2              {octet}{octet}
uuid_rep            {octet2}{octet2}\-{octet2}\-{octet2}\-{octet2}\-{octet2}{octet2}{octet2}
character_lit       '([^'\\\n]|\\.)*
string_lit          \"([^\"\\\n]|\\.)*
comment             \/\*(([^*])|(\*[^/]))*\*\/

%%

{ws}                { /* No action, and no return */ }

{character_lit}'    {
                        yylval->expression = new Literal<char>((yytext[1] != '\\') ? yytext[1] : yytext[2]);
                        return CHARACTER_CONST;
                    }

{string_lit}\"      {
                        char* s = strdup(yytext + 1);
                        s[strlen(s) - 1] = '\0';
                        yylval->expression = new Literal<char*>(s);
                        return STRING_CONST;
                    }

{integer}           {
                        yylval->expression = new Literal<int>(strtoll(yytext, 0, 10));
                        return INTEGER_LITERAL;
                    }

{hex_integer}       {
                        yylval->expression = new Literal<int>(strtoll(yytext, 0, 16), 16);
                        return INTEGER_LITERAL;
                    }

boolean             {
                        yylval->type = NameSpace::newBoolean();
                        return BOOLEAN;
                    }
byte                {
                        return BYTE;
                    }
char                {
                        yylval->type = NameSpace::newCharacter();
                        return CHAR;
                    }
const               {
                        return CONST;
                    }
double              {
                        yylval->type = NameSpace::newFloat(64);
                        return FLOAT;
                    }
enum                {
                        return ENUM;
                    }
false               {
                        yylval->expression = new Literal<bool>(false);
                        return FALSE;
                    }
float               {
                        yylval->type = NameSpace::newFloat(32);
                        return FLOAT;
                    }
import              {
                        BEGIN(import_state);
                        return IMPORT;
                    }
in                  {
                        yylval->attribute = new InAttribute();
                        return IN;
                    }
int                 {
                        return INT;
                    }
interface           {
                        return INTERFACE;
                    }
long                {
                        return LONG;
                    }
object              {
                        yylval->attribute = new ObjectAttribute();
                        return OBJECT;
                    }
out                 {
                        yylval->attribute = new OutAttribute();
                        return OUT;
                    }
short               {
                        return SHORT;
                    }
struct              {
                        return STRUCT;
                    }
true                {
                        yylval->expression = new Literal<bool>(true);
                        return TRUE;
                    }
unsigned            {
                        return UNSIGNED;
                    }
uuid                {
                        return UUID;
                    }
void                {
                        return VOID;
                    }
iid_is              {
                        return IID_IS;
                    }
size_is             {
                        return SIZE_IS;
                    }
ref                 {
                        yylval->attribute = new RefAttribute();
                        return REF;
                    }
unique              {
                        yylval->attribute = new UniqueAttribute();
                        return UNIQUE;
                    }
full                {
                        yylval->attribute = new FullAttribute();
                        return FULL;
                    }
{id}                {
                        yylval->id = strdup(yytext);
                        return ID;
                    }
{uuid_rep}          {
                        yylval->attribute = new UuidAttribute(yytext);
                        return UUID_REP;
                    }

{comment}           {
                        /* Comment */
                        if (strncmp(yytext, "/**", 3) == 0)
                        {
                            /* Javadoc style comment */
                            yylval->id = strdup(yytext);
                            return COMMENT;
                        }
                    }

"//"                {
                        /* Comment */
                        int c;

                        do
                        {
                            c = yyinput();
                        } while (c != '\n' && c != EOF);
                    }

<import_state>{ws}  { /* No action, and no return */ }

<import_state>{string_lit}\"    {
                        /* save the import file name */
                        yylval->id = strdup(yytext);

                        std::string filename;
                        if (includePath)
                        {
                            filename = includePath;
                            filename += "/";
                            filename += (yytext + 1);
                        }
                        else
                        {
                            filename = yytext + 1;
                        }
                        filename.erase(filename.size() - 1);

                        if (importedFiles.count(filename))
                        {
                            // filename has been imported once.
                            return ID;
                        }
                        importedFiles.insert(filename);

                        includeStack.push(YY_CURRENT_BUFFER);
                        yyin = fopen(filename.c_str(), "r");
                        if (!yyin)
                        {
                            yyerror(filename.c_str());
                            yyerror("Could not open an include file.");
                            break;
                        }

                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                        BEGIN(INITIAL);
                        yyparse();
                        yy_delete_buffer(YY_CURRENT_BUFFER);
                        yy_switch_to_buffer(includeStack.top());
                        includeStack.pop();
                        BEGIN(import_state);
                        return ID;
                    }

<import_state>;     {
                        BEGIN(INITIAL);
                        return (int) yytext[0];
                    }

<import_state>.     { return (int) yytext[0]; }

.                   { return (int) yytext[0]; }

%%
