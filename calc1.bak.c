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
	//snprintf(fact.value.ptr,sizeof(fact.value.ptr),"%d",curfactor);
	curfactor = atoi(fact.value.ptr);
	return curfactor;
}

int term(Interpreter *i)
{
	int t = factor(i);
	while ((strcmp(i->current_token.type,"MUL") == 0) || (strcmp(i->current_token.type,"DIV") == 0))
	{
		Token tok = i->current_token;
		if (strcmp(tok.type,"MUL") == 0)
		{
			parse(i,"MUL");
			t = t * factor(i);
		}
		else
		{
			parse(i,"DIV");
			t = t / factor(i);
		}
	}
	return t;
}

void expr(Interpreter *i)
{
	Token result;
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


/*void interpret(Interpreter *i)
{
    Token left,op,right;
    int result,flag = 0;

    i->pos = 0;
    i->current_token.type = "";
    init_string(&i->current_token.value,MAXLEN);

    i->current_token = expr(i);
    left = i->current_token;
    parse(i,"INTEGER");
    //printf("%s %d %s %s\n",i->text,i->pos,i->current_token.type,i->current_token.value.ptr);
    while(strcmp(i->current_token.type,"EOF") != 0)
    {
        op = i->current_token;

        if(strcmp(op.type,"PLUS") == 0)
        {
            parse(i,"PLUS");
            flag = 1;
        }
        else if(strcmp(op.type,"MINUS") == 0)
        {
            parse(i,"MINUS");
            flag = 2;
        }
        else if(strcmp(op.type,"MULTIPLY") == 0)
        {
            parse(i,"MULTIPLY");
            flag = 3;
        }
        else if(strcmp(op.type,"DIVIDE") == 0)
        {
            parse(i,"DIVIDE");
            flag = 4;
        }
	else if(strcmp(op.type,"MODULUS") == 0)
	{
		parse(i,"MODULUS");
		flag = 5;
	}
        else
            parse_error();

        right = i->current_token;
        parse(i,"INTEGER");

        switch(flag)
        {
        case 1:
            result = atoi(left.value.ptr) + atoi(right.value.ptr);
            break;
        case 2:
            result = atoi(left.value.ptr) - atoi(right.value.ptr);
            break;
        case 3:
            result = atoi(left.value.ptr) * atoi(right.value.ptr);
            break;
        case 4:
            result = atoi(left.value.ptr) / atoi(right.value.ptr);
            break;
	case 5:
	    result = atoi(left.value.ptr) % atoi(right.value.ptr);
	    break;
        }

        snprintf(left.value.ptr, sizeof(left.value.ptr),"%d", result);
    }
    if(flag)
        printf("%d\n",result);
    else
        display_string(left.value);

    free(i->current_token.value.ptr);
    //printf("%d\n",i->current_token.value);
    //free(i->current_token.value.ptr);
}*/

void interpret(Interpreter *i)
{
	i->pos = 0;
	i->current_token.type = "";
	init_string(&i->current_token.value,MAXLEN);
	i->current_token = get_next_token(i);
	//Token result = expr(i);
	expr(i);
	//int res;
	//snprintf(result.value.ptr,sizeof(result.value.ptr),"%d",res);
	//printf("%d\n",res);
}
	

void parse(Interpreter *i,char *type)
{
    //printf("%s %d %s %s\n",i->text,i->pos,i->current_token.type,i->current_token.value.ptr);
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
