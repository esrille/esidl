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

%{

#include <iostream>
#include "esidl.h"

extern FILE* yyin;

extern "C" int yyparse(void);

void yyerror(const char* str)
{
    std::cerr << "error: " << str << '\n';
}

extern "C" int yywrap()
{
    return 1;
}

int includeLevel();

%}

%pure-parser

%union
{
    Attribute*          attribute;
    AttributeList*      attributeList;
    Constant*           constant;
    Declarator*         declarator;
    Enum*               enumeration;
    Expression*         expression;
    Function*           function;
    Identifier*         identifier;
    Interface*          interface;
    Structure*          structure;
    Identifier*         type;
    int                 pointer;
    int                 bits;
    char*               id;                 /* XXX fix strdup() */
}

%{

int yylex(YYSTYPE* yylval);

%}

%token <type>           BOOLEAN
%token                  BYTE
%token <type>           CHAR
%token <expression>     CHARACTER_CONST
%token                  CONST
%token <id>             COMMENT
%token <type>           DOUBLE
%token                  ENUM
%token <expression>     FALSE
%token <type>           FLOAT
%token <id>             ID
%token                  IMPORT
%token <attribute>      IN
%token                  INT
%token <expression>     INTEGER_LITERAL
%token                  INTERFACE
%token                  LONG
%token <attribute>      OBJECT
%token <attribute>      OUT
%token <expression>     STRING_CONST
%token                  SHORT
%token                  STRUCT
%token <expression>     TRUE
%token                  UNSIGNED
%token                  UUID
%token <attribute>      UUID_REP
%token <type>           VOID

%token                  IID_IS
%token                  SIZE_IS
%token <attribute>      REF
%token <attribute>      UNIQUE
%token <attribute>      FULL

%type <pointer>         ptr_operator
%type <bits>            int_size

%type <attributeList>   interface_attributes
%type <attribute>       interface_attribute
%type <attributeList>   param_attributes
%type <attributeList>   param_attribute_list
%type <attribute>       param_attribute

%type <attributeList>   array_bounds_list
%type <attribute>       array_bounds_declarator
%type <attribute>       array_bound

%type <id>              tag
%type <id>              optional_tag
%type <id>              interface_extends
%type <id>              filename
%type <id>              optional_comment

%type <constant>        const_declaration

%type <declarator>      declarator
%type <declarator>      simple_declarator
%type <declarator>      complex_declarator
%type <declarator>      param_declaration
%type <declarator>      array_declarator
%type <declarator>      ref_declarator
%type <declarator>      function_ptr_declarator
%type <declarator>      method_declarator

%type <structure>       struct_specifier

%type <function>        param_declarator
%type <function>        param_declaration_list

%type <type>            type_specifier
%type <type>            simple_type_specifier
%type <type>            integer_type
%type <type>            signed_int
%type <type>            unsigned_int
%type <type>            floating_type
%type <type>            char_type
%type <type>            boolean_type
%type <type>            void_type
%type <type>            uuid_type
%type <type>            enum_specifier

%type <interface>       interface
%type <interface>       interface_header

%type <expression>      integer_const_exp
%type <expression>      conditional_exp
%type <expression>      logical_or_exp
%type <expression>      logical_and_exp
%type <expression>      inclusive_or_exp
%type <expression>      exclusive_or_exp
%type <expression>      and_exp
%type <expression>      equality_exp
%type <expression>      relational_exp
%type <expression>      shift_exp
%type <expression>      additive_exp
%type <expression>      multiplicative_exp
%type <expression>      unary_exp
%type <expression>      primary_exp
%type <expression>      char_const_exp
%type <expression>      string_const_exp
%type <expression>      boolean_const_exp

%%

specification           :   /* empty */
                            {
                                yyerror("empty file");
                                YYABORT;
                            }
                        |   definition_list
                        ;
definition_list         :   definition
                        |   definition_list definition
                        ;
definition              :   import
                        |   optional_comment interface
                            {
                                $2->setComment($1);
                            }
                        |   optional_comment struct_specifier
                            {
                                $2->setComment($1);
                                NameSpace::local().addLast($2, includeLevel());
                            }
                        |   ';' /* dangling semicolon */
                        ;
import                  :   IMPORT import_files ';'
                        ;
import_files            :   filename
                            {
                                NameSpace::local().addImport(new Import($1, includeLevel()));
                            }
                        |   import_files ',' filename
                            {
                                NameSpace::local().addImport(new Import($3, includeLevel()));
                            }
                        ;
