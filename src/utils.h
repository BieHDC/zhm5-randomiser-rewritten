#ifndef MYUTILS_H
#define MYUTILS_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define true ((int32_t)1)
#define false ((int32_t)0)

// DEBUG
#define FIXME(M, ...) (printf("[FIXME] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__))
#define INFO(M, ...) (printf("[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__))
#define ERR(M, ...) (printf("[ERR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__))

#ifdef DEBUG
#define SPAM(M, ...) (printf("[SPAM] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__))
#else
#define SPAM(...)
#endif

void cstrcat(char** result, const char* str1, const char* str2)
{
    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);
    size_t i = 0, j = 0;

    *result = malloc(str1_len+str2_len+1);

    if (*result) {
        for (i = 0 ; str1[i] != '\0' ; i++, j++)
            (*result)[i] = str1[i];
        
        for (i = 0 ; str2[i] != '\0' ; i++)
            (*result)[j+i] = str2[i];

        (*result)[j+i] = '\0';
    }
}

void cstrappend(char* dst, const char* str1)
{
    size_t dst_len = strlen(dst);
    size_t i = 0;

    if (dst) {      
        for (i = 0 ; str1[i] != '\0' ; i++)
            (dst)[dst_len+i] = str1[i];

        (dst)[dst_len+i] = '\0';
    }
}

/* alloc and duplicate string */
char* cstrdup(const char* original) {
    char* duplicate = malloc(strlen(original) + 1);
    if (!duplicate) return NULL;

    strcpy(duplicate, original);
    return duplicate;
}

// From SDL2 source code
char* getBasePath(void) {
    typedef DWORD (WINAPI *GetModuleFileNameExA_t)(HANDLE, HMODULE, LPSTR, DWORD);
    GetModuleFileNameExA_t pGetModuleFileNameExA;
    DWORD buflen = 128;
    CHAR* path = NULL;
    HANDLE psapi = LoadLibrary("psapi.dll");
    DWORD len = 0;
    DWORD i;

    if (!psapi) {
        ERR("Couldn't load psapi.dll");
        return NULL;
    }

    pGetModuleFileNameExA = (GetModuleFileNameExA_t)GetProcAddress(psapi, "GetModuleFileNameExA");
    if (!pGetModuleFileNameExA) {
        ERR("Couldn't find GetModuleFileNameExA");
        FreeLibrary(psapi);
        return NULL;
    }

    for (;;) {
        void *ptr = realloc(path, buflen * sizeof(CHAR));
        if (!ptr) {
            free(path);
            FreeLibrary(psapi);
            return NULL;
        }

        path = (CHAR*)ptr;

        len = pGetModuleFileNameExA(GetCurrentProcess(), NULL, path, buflen);
        if (len != buflen) {
            break;
        }

        /* buffer too small? Try again. */
        buflen *= 2;
    }

    FreeLibrary(psapi);

    if (len == 0) {
        free(path);
        ERR("Couldn't locate our .exe");
        return NULL;
    }

    for (i = len-1; i > 0; i--) {
        if (path[i] == '\\') {
            break;
        }
    }
    path[i+1] = '\0';  /* chop off filename. */

    return path;
}


// From BSD Source Code
char* strnstr(const char *s, const char *find, size_t slen) {
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

void printf_guid(GUID guid) {
    printf("Guid = {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n", 
      guid.Data1, guid.Data2, guid.Data3, 
      guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
      guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

#endif