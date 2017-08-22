#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_NULL		'0'
#define TYPE_NONE		'N'
#define TYPE_FALSE		'F'
#define TYPE_TRUE		'T'
#define TYPE_STOPITER		'S'
#define TYPE_ELLIPSIS   	'.'
#define TYPE_INT		'i'
#define TYPE_INT64		'I'
#define TYPE_FLOAT		'f'
#define TYPE_BINARY_FLOAT	'g'
#define TYPE_COMPLEX		'x'
#define TYPE_BINARY_COMPLEX	'y'
#define TYPE_LONG		'l'
#define TYPE_STRING		's'
#define TYPE_INTERNED		't'
#define TYPE_STRINGREF		'R'
#define TYPE_TUPLE		'('
#define TYPE_LIST		'['
#define TYPE_DICT		'{'
#define TYPE_CODE		'c'
#define TYPE_UNICODE		'u'
#define TYPE_UNKNOWN		'?'
#define TYPE_SET		'<'
#define TYPE_FROZENSET  	'>'

void read_magic(FILE *fp, int tablevel);
void read_time(FILE *fp, int tablevel);
void read_object(FILE *fp, int tablevel);
void print_tab(int tablevel);
int read_string(char *s, long *np, FILE *fp);
long read_long(FILE *fp);

int main(int argc, char *argv[]) {
    //int fd = open(argv[1], O_RDONLY);
    FILE *fp = fopen(argv[1], "r");
    int tablevel = 1;

    printf("<PycFile>\n");
    read_magic(fp, tablevel);
    read_time(fp, tablevel);
    read_object(fp, tablevel);
    fclose(fp);
    printf("</PycFile>\n");

}

void print_tab(int tablevel) {
    for (int i = 0; i < tablevel; i++) {
        printf("  ");
    }
}

long read_long(FILE *fp) {
    long a;
    a  = getc(fp);
    a  |= (long)getc(fp) << 8;
    a  |= (long)getc(fp) << 16;
    a  |= (long)getc(fp) << 24;
    return a;
}

int read_string(char *s, long *np, FILE *fp)
{
    long n = *np;
    int printable = 1;
    long end = -1;

    for (int i = 0; i < n; i++)
    {
        int c = fgetc(fp);
        if ( !isprint(c))
            printable = 0;
        if ( c == '<') {
            s[++end] = '&';
            s[++end] = 'l';
            s[++end] = 't';
            s[++end] = ';';
        }
        else if ( c == '>') {
            s[++end] = '&';
            s[++end] = 'g';
            s[++end] = 't';
            s[++end] = ';';
        }
        else {
            s[++end] = c;
        }
    }
    
    *np = end + 1;
    return printable;
}

void read_magic(FILE *fp, int tablevel) {
    long magic = read_long(fp);

    print_tab(tablevel);
    printf("<magic value=\"%ld\"/>\n", magic);
}

void read_time(FILE *fp, int tablevel) {
    time_t time = (time_t) read_long(fp);
    struct tm *local = localtime(&time);

    print_tab(tablevel);
    printf("<time value=\"%d-%d-%d\"/>\n", (local->tm_year + 1900) % 100, local->tm_mon + 1, local->tm_mday);

}

