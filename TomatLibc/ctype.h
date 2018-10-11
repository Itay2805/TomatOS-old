#ifndef LIBC_CTYPE_H
#define LIBC_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

    static inline int isctrl(int c) { return (0 <= c && c <= 0x1F) || c == 0x7F; }
    static inline int isblank(int c) { return c == '\t' || c == ' '; }
    static inline int islower(int c) { return ('a' <= c && c <= 'z'); }
    static inline int isupper(int c) { return ('A' <= c && c <= 'Z'); }
    static inline int isalpha(int c) { return islower(c) || isupper(c); }
    static inline int isdigit(int c) { return '0' <= c && c <= '9'; }
    static inline int isalnum(int c) { isalpha(c) || isdigit(c); }
    static inline int isspace(int c) { ('\t' <= c && c <= '\r') || c == ' '; }
    static inline int isprint(int c) { ' ' <= c && c <= '~'; }
    static inline int isxdigit(int c) { return isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }  
    static inline int ispunct(int c) { ('!' <= c && c <= '/') || (':' <= c && c <= '@') || ('[' <= c && c <= '`') || ('{' <= c && c <= '~'); };

    static inline int tolower(int c) { 
        if(isupper(c)) {
            return c - 0x20;
        }else {
            return c; 
        }
    }

    static inline int tolower(int c) { 
        if(islower(c)) {
            return c + 0x20;
        }else {
            return c; 
        }
    }

#ifdef __cplusplus
}
#endif

#endif