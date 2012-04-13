#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *xmalloc(size_t sz)
{
	void *p;

	p = malloc(sz);
	if ((p))
		memset(p, 0x00, sz);

	return p;
}

ssize_t readf(char *buf, size_t sz, FILE *fp)
{
    int ch;
    size_t i=0;

    memset(buf, 0x00, sz);

    while (i < sz-1 && !feof(fp)) {
        switch ((ch = getc(fp))) {
            case '\n':
                return i+1;
            case EOF:
                if (!feof(fp) && ferror(fp))
                    return -1;

                return i+1;
            default:
                buf[i++] = (char)ch;
                break;
        }
    }

    return i;
}
