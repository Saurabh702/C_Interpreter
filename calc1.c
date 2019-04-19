#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>

jmp_buf resume_here;

struct token
{
	char *type;
	char value;
};

struct Interpreter
{
	char *text;
	int pos;
	struct token current_token;
};

//typedef struct token T
//typedef struct Interpreter I
void interpret(struct Interpreter *);
struct token get_next_token(struct Interpreter *);
void parse (struct Interpreter *,char *);
void parse_error();

void interpret(struct Interpreter *i)
{
	struct token left,op,right;
	int result;

	i->pos = 0;
	i->current_token.type = "";
    i->current_token.value = '0';

    i->current_token = get_next_token(i);
    //printf("%s %d %s %c\n",i->text,i->pos,i->current_token.type,i->current_token.value);
    left = i->current_token;
    parse(i,"INTEGER");

    op = i->current_token;
    parse(i,"PLUS");

    right = i->current_token;
    parse(i,"INTEGER");

    result = left.value - '0' + right.value - '0';

    printf("%d\n",result);
}

void parse(struct Interpreter *i,char *type)
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

struct token get_next_token(struct Interpreter *i)
{
	char current_char;

    if (i->pos > strlen(i->text) - 1)
		return (struct token){"EOF",'0'};

    current_char = i->text[i->pos];

    if(isdigit(current_char))
    {
        i->pos++;
        return (struct token){"INTEGER",current_char};
    }
    else if(current_char == '+')
    {
        i->pos++;
        return (struct token){"PLUS",current_char};
    }
    else
        parse_error();
}

int main()
{
	//Turn off buffering in stdout
	setvbuf(stdout, NULL, _IONBF, 0);
	
	struct Interpreter i;
    i.text = malloc(sizeof(char*));

	while(1)
	{
		setjmp(resume_here);
		printf("calc> ");
		scanf("%s",i.text);

		if(strcmp(i.text,"exit") == 0)
			break;

		interpret(&i);
	}
	return 0;
}