filename                :   ID
                        ;
interface               :   interface_header
                            '{'
                                { NameSpace::setLocal($1); }
                                interface_specification
                                { NameSpace::setLocal($1->parent()); }
                            '}'
                        ;
interface_header        :   '[' interface_attributes ']' INTERFACE ID interface_extends
                            {
                                $$ = new Interface($5, &NameSpace::local());
                                $$->setAttributes($2);
                                $$->setExtends($6);
                                NameSpace::local().addLast($$, includeLevel());
                            }
                        ;
interface_attributes    :   interface_attribute
                            {
                                $$ = new AttributeList();
                                $$->addLast($1);
                            }
                        |   interface_attributes ',' interface_attribute
                            {
                                $$ = $1;
                                $$->addLast($3);
                            }
                        ;
interface_attribute     :   UUID '(' UUID_REP ')'
                            {
                                $$ = $3;
                            }
                        |   OBJECT
                            {
                                $$ = 0;
                            }
                        ;
interface_extends       :   /* empty */
                            {
                                $$ = 0;
                            }
                        |   ':' ID
                            {
                                $$ = $2;
                            }
                        ;
interface_specification :   interface_declaration
                        |   interface_specification interface_declaration
                        ;
interface_declaration   :   optional_comment const_declaration ';'
                            {
                                $2->setComment($1);
                                NameSpace::local().addLast($2);
                            }
                        |   optional_comment method_declarator ';'
                            {
                                $2->setComment($1);
                                NameSpace::local().addLast($2);
                            }
                        |   optional_comment struct_specifier
                            {
                                $2->setComment($1);
                                NameSpace::local().addLast($2);
                            }
                        |   optional_comment enum_specifier
                            {
                                $2->setComment($1);
                                NameSpace::local().addLast($2);
                            }
                        |   ';' /* dangling semicolon */
                        ;
const_declaration       :   CONST integer_type ID '=' integer_const_exp
                            {
                                $$ = new Constant($2, 0, $3, $5);
                            }
                        |   CONST CHAR ID '=' char_const_exp
                            {
                                $$ = new Constant($2, 0, $3, $5);
                            }
                        |   CONST CHAR '*'  ID '=' string_const_exp
                            {
                                $$ = new Constant($2, 1, $4, $6);
                            }
                        |   CONST BOOLEAN ID '=' boolean_const_exp
                            {
                                $$ = new Constant($2, 0, $3, $5);
                            }
                        |   CONST VOID '*' ID '=' '0'   /* null pointer */
                            {
                                $$ = new Constant($2, 1, $4, 0);
                            }
                        ;
integer_const_exp       :   conditional_exp
conditional_exp         :   logical_or_exp
                        |   logical_or_exp '?' integer_const_exp ':' conditional_exp
                            {
                                $$ = new Cond($1, $3, $5);
                            }
                        ;
logical_or_exp          :   logical_and_exp
                        |   logical_or_exp "||" logical_and_exp
                            {
                                $$ = new LogicalOr($1, $3);
                            }
                        ;
logical_and_exp         :   inclusive_or_exp
                        |   logical_and_exp "&&" inclusive_or_exp
                            {
                                $$ = new LogicalAnd($1, $3);
                            }
                        ;
inclusive_or_exp        :   exclusive_or_exp
                        |   inclusive_or_exp '|' exclusive_or_exp
                            {
                                $$ = new Or($1, $3);
                            }
                        ;
exclusive_or_exp        :   and_exp
                        |   and_exp '^' and_exp
                            {
                                $$ = new Xor($1, $3);
                            }
                        ;
and_exp                 :   equality_exp
                        |   and_exp '&' equality_exp
                            {
                                $$ = new And($1, $3);
                            }
                        ;
equality_exp            :   relational_exp
                        |   equality_exp "==" relational_exp
                            {
                                $$ = new Eq($1, $3);
                            }
                        |   equality_exp "!=" relational_exp
                            {
                                $$ = new Ne($1, $3);
                            }
                        ;
relational_exp          :   shift_exp
                        |   relational_exp '<' shift_exp
                            {
                                $$ = new Lt($1, $3);
                            }
                        |   relational_exp '>' shift_exp
                            {
                                $$ = new Gt($1, $3);
                            }
                        |   relational_exp "<=" shift_exp
                            {
                                $$ = new Le($1, $3);
                            }
                        |   relational_exp ">=" shift_exp
                            {
                                $$ = new Ge($1, $3);
                            }
                        ;
