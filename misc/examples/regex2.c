#define i_extern
#include <stc/cregex.h>

int main()
{
    struct { const char *pattern, *input; } s[] = {
        {"(\\d\\d\\d\\d)[-_](1[0-2]|0[1-9])[-_](3[01]|[12][0-9]|0[1-9])",
         "date: 2024-02-29 leapyear day, christmas eve is on 2022-12-24."
        },
        {"(https?://|ftp://|www\\.)([0-9A-Za-z@:%_+~#=-]+\\.)+([a-z][a-z][a-z]?)(/[/0-9A-Za-z\\.@:%_+~#=\\?&-]*)?",
         "https://en.cppreference.com/w/cpp/regex/regex_search"
        },
        {"!((abc|123)+)!", "!123abcabc!"},
        {"(\\p{L&}+ )+(\\p{Nd}+)", "Großpackung süßigkeiten 199"},
    };

    c_AUTO (cregex, re)
    c_FORRANGE (i, c_ARRAYLEN(s))
    {
        int res = cregex_compile(&re, s[i].pattern, CREG_DEFAULT);
        if (res < 0) {
            printf("error in regex pattern: %d\n", res);
            continue;
        }
        printf("\ninput: %s\n", s[i].input);

        c_FORMATCH (j, &re, s[i].input) {
            c_FORRANGE (k, cregex_captures(&re))
                printf("  submatch %lld: %.*s\n", k, c_ARGSV(j.match[k]));
        }
    }
}