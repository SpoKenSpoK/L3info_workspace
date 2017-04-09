#ifndef GENERATIONX86_H
#define GENERATIONX86_H

#define MAX_BSS 1000

int sectionBSS;
int sectionDATA;
int sectionTEXT;

char* outPUT_filename;
FILE* outPUT;

void defaultRoute(char* );

char* OReadFile(char* );
void IWriteFile(char*);
void IWriteFileFrom(char*, int);

void X86_INIT();
void X86_CLOSE();
void X86_include();
void X86_data();

void X86_bss(char* name, char* bytes, int mult);

void X86_text();
void X86_start();
void X86_main();

void X86_instrWrite();

#endif