void read_object(FILE *fp, int tablevel) {
    static char *strings[1000];
    static int strings_length = 0;
    //print_tab(tablevel);
    //printf("<codeObject>\n");

    int c = getc(fp);
    if (c == TYPE_NULL) {
        print_tab(tablevel);
        printf("<null/>\n");
	}
	else if (c == TYPE_NONE) {
        print_tab(tablevel);
        printf("<none/>\n");
	}
	else if (c == TYPE_FALSE) {
        print_tab(tablevel);
        printf("<false/>\n");
	}
	else if (c == TYPE_TRUE) {
        print_tab(tablevel);
        printf("<true/>\n");
	}
	else if (c == TYPE_STOPITER) {
        print_tab(tablevel);
        printf("<stopiter/>\n");
	}
	else if (c == TYPE_ELLIPSIS) {
        print_tab(tablevel);
        printf("<ellipsis/>\n");
	}
	else if (c == TYPE_INT) {
        long i = read_long(fp);
        print_tab(tablevel);
        printf("<int value = \"%ld\"/>\n", i);
	}
	else if (c == TYPE_INT64) {
        //long lo4 = read_long(p);
        //long hi4 = read_long(p);
//#if SIZEOF_LONG > 4
        //long x = (hi4 << 32) | (lo4 & 0xFFFFFFFFL);
        //printf("<int64 value=\"%l\"/>\n", x);
//#else
        //unsigned char buf[8];
        //int one = 1;
        //int is_little_endian = (int)*(char*)&one;
        //if (is_little_endian) {
            //memcpy(buf, &lo4, 4);
            //memcpy(buf+4, &hi4, 4);
        //}
        //else {
            //memcpy(buf, &hi4, 4);
            //memcpy(buf+4, &lo4, 4);
        //}
        //return _PyLong_FromByteArray(buf, 8, is_little_endian, 1);
//#endif
    ;
	}
	else if (c == TYPE_FLOAT) {
		//read_float(fp, tablevel + 1);
	}
	else if (c == TYPE_BINARY_FLOAT) {
		//read_binary_float(fp, tablevel + 1);
	}
	else if (c == TYPE_COMPLEX) {
		//read_complex(fp, tablevel + 1);
	}
	else if (c == TYPE_BINARY_COMPLEX) {
		//read_binary_complex(fp, tablevel + 1);
	}
	else if (c == TYPE_LONG) {
		//read_long(fp, tablevel + 1);
	}
	else if (c == TYPE_STRING) {
        long n = read_long(fp);
        char s[4 * n + 1];
        int printable;

        print_tab(tablevel);
		printable = read_string(s, &n, fp);
        s[n] = '\0';
        if ( printable) {
            printf("<str length='%ld' value=\"%s\"/>\n", n, s);
        } else {
            printf("<str length='%ld' value = 'binary'/>\n", n);
        }
	}
	else if (c == TYPE_INTERNED) {
        long n = read_long(fp);
        char s[4 * n + 1];
        int printable = 1;

        print_tab(tablevel);
		printable = read_string(s, &n, fp);
        s[n] = '\0';
        strings[strings_length] = malloc(n + 1);
        memcpy(strings[strings_length], s, n + 1);
        if ( printable) {
            printf("<internStr index='%d' length='%ld' value='%s'/>\n", strings_length, n, s);
        } else {
            printf("<internStr index='%d' length='%ld' value = 'binary'/>\n", strings_length, n);
        }
        strings_length++;
	}
	else if (c == TYPE_STRINGREF) {
        long index = read_long(fp);
        int printable = 1;
        char *s = strings[index];
        char n = strlen(s);
        print_tab(tablevel);
        for (int i = 0; i < n; i++) {
            if (!isprint(s[i])) {
                printable = 0;
            }
        }
        if ( printable) {
            printf("<strRef index='%ld' value='%s'/>\n", index, s);
        } else {
            printf("<strRef index='%ld' value='binary'/>\n", index);
        }
	}
	else if (c == TYPE_TUPLE) {
		//read_tuple(fp, tablevel + 1);
        long n = read_long(fp);
        print_tab(tablevel);
        printf("<tuple length='%ld'>\n", n);
		for (int i = 0; i < n; i++) {
			read_object(fp, tablevel + 1);
		}
        print_tab(tablevel);
        printf("</tuple>\n");
	}
	else if (c == TYPE_LIST) {
        long n = read_long(fp);
        print_tab(tablevel);
        printf("<list length='%ld'>\n", n);
		for (int i = 0; i < n; i++) {
			read_object(fp, tablevel + 1);
		}
        print_tab(tablevel);
        printf("</list>\n");
		//read_list(fp, tablevel + 1);
	}
	else if (c == TYPE_DICT) {
		//read_dict(fp, tablevel + 1);
	}
	else if (c == TYPE_CODE) {
        long integer;

        print_tab(tablevel);
        printf("<codeObject>\n");

        integer = read_long(fp);
        print_tab(tablevel + 1);
        printf("<argCount value = \"%ld\"/>\n", integer);

        integer = read_long(fp);
        print_tab(tablevel + 1);
        printf("<nlocals value = \"%ld\"/>\n", integer);

        integer = read_long(fp);
        print_tab(tablevel + 1);
        printf("<stacksize value = \"%ld\"/>\n", integer);

        integer = read_long(fp);
        print_tab(tablevel + 1);
        printf("<flags value = \"%ld\"/>\n", integer);

        print_tab(tablevel + 1);
        printf("<code>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</code>\n");

        print_tab(tablevel + 1);
        printf("<consts>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</consts>\n");
        
        print_tab(tablevel + 1);
        printf("<names>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</names>\n");

        print_tab(tablevel + 1);
        printf("<varnames>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</varnames>\n");

        print_tab(tablevel + 1);
        printf("<freevars>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</freevars>\n");

        print_tab(tablevel + 1);
        printf("<cellvars>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</cellvars>\n");

        print_tab(tablevel + 1);
        printf("<filename>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</filename>\n");

        print_tab(tablevel + 1);
        printf("<name>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</name>\n");

        integer = read_long(fp);
        print_tab(tablevel + 1);
        printf("<firstlineno value='%ld'/>\n", integer);

        print_tab(tablevel + 1);
        printf("<lnotab>\n");
        read_object(fp, tablevel + 2);
        print_tab(tablevel + 1);
        printf("</lnotab>\n");
        print_tab(tablevel);
        printf("</codeObject>\n");
            //read_code(fp, tablevel + 1);
	}
	else if (c == TYPE_UNICODE) {
		//read_unicode(fp, tablevel + 1);
	}
	else if (c == TYPE_UNKNOWN) {
		//read_unknown(fp, tablevel + 1);
	}
	else if (c == TYPE_SET) {
		//read_set(fp, tablevel + 1);
	}
	else if (c == TYPE_FROZENSET) {
		//read_frozense(fp, tablevel + 1);
	}
}

