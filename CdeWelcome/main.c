#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <ctype.h>

#define BSIZE   256
#define NUMSEC  3/* number of seconds to wait*/

#define TYPESIZE 1  /* byte */

#if 1 == TYPESIZE
#   define TYPE char
#   define FORMATW_ADDR "%04zX: %02llX%s"
#   define FORMATWOADDR "%s%02llX%s"
#elif 2 == TYPESIZE
#   define TYPE short
#   define FORMATW_ADDR "%04zX: %04llX%s"
#   define FORMATWOADDR "%s%04llX%s"
#elif 4 == TYPESIZE
#   define TYPE int
#   define FORMATW_ADDR "%04zX: %08llX%s"
#   define FORMATWOADDR "%s%08llX%s"
#else
#   error UNSUPPORTED TYPE SIZE, choose 1,2 or 4
#endif

#define TYPEMASK ((1ULL << TYPESIZE * 8)-1)

#define STARTOFLINE i % (16 / TYPESIZE) == 0
#define MIDOFLINE   (i + 1) % (8 / TYPESIZE) == 0
#define ENDOFLINE   (i + 1) % (16 / TYPESIZE) == 0


#define CDETRACE(msg) printf("%s//"__FILE__"(%d): ", gpszModule, __LINE__), printf msg

void help(int argc, char** argv)
{
    //
    // check argument count and help request
    //
    if (argc < 1 || (argc > 1 && !strcmp(argv[1], "/?")))
    {
        printf("CDE Welcome Demo\n");
        exit(1);
    }
}

char* gpszModule;

int main(int argc, char** argv) {
    clock_t clk = 0;
    char filename[16];                // cdeWelcomeDxe/Pei/Pre fits size
    unsigned char *pBuffer = malloc(BSIZE);
    int i = 0,j = 0;

    gpszModule = argv[0];

    memset(filename, 0, sizeof(filename));      // clear the pBuffer
    sscanf(argv[0], "%15s", &filename[0]);      // get the file name, limit to length of 16
    
    while ('\0' != filename[i])                 // upcase the filename
    {
        filename[i] = (char)toupper(filename[i]);
        i++;
    }
    
    printf("\n\n----------------\n");

    CDETRACE(("%s ->\n Parameters are typechecked at build time by compilers from %ls, %s and %ls during %lld to %d \n",
        "INTRODUCTION: \"CDETRACE\" macro.",
        /*%ls*/     L"GNU",
        /*%s*/      "MICROSOFT",
        /*%ls*/     L"others",
        /*%lld*/   (long long)2015,
        /*%d*/     (int)2021
        ));

    //
    // check command line arguments
    //
    help(argc, argv);  // check command line parameter

    //
    // check if buffer allocation pBuffer has failed...
    //
    if (NULL == pBuffer)
        abort();
    //
    // initialize the pBuffer
    //
    for (i = 0; i < BSIZE; i++)
        pBuffer[i] = (char)i;

    //
    // print the pBuffer to the console/stdout in "dump"-format
    //
    wprintf(L"Printing %d bytes of initilaized memory in a \"dump\"-format:\n", BSIZE);
    for (i = 0; i < BSIZE/TYPESIZE; i++)
    {
        
        printf( // this single printf()-format specifier is tricky...
            STARTOFLINE ? FORMATW_ADDR : FORMATWOADDR,
            STARTOFLINE ? (size_t)(i * TYPESIZE) : (size_t)"",      /*NOTE: this hack works as long as TYPESIZE is <= pointer size
                                                                            64 Bit data size won't run in 32Bit mode                */
            (unsigned long long)(TYPEMASK & (((TYPE*)pBuffer)[i])),
            ENDOFLINE ? "\n" : (MIDOFLINE ? " - " : " "));
    }

    for (i = NUMSEC; i > -1; i--) 
    {
        clk = CLOCKS_PER_SEC + clock();
        sprintf(pBuffer, "%ds...      ", i);
        printf("%s", pBuffer);
        fflush(stdout);

        while (clk > clock())                   // wait a second
            ;

        for (j = 0; j < (signed)strlen(pBuffer); j++)
            printf("\b");

    }
    for (j = 0; j < (signed)strlen(pBuffer); j++)
        printf(" ");
    printf("\n");

    //
    // no need to free() malloced memory. This is automatically done by CRT0
    //
    return 0;
}
