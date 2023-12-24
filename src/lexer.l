%option noyywrap noinput nounput

%{
#include "parser.h"

void update_yylloc(void) {
    size_t i;

    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column;

    for (i = 0; i < yyleng; ++i) {
        switch (yytext[i]) {
        case '\n':
            ++yylloc.last_line;
            yylloc.last_column = 1;
            break;

        default:
            ++yylloc.last_column;
        }
    }
}
%}

S [ \b\n\t\f\r]
W [a-zA-Z_]
D [0-9]
X [0-9a-fA-F]

%%

{S}     update_yylloc();
\/\/.*$ update_yylloc();

function    update_yylloc(); return TL_FUNCTION;
int         update_yylloc(); return TL_INT;
var         update_yylloc(); return TL_VAR;
if          update_yylloc(); return TL_IF;
else        update_yylloc(); return TL_ELSE;
while       update_yylloc(); return TL_WHILE;
do          update_yylloc(); return TL_DO;
for         update_yylloc(); return TL_FOR;
return      update_yylloc(); return TL_RETURN;
break       update_yylloc(); return TL_BREAK;
continue    update_yylloc(); return TL_CONTINUE;
"+="        update_yylloc(); return TL_PLUS_ASN;
"-="        update_yylloc(); return TL_MINUS_ASN;
"*="        update_yylloc(); return TL_MUL_ASN;
"/="        update_yylloc(); return TL_DIV_ASN;
"%="        update_yylloc(); return TL_REM_ASN;
"&="        update_yylloc(); return TL_BAND_ASN;
"^="        update_yylloc(); return TL_BXOR_ASN;
"|="        update_yylloc(); return TL_BOR_ASN;
"<<="       update_yylloc(); return TL_SHL_ASN;
">>="       update_yylloc(); return TL_SHR_ASN;
"&&="       update_yylloc(); return TL_AND_ASN;
"||="       update_yylloc(); return TL_OR_ASN;
"<<"        update_yylloc(); return TL_SHL;
">>"        update_yylloc(); return TL_SHR;
"&&"        update_yylloc(); return TL_AND;
"||"        update_yylloc(); return TL_OR;
"=="        update_yylloc(); return TL_EQ;
"!="        update_yylloc(); return TL_NE;
"<="        update_yylloc(); return TL_LE;
">="        update_yylloc(); return TL_GE;
"++"        update_yylloc(); return TL_INC;
"--"        update_yylloc(); return TL_DEC;

{D}+        update_yylloc(); yylval.int_ = atoi(yytext); return T_INT;
true|false  update_yylloc(); yylval.int_ = yytext[0] == 't'; return T_INT;

{W}({W}|{D})*   update_yylloc(); yylval.id = strdup(yytext); return T_ID;

.       update_yylloc(); return yytext[0];