shift_exp               :   additive_exp
                        |   shift_exp "<<" additive_exp
                            {
                                $$ = new ShiftLeft($1, $3);
                            }
                        |   shift_exp ">>" additive_exp
                            {
                                $$ = new ShiftRight($1, $3);
                            }
                        ;
additive_exp            :   multiplicative_exp
                        |   additive_exp '+' multiplicative_exp
                            {
                                $$ = new Add($1, $3);
                            }
                        |   additive_exp '-' multiplicative_exp
                            {
                                $$ = new Sub($1, $3);
                            }
                        ;
multiplicative_exp      :   unary_exp
                        |   multiplicative_exp '*' unary_exp
                            {
                                $$ = new Mul($1, $3);
                            }
                        |   multiplicative_exp '/' unary_exp
                            {
                                $$ = new Div($1, $3);
                            }
                        |   multiplicative_exp '%' unary_exp
                            {
                                $$ = new Mod($1, $3);
                            }
                        ;
unary_exp               :   primary_exp
                        |   '+' primary_exp
                            {
                                $$ = new Plus($2);
                            }
                        |   '-' primary_exp
                            {
                                $$ = new Minus($2);
                            }
                        |   '~' primary_exp
                            {
                                $$ = new Tilde($2);
                            }
                        |   '!' primary_exp
                            {
                                $$ = new Not($2);
                            }
                        ;
