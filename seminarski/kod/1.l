%option noyywrap

%x comment1
%x comment2

%%
"/*"				{ BEGIN(comment1); }
<comment1>[^*\n]*	{
					}
<comment1>"*"[^/\n]	{
					}
<comment1>"*"\n		{
					}
<comment1>\n		{
					}
<comment1>"*/"		{ BEGIN(INITIAL); }
"//"				{ BEGIN(comment2); }
<comment2>[^*\n]*	{
					}
<comment2>"*"[^/\n]	{
					}
<comment2>"*"\n		{
					}
<comment2>\n		{ BEGIN(INITIAL); }
[\t\n]+				{ putchar('\n'); }
[\t]*				{ putchar(' '); }

%%

int main() 
{
	yylex();
	putchar('\n');
	
	return 0;
}
