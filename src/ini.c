#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "misc.h"
#include "linkedlist.h"
#include "ini.h"

INI *parse_ini(char *filename)
{
    INI *ini;
    FILE *fp;
    SECTION *section=NULL;
    char buf[512+1];
    char *p1, *p2;

    fp = fopen(filename, "r");
    if (!(fp))
        return NULL;

    ini = xmalloc(sizeof(INI));
    if (!(ini)) {
        fclose(fp);
        return NULL;
    }

    while (!feof(fp)) {
        memset(buf, 0x00, sizeof(buf));
        if (readf(buf, sizeof(buf), fp) <= 0)
            break;

        switch (buf[0]) {
            case '\0':
            case '#':
                break;
            case '[':
                p1 = strchr(buf, ']');
                if (!(p1))
                    goto error;

                *p1 = 0x00;
                section = add_section(ini, buf+1);
                break;
            default:
                if (!(section))
                    goto error;

                if (parse_setting(section, buf) == NULL)
                    goto error;

                break;
        }
    }

error:
    fclose(fp);
    return ini;
}

LISTNODE *parse_setting(SECTION *section, char *buf)
{
    char *p1, *p2;

    p1 = buf+(strlen(buf)-1);
    while (isspace(*p1))
        *(p1--) = 0x00;

    p1 = p2 = strchr(buf, '=');
    if (!(p1))
        return NULL;

    *(p1++) = 0x00;
    while (isspace(*p1))
        p1++;

    if (*p1 == 0x00)
        return NULL;

    while (p2 > buf && isspace(*(--p2)))
        *p2=0x00;

    while (isspace(*buf))
        buf++;

    if (p2 < buf || *buf == 0x00 || *p1 == 0x00)
        return NULL;

    return add_setting(section, buf, p1);
}

SECTION *add_section(INI *ini, char *name)
{
    SECTION *section;

    if (!(ini->sections)) {
        section = ini->sections = xmalloc(sizeof(SECTION));
        if (!(section))
            return NULL;
    } else {
        section = ini->sections;
        while ((section->next)) {
            if (!strcmp(section->name, name))
                return section;

            section = section->next;
        }
        
        section->next = xmalloc(sizeof(SECTION));
        if (!(section->next))
            return NULL;

        section = section->next;
    }

    section->name = strdup(name);
    section->settings = xmalloc(sizeof(LINKEDLIST));

    return section;
}

LISTNODE *add_setting(SECTION *section, char *name, char *value)
{
    if (!(section))
        return NULL;

    /* Gotta be +1 to copy the terminating NUL byte */
    return AddNode(section->settings, name, strdup(value), strlen(value)+1);
}

SECTION *get_section(INI *ini, char *name)
{
    SECTION *section;

    for (section = ini->sections; section != NULL; section = section->next)
        if (!strcmp(section->name, name))
            return section;

    return NULL;
}

char *get_section_var(SECTION *section, char *name)
{
    LISTNODE *node;

    for (node = section->settings->head; node != NULL; node = node->next)
        if (!strcmp(node->name, name))
            return node->data;

    return NULL;
}

void free_ini(INI *ini)
{
    SECTION *section, *prev;

    section = ini->sections;
    while ((section)) {
        prev = section;
        FreeList(section->settings);
        section = section->next;
        free(prev);
    }
}

void print_ini(INI *ini)
{
    SECTION *section;

    if (!(ini))
        return;

    for (section = ini->sections; section != NULL; section = section->next) {
        fprintf(stderr, "[%s]\n", section->name);
        PrintList(section->settings);
    }
}
