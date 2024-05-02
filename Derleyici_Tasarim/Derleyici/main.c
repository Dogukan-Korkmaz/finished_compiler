#include <stdio.h>
#include "helpers/vector.h"
#include "compiler.h"

int main()
{
    // Derleme fonksyonu çağırma sonrası return edilen mesaja göre hata metni yazdırma.
    int res = compile_file("./test.c", "./test", 0);
    if (res == COMPILER_FILE_COMPILED_OK)
    {
        printf("Derleme islemi basarili !\n");
    }
    else if(res == COMPILER_FAILED_WITH_ERRORS)
    {
        printf("Derleme islemi basarisiz !\n");
    }
    else
    {
        printf("Beklenmeyen bir hata meydana geldi !\n");
    }
    return 0;
}