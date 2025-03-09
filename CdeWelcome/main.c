#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <ctype.h>
#include <signal.h>
#include <CdeServices.h>
#include <math.h>
#include <Library\DebugLib.h>
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

void myCtrlCHandler (int signum) {
    printf("You invoked CTRL+C by 'raise(SIGINT)' during POST, but I don't care\n");
}

int main(int argc, char** argv) {
    clock_t clk = 0;
    char filename[16];                // cdeWelcomeDxe/Pei/Pre fits size
    unsigned char *pBuffer = malloc(BSIZE);
    int i = 0,j = 0;

    gpszModule = argv[0];
    
    // MATH.H demo
    if (1)
    {
        CDEDOUBLE x;
		x.dbl = sin(0.5);
		printf("sin(0.5) = %016llX\n", x.uint64);
        x.dbl = sin(0.0);
        printf("sin(0.0) = %016llX\n", x.uint64);
        x.dbl = sin(1.0);
        printf("sin(1.0) = %016llX\n", x.uint64);
        x.dbl = sin(3.1415926);
        printf("sin(3.1415926) = %016llX\n", x.uint64);

        int e;
		double iptr;
        x.dbl = acos(0.4);
		printf("acos(0.4) = %016llX\n", x.uint64);
        x.dbl = asin(0.4);
		printf("asin(0.4) = %016llX\n", x.uint64);
        x.dbl = atan(0.4);
		printf("atan(0.4) = %016llX\n", x.uint64);
        x.dbl = atan2(0.3, 0.4);
		printf("atan2(0.3, 0.4) = %016llX\n", x.uint64);
        x.dbl = cos(0.4);
		printf("cos(0.4) = %016llX\n", x.uint64);
        x.dbl = sin(0.4);
		printf("sin(0.4) = %016llX\n", x.uint64);
        x.dbl = tan(0.4);
		printf("tan(0.4) = %016llX\n", x.uint64);
        x.dbl = cosh(0.4);
		printf("cosh(0.4) = %016llX\n", x.uint64);
        x.dbl = sinh(0.4);
		printf("sinh(0.4) = %016llX\n", x.uint64);
        x.dbl = tanh(0.4);
		printf("tanh(0.4) = %016llX\n", x.uint64);
        x.dbl = exp(0.4);
		printf("exp(0.4) = %016llX\n", x.uint64);
        x.dbl = frexp(0.3, &e);
		printf("frexp(0.3) = %016llX\n", x.uint64);
        x.dbl = ldexp(0.4, 3);
		printf("ldexp(0.4, 3) = %016llX\n", x.uint64);
        x.dbl = log(0.4);
		printf("log(0.4) = %016llX\n", x.uint64);
        x.dbl = log10(0.4);
		printf("log10(0.4) = %016llX\n", x.uint64);
        x.dbl = modf(0.3, &iptr);
		printf("modf(0.3) = %016llX\n", x.uint64);
        x.dbl = pow(0.4, 0.3);
		printf("pow(0.4, 0.3) = %016llX\n", x.uint64);
        x.dbl = sqrt(0.4);
		printf("sqrt(0.4) = %016llX\n", x.uint64);
        x.dbl = ceil(0.4);
		printf("ceil(0.4) = %016llX\n", x.uint64);
        x.dbl = fabs(0.4);
		printf("fabs(0.4) = %016llX\n", x.uint64);
        x.dbl = floor(0.4);
		printf("floor(0.4) = %016llX\n", x.uint64);
        x.dbl = fmod(0.4, 0.3);
		printf("fmod(0.4, 0.3) = %016llX\n", x.uint64);


    }
   

    DEBUG((DEBUG_ERROR, "-->\n"));
    if (1)
    {
        EFI_GUID guid = { 0x12345678, 0x1111, 0x2222, {0x33, 0x33, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF} };
        EFI_TIME etime = { 
            .Year=2024,
            .Month = 5,
            .Day = 1,
            .Hour = 2,
            .Minute = 3

        };
        EFI_STATUS Status = ((1UL << 31) + 13UL);

        DEBUG((DEBUG_ERROR, "%g\n", &guid));
        DEBUG((DEBUG_ERROR, "%t\n", &etime));
        DEBUG((DEBUG_ERROR, "%r\n", Status));
    }

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
    // install CTRL-C Handler to demonstrate signal handling during post (CTRL-C is "raised()" below)
    //
    signal(SIGINT, myCtrlCHandler);

    //
    // print the pBuffer to the console/stdout in "dump"-format
    //
    wprintf(L"Printing %d bytes of initialized memory in a \"dump\"-format:\n", BSIZE);
    for (i = 0; i < BSIZE/TYPESIZE; i++)
    {
        
        printf( // this single printf()-format specifier is tricky...
            STARTOFLINE ? FORMATW_ADDR : FORMATWOADDR,
            STARTOFLINE ? (size_t)(i * TYPESIZE) : (size_t)"",      /*NOTE: this hack works as long as TYPESIZE is <= pointer size
                                                                            64 Bit data size won't run in 32Bit mode                */
            (unsigned long long)(TYPEMASK & (((TYPE*)pBuffer)[i])),
            ENDOFLINE ? "\n" : (MIDOFLINE ? " - " : " "));
    }

    wprintf(L"\nIf it counts too slow, \"8254/Timer Clock Gating\" is enabled in BIOS Setup.\nDisable setup item!!!\n");

    for (i = NUMSEC; i > -1; i--)
    {
        clk = CLOCKS_PER_SEC + clock();
        sprintf(pBuffer, "%ds...      ", i);
        printf("%s", pBuffer);

        while (clk > clock())                   // wait a second
            ;

        for (j = 0; j < (signed)strlen(pBuffer); j++)
            printf("\b");

    }
    for (j = 0; j < (signed)strlen(pBuffer); j++)
        printf(" ");
    printf("\n");

    //
    // simple demonstration of signal handling CTRL-C
    //
    printf("### raise(SIGINT)\" directly: Invoke signal handler\n");
    raise(SIGINT);
    printf("### raise(SIGINT)\" directly: This will terminate the process\n");
    raise(SIGINT);
    printf("### THIS LINE IS NEVER REACHED ###\n");
    
    //
    // no need to free() malloced memory. This is automatically done by CRT0
    //
    return 0;
}
