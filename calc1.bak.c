#include <ctype.h>
#include <setjmp.h>
#include "str.h"
#define MAXLEN 256

jmp_buf resume_here;

typedef struct Token
{
    char *type;
    string value;
} Token;

typedef struct Interpreter
{
    char *text;
    int pos;
    Token current_token;
} Interpreter;

int factor(Interpreter *);
int term(Interpreter *);
void expr(Interpreter *);
void interpret(Interpreter *);
Token get_next_token(Interpreter *);
void parse (Interpreter *,char *);
void parse_error();

int factor(Interpreter *i)
{
	int curfactor;
	Token fact = i->current_token;
	parse(i,"INTEGER");
	curfactor = atoi(fact.value.ptr);
	return curfactor;
}

int term(Interpreter *i)
{
	int t = factor(i);
	if (strcmp(i->current_token.type,"INTEGER") == 0)
		parse_error();
	while ((strcmp(i->current_token.type,"MULTIPLY") == 0) || (strcmp(i->current_token.type,"DIVIDE") == 0))
	{
		Token tok = i->current_token;
		if (strcmp(tok.type,"MULTIPLY") == 0)
		{
			parse(i,"MULTIPLY");
			t = t * factor(i);
		}
		else
		{
			parse(i,"DIVIDE");
			t = t / factor(i);
		}
	}
	return t;
}

void expr(Interpreter *i)
{
	//Token result;
	int res = term(i);
	while ((strcmp(i->current_token.type,"PLUS") == 0) || (strcmp(i->current_token.type,"MINUS") == 0) || (strcmp(i->current_token.type,"MODULUS") == 0))
	{
		Token tok = i->current_token;
		if (strcmp(tok.type,"PLUS") == 0)
		{
			parse(i,"PLUS");
			res = res + term(i);
		}
		else if (strcmp(tok.type,"MINUS") == 0)
		{
			parse(i,"MINUS");
			res = res - term(i);
		}
		else
		{
			parse(i,"MODULUS");
			res = res % term(i);
		}
	}
	//result.type = "INTEGER";
	//result.value.ptr = "10";
	//return result;
	printf("%d\n",res);
}

void interpret(Interpreter *i)
{
	//space check from main interpret function
	i->pos = 0;
	i->current_token.type = "";
	init_string(&i->current_token.value,MAXLEN);
	i->current_token = get_next_token(i);
	expr(i);
}
	

void parse(Interpreter *i,char *type)
{
    if(strcmp(i->current_token.type,type) == 0)
        i->current_token = get_next_token(i);
    else
        parse_error();
}

void parse_error()
{
    printf("Error parsing input\n");
    longjmp(resume_here,1);
}

Token get_next_token(Interpreter *i)
{
    string current_char;
    init_string(&current_char,MAXLEN);

    while(isspace(i->text[i->pos]))
        current_char.ptr[0] = i->text[++i->pos];

    if (i->pos > strlen(i->text) - 1)
        return (Token)
    {"EOF",current_char
    };

    current_char.ptr[0] = i->text[i->pos];

    if(isdigit(current_char.ptr[0]))
    {
        int j = 0;
        while(isdigit(i->text[++i->pos]))
            current_char.ptr[++j] = i->text[i->pos];
        current_char.ptr[++j] = '\0';
        return (Token)
        {"INTEGER",current_char
        };
    }
    else if(current_char.ptr[0] == '+')
    {
        i->pos++;
        return (Token)
        {"PLUS",current_char
        };
    }
    else if(current_char.ptr[0] == '-')
    {
        i->pos++;
        return (Token)
        {"MINUS",current_char
        };
    }
    else if(current_char.ptr[0] == '*')
    {
        i->pos++;
        return (Token)
        {"MULTIPLY",current_char
        };
    }
    else if(current_char.ptr[0] == '/')
    {
        i->pos++;
        return (Token)
        {"DIVIDE",current_char
        };
    }
    else if(current_char.ptr[0] == '%')
    {
	i->pos++;
	return (Token){"MODULUS",current_char};
    }
    else
    {
        free(current_char.ptr);
        parse_error();
        return (Token)
        {"", {"",0}
        };
    }
}

int main()
{
    //Turn off buffering in stdout
    setvbuf(stdout, NULL, _IONBF, 0);

    Interpreter i;
    i.text = malloc(sizeof(char*) * MAXLEN);

    while(1)
    {
        setjmp(resume_here); //check
        printf("calc> ");
        //scanf("%s",i.text);  //for whitespaces use gets
        gets(i.text);
        if(strcmp(i.text,"exit") == 0)
            break;

        interpret(&i);
    }

    free(i.text);

    return 0;
}
