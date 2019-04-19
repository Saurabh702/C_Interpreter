#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>

jmp_buf resume_here;


typedef struct String 
{
	char *ptr;
	size_t len;
}string;
void init_string(string *,int);
void display_string(string);

void init_string(string *s,int len)
{
    s->len = len;
    s->ptr = malloc(s->len+1);

    if (s->ptr == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
	
	if(len > 0)
		s->ptr[len] = '\0';
	else
		s->ptr[0] = '\0';
}
void display_string(string s)
{
    printf("%s",s.ptr);
}


typedef struct Token
{
	char *type;
	string value;
}Token;

typedef struct Interpreter
{
	char *text;
	int pos;
	Token current_token;
}Interpreter;

void interpret(Interpreter *);
Token get_next_token(Interpreter *);
void parse (Interpreter *,char *);
void parse_error();

void interpret(Interpreter *i)
{
	Token left,op,right;
	int result;

	i->pos = 0;
	i->current_token.type = "";
    init_string(&i->current_token.value,0);
	
    i->current_token = get_next_token(i);
    //printf("%s %d %s %s\n",i->text,i->pos,i->current_token.type,i->current_token.value.ptr);
    left = i->current_token;
    parse(i,"INTEGER");
	int flag = 0;
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
	else
		parse_error();
	
    right = i->current_token;
    parse(i,"INTEGER");
	
    switch(flag)
	{
		case 1:	result = atoi(left.value.ptr) + atoi(right.value.ptr);
				break;
		case 2: result = atoi(left.value.ptr) - atoi(right.value.ptr);
				break;
		case 3: result = atoi(left.value.ptr) * atoi(right.value.ptr);
				break;
		case 4: result = atoi(left.value.ptr) / atoi(right.value.ptr);
				break;
	}
	
    printf("%d\n",result);
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
	init_string(&current_char,1);
    if (i->pos > strlen(i->text) - 1)
		return (Token){"EOF",current_char};

    current_char.ptr[0] = i->text[i->pos];
	
	while(isspace(current_char.ptr[0]))
		current_char.ptr[0] = i->text[++i->pos];
	
	if(isdigit(current_char.ptr[0]))
    {
        i->pos++;
        return (Token){"INTEGER",current_char};
    }
    else if(current_char.ptr[0] == '+')
    {
        i->pos++;
        return (Token){"PLUS",current_char};
    }
	else if(current_char.ptr[0] == '-')
	{
		i->pos++;
        return (Token){"MINUS",current_char};
	}
	else if(current_char.ptr[0] == '*')
	{
		i->pos++;
        return (Token){"MULTIPLY",current_char};
	}
	else if(current_char.ptr[0] == '/')
	{
		i->pos++;
        return (Token){"DIVIDE",current_char};
	}
    else
		parse_error();
}

int main()
{
	//Turn off buffering in stdout
	setvbuf(stdout, NULL, _IONBF, 0);
	
	Interpreter i;
    i.text = malloc(sizeof(char*));

	while(1)
	{
		setjmp(resume_here); //check
		printf("calc> ");
		//scanf("%s",i.text); //free i.text //for whitespaces use get
		gets(i.text);
		if(strcmp(i.text,"exit") == 0)
			break;

		interpret(&i);
	}
	return 0;
}