primary_exp             :   INTEGER_LITERAL
                        |   ID
                            {
                                $$ = dynamic_cast<Constant*>(NameSpace::local().lookup($1));
                                if ($$ == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                            }
                        ;

char_const_exp          :   CHARACTER_CONST
                        |   ID
                            {
                                $$ = dynamic_cast<Constant*>(NameSpace::local().lookup($1));
                                if ($$ == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                            }
                        ;
string_const_exp        :   STRING_CONST
                        |   ID
                            {
                                $$ = dynamic_cast<Constant*>(NameSpace::local().lookup($1));
                                if ($$ == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                            }
                        ;
boolean_const_exp       :   TRUE
                        |   FALSE
                        |   ID
                            {
                                $$ = dynamic_cast<Constant*>(NameSpace::local().lookup($1));
                                if ($$ == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                            }
                        ;
type_specifier          :   simple_type_specifier
                        |   ID
                            {
                                $$ = NameSpace::local().lookup($1);
                                if ($$ == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                            }
                        ;
declarator              :   simple_declarator
                        |   complex_declarator
                        ;
simple_declarator       :   ID
                            {
                                $$ = new Declarator($1);
                            }
                        ;
complex_declarator      :   array_declarator
                        |   function_ptr_declarator
                        |   ref_declarator
                        ;
ref_declarator          :   '&' ID
                            {
                                $$ = new Ref($2);
                            }
                        ;
simple_type_specifier   :   integer_type
                        |   floating_type
                        |   char_type
                        |   boolean_type
                        |   void_type
                        |   uuid_type
                        ;
floating_type           :   FLOAT
                        |   DOUBLE
                        ;
integer_type            :   signed_int
                        |   unsigned_int
                        ;
signed_int              :   int_size
                            {
                                $$ = NameSpace::newInteger($1, true);
                            }
                        ;
unsigned_int            :   UNSIGNED int_size
                            {
                                $$ = NameSpace::newInteger($2, false);
                            }
                        ;
int_size                :   LONG
                            {
                                $$ = 64;
                            }
                        |   INT
                            {
                                $$ = 32;
                            }
                        |   SHORT
                            {
                                $$ = 16;
                            }
                        |   BYTE
                            {
                                $$ = 8;
                            }
                        ;
char_type               :   optional_unsigned CHAR
                            {
                                $$ = $2;
                            }
                        ;
optional_unsigned       :   /* empty */
                        |   UNSIGNED
                        ;
boolean_type            :   BOOLEAN
                        ;
void_type               :   VOID
                            {
                                $$ = NameSpace::newVoid();
                            }
                        ;
uuid_type               :   UUID
                            {
                                $$ = NameSpace::newUuid();
                            }
                        ;
tag                     :   ID
                        ;
optional_tag            :   /* empty */
                            {
                                $$ = 0;
                            }
                        |   ID
                        ;
ptr_operator            :   /* empty */
                            {
                                $$ = 0;
                            }
                        |   ptr_operator '*'
                            {
                                $$ = $1 + 1;
                            }
                        ;
struct_specifier        :   STRUCT tag { $<structure>$ = new Structure($2, &NameSpace::local()); }
                            '{'
                                { NameSpace::setLocal($<structure>3); }
                                struct_specification
                                { NameSpace::setLocal($<structure>3->parent()); }
                            '}'
                            {
                                $$ = $<structure>3;
                            }
                        ;
struct_specification    :   struct_declaration
                        |   struct_specification struct_declaration
                        ;
struct_declaration      :   type_specifier ptr_operator declarator ';'
                            {
                                $3->setType($1);
                                $3->setPointer($2);
                                NameSpace::local().addLast($3);
                            }
                        ;
enum_specifier          :   ENUM optional_tag { $<enumeration>$ = new Enum($2, &NameSpace::local()); }
                            '{'
                                { NameSpace::setLocal($<enumeration>3); }
                                enum_list
                                { NameSpace::setLocal($<enumeration>3->parent()); }
                            '}'
                            {
                                $$ = $<enumeration>3;
                            }
                        ;
enum_list               :   enum_definition
                        |   enum_list ',' enum_definition
                        ;
enum_definition         :   optional_comment ID
                            {
                                Constant* c = new Constant(NameSpace::newInteger(32, true), 0, $2);
                                c->setComment($1);
                                NameSpace::local().addLast(c);
                            }
                        |   optional_comment ID '=' integer_const_exp
                            {
                                Constant* c = new Constant(NameSpace::newInteger(32, true), 0, $2);
                                c->setComment($1);
                                c->setExpression($4);
                                NameSpace::local().addLast(c);
                            }
                        ;
array_declarator        :   ID array_bounds_list
                            {
                                $$ = new Array($1, $2);
                            }
array_bounds_list       :   array_bounds_declarator
                            {
                                $$ = new AttributeList();
                                $$->addLast($1);
                            }
                        |   array_bounds_list array_bounds_declarator
                            {
                                $$ = $1;
                                $$->addLast($2);
                            }
                        ;
array_bounds_declarator :   '[' ']'
                            {
                                $$ = new BoundAttribute();
                            }
                        |   '[' array_bound ']'
                            {
                                $$ = $2;
                            }
                        ;
array_bound             :   INTEGER_LITERAL
                            {
                                $$ = new BoundAttribute($1);
                            }
                        |   ID
                            {
                                Constant* c = dynamic_cast<Constant*>(NameSpace::local().lookup($1));
                                if (c == 0)
                                {
                                    yyerror("unknown identifier");
                                }
                                $$ = new BoundAttribute(c);
                            }
                        ;
function_ptr_declarator :   '(' '*' ID ')'  param_declarator
                            {
                                $$ = $5;
                                $$->setName($3);
                            }
                        ;
method_declarator       :   type_specifier ptr_operator ID param_declarator
                            {
                                $4->setName($3);
                                $4->setType($1);
                                $4->setPointer($2);
                                $$ = $4;
                            }
                        ;
param_declarator        :   '(' ')'
                            {
                                $$ = new Function();
                            }
                        |   '(' param_declaration_list ')'
                            {
                                $$ = $2;
                            }
                        ;
param_declaration_list  :   param_declaration
                            {
                                $$ = new Function();
                                $$->addLast($1);
                            }
                        |   param_declaration_list ',' param_declaration
                            {
                                $$ = $1;
                                $$->addLast($3);
                            }
                        ;
param_declaration       :   param_attributes type_specifier ptr_operator declarator
                            {
                                $$ = $4;
                                $$->setAttributes($1);
                                $$->setType($2);
                                $$->setPointer($3);
                            }
                        ;
param_attributes        :   /* empty */
                            {
                                $$ = 0;
                            }
                        |   '[' param_attribute_list ']'
                            {
                                $$ = $2;
                            }
                        ;
param_attribute_list    :   param_attribute
                            {
                                $$ = new AttributeList();
                                $$->addLast($1);
                            }
                        |   param_attribute_list ',' param_attribute
                            {
                                $$ = $1;
                                $$->addLast($3);
                            }
                        ;
param_attribute         :   IN
                        |   OUT
                        |   IID_IS '(' ID ')'
                            {
                                $$ = new IidIsAttribute($3);
                            }
                        |   SIZE_IS '(' ID ')'
                            {
                                $$ = new SizeIsAttribute($3);
                            }
                        |   REF
                        |   UNIQUE
                        |   FULL
                        ;
optional_comment        :   /* empty */
                            {
                                $$ = 0;
                            }
                        |   COMMENT
                        ;
