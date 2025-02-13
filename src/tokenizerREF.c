#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_TOKEN_LEN 100
typedef enum {
TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_IF, TOKEN_EQUAL, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_UNKNOWN, TOKEN_EOF
}
TokenType;
typedef struct
{
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

void getNextToken(FILE *file, Token *token)
{
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (isspace(c))
            continue;
        if (isalpha(c))
        {
            int len = 0;
            token->text[len++] = c;
            while (isalnum(c = fgetc(file)))
            {
                if (len < MAX_TOKEN_LEN - 1)
                    token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';
            if (strcmp(token->text, "int") == 0)
                token->type = TOKEN_INT;
            else if (strcmp(token->text, "if") == 0)
                token->type = TOKEN_IF;
            else
                token->type = TOKEN_IDENTIFIER;
            return;
        }
        if (isdigit(c))
        {
            int len = 0;
            token->text[len++] = c;
            while (isdigit(c = fgetc(file)))
            {
                if (len < MAX_TOKEN_LEN - 1)
                    token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';
            token->type = TOKEN_NUMBER;
            return;
        }
        switch (c)
        {
        case '=':
            token->type = TOKEN_ASSIGN;
            token->text[0] = '=';
            token->text[1] = '\0';
            return;
            // develop same for +,-,*,/,etc.
        }
    }
    token->type = TOKEN_EOF;
    token->text[0] = '\0';
}
int main()
{
    FILE *file = fopen("input.txt", "r");
    if (!file)
    {
        perror("Failed to open file");
        return 1;
    }
    Token token;
    do
    {
        getNextToken(file, &token);
        printf("Token: %d, Text: %s\n", token.type, token.text);
    } while (token.type != TOKEN_EOF);
    fclose(file);
    return 0;
}
