#include "stdio.h"
#include "../utils/util.h"
#include "../heap/heap.h"

int stdout_index = 0;
int stderr_index = 0;
int stdin_index = 0;

#define MAX_PRECISION (10)
static const double rounders[MAX_PRECISION + 1] =
    {
        0.5,          // 0
        0.05,         // 1
        0.005,        // 2
        0.0005,       // 3
        0.00005,      // 4
        0.000005,     // 5
        0.0000005,    // 6
        0.00000005,   // 7
        0.000000005,  // 8
        0.0000000005, // 9
        0.00000000005 // 10
};

char *itoa(int x)
{
    int len = 0;
    if (x == 0)
    {
        char *ret = malloc(2);
        ret[0] = '0';
        ret[1] = 0;
        return ret;
    }
    int z = x;
    bool negative = false;
    if (z < 0)
    {
        z = -z;
        negative = true;
    }

    while (z > 0)
    {
        z = z / 10;
        ++len;
    }
    if (!negative)
    {
        char *str = malloc(len + 1);
        int idx = 0;
        while (x > 0)
        {
            int mod = x % 10;
            str[idx++] = (char)mod + '0';
            x = x / 10;
        }
        return str;
    }
    else
    {
        x = -x;
        char *str = malloc(len + 1);
        int idx = 1;
        while (x > 0)
        {
            int mod = x % 10;
            str[idx++] = (char)mod + '0';
            x = x / 10;
        }
        str[0] = '-';
        return str;
    }
}

char *ftoa(double f)
{
    int precision = -1;
    int int_part = 0;
    bool negative = false;
    if (f < 0)
    {
        f = -f;
        negative = true;
    }
    if (precision < 0)
    {
        if (f < 1.0)
            precision = 6;
        else if (f < 10.0)
            precision = 5;
        else if (f < 100.0)
            precision = 4;
        else if (f < 1000.0)
            precision = 3;
        else if (f < 10000.0)
            precision = 2;
        else
            precision = 1;
    }
    if (precision)
        f += rounders[precision];

    int_part = f;
    f -= int_part;
    bool begin_zero = false;
    char *int_part_str = itoa(int_part);
    int cnt = strlen(int_part_str) + 2 + precision;
    char *str = malloc(cnt);
    int i = 0;
    for (i = 0; i < strlen(int_part_str); ++i)
    {
        str[i] = int_part_str[i];
    }
    free(int_part_str);
    str[i] = '.';
    ++i;
    while (--precision)
    {
        f *= 10.0;
        char c = f;
        str[i++] = '0' + c;
        f -= c;
    }
    str[i] = 0;
    return str;
}

void putc(char c)
{
    stdout[stdout_index] = c;
    ++stdout_index;
}
char getc()
{
    if (!stdin_index)
        return 0;
    return stdin[stdin_index--];
}

char *getline()
{
    bool found_endl = false;

    int cnt = 0;
    int ln = 0;
    int x = 0;
    for (x = stdin_index; x > 0; --x)
    {
        if (stdin[x] == '\n')
        {
            ++cnt;
        }
        if (cnt == 3)
        {
            found_endl = true;
            break;
        }
        ++ln;
    }
    if (!found_endl)
    {
        return 0;
    }
    int i = 0;
    printf("x: %d  stdin: %d\n", x + 1, stdin_index);

    for (int z = x + 1; z < ln + x; ++z)
    {
        if (stdin[z] == '\n')
        {
            break;
        }
        printf("hi %c\n", stdin[z]);
    }
    return "test";
}

char *gets()
{
    int len = stdin_index;
    char *str = malloc(len + 1);
    for (int i = 0; i < len; ++i)
    {
        str[i] = stdin[i];
    }
    str[len] = 0;
    return str; // remember to malloc :)
}

int printf(char *fmt, ...)
{
    char *start = fmt;
    va_list ap;
    int narg = 0;
    int len = 0;
    int z = 0;

    fmt = start;
    va_start(ap, fmt);
    while (*fmt)
    {
        if (*fmt == '%')
        {
            ++fmt;
            switch (*fmt)
            {
            case 's':
            {
                char *str = va_arg(ap, char *);
                len += strlen(str);
            }
            break;
            case 'd':
            {
                int x = va_arg(ap, int);
                char *str = itoa(x);
                len += strlen(str);
                free(str);
            }
            break;
            case 'f':
            {
                float x = va_arg(ap, double);
                char *str = ftoa(x);
                len += strlen(str);
                free(str);
            }
            break;
            case 'c':
            {
                ++len;
            }
            break;
            }
            ++fmt;
        }
        else
        {
            ++len;
            ++fmt;
        }
    }
    len += 2;
    char *out = malloc(len);
    fmt = start;
    int idx = 0;
    va_start(ap, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            ++fmt;
            switch (*fmt)
            {
            case 's':
            {
                char *str = va_arg(ap, char *);
                int l = strlen(str);

                for (int i = 0; i < l; ++i)
                {
                    out[idx] = str[i];
                    ++idx;
                }
            }
            break;
            case 'c':
            {
                char c = va_arg(ap, char);
                out[idx] = c;
                ++idx;
            }
            case 'd':
            {
                int x = va_arg(ap, int);
                char *str = itoa(x);
                int l = strlen(str);

                for (int i = 0; i < l; ++i)
                {
                    out[idx] = str[i];
                    ++idx;
                }
            }
            break;
            case 'f':
            {
                float x = va_arg(ap, double);
                char *str = ftoa(x);
                int l = strlen(str);

                for (int i = 0; i < l; ++i)
                {
                    out[idx] = str[i];
                    ++idx;
                }
            }
            break;
            }
            ++fmt;
        }
        else
        {
            out[idx] = *fmt;
            ++fmt;
            ++idx;
        }
    }
    out[idx] = 0;
    int ln = strlen(out);
    for (int i = 0; i < ln; ++i)
    {
        stdout[stdout_index] = out[i];
        ++stdout_index;
    }
    free(out);
}
bool strcmp(char *a, char *b)
{
    if (strlen(a) != strlen(b))
    {
        return false;
    }
    while (*a && *b)
    {
        if (*a != *b)
        {
            return false;
        }
        ++a;
        ++b;
    }

    return true;
}