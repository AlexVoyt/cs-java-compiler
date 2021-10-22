#ifndef TERM_H
#define TERM_H

#define TERM_BLACK     "\033[30m"
#define TERM_RED       "\033[31m"
#define TERM_DEFAULT   "\033[39m"
#define SET_TERM_COLOR(Color) printf((Color))
#define RESET_TERM_COLOR() SET_TERM_COLOR(TERM_DEFAULT)

#endif /* TERM_H */
