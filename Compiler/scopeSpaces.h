#ifndef SCOPESPACES_H
#define	SCOPESPACES_H

enum scopespace_t {
    programvar, functionlocal, formalarg
};

enum scopespace_t currscopespace(void);
unsigned int currscopeoffset(void);
void inccurrscopeoffset(void);
void enterscopespace(void);
void exitscopespace(void);
void restorecurrscopeoffset(unsigned int val);
void resetcurrscopeoffset(void);

#endif	/* SCOPESPACES_H */

