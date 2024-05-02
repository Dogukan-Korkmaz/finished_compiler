#include "compiler.h"

/*Token'in anahtar kelime(unsigned, if, int, for..vb) 
olup olmadığını anlayan fonksyon.Anahtar kelimeyi döndürür.*/
bool token_is_keyword(struct token* token, const char* value)
{
    return token->type == TOKEN_TYPE_KEYWORD && S_EQ(token->sval, value);
}