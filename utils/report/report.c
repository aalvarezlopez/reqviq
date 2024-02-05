#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "requirements.h"
#include "databaseconnect.h"

void writeblockheader(char *title, char * desc, FILE *file)
{
    fprintf(file,"\\section{%s}\n\n", title);
    fputs( "\\begin{tabular}{p{0.3\\textwidth}p{0.3\\textwidth}p{0.3\\textwidth}}\n", file);
    fprintf(file, "\\begin{projectreqpass}{%s} %s\\end{projectreqpass}&", title, desc);
}

void writesysrequirement(char *title, char * desc, FILE * file)
{
    fprintf(file, "\\begin{systemreqpass}{%s} %s\\end{systemreqpass}&", title, desc);
}

void writel3requirement(char *title, char * desc, bool isfirst,  FILE * file)
{
    if( isfirst == false ){
        fputs("&&", file);
    }
    fprintf(file, "\\begin{softwarereqpass}{%s} %s\\end{softwarereqpass}\\\\\n", title, desc);
}

void writeemptysysreq(FILE * file)
{
    fputs("& \\\\\n", file);
}

void writeemptyl3req(FILE * file)
{
    fputs(" \\\\\n", file);
}

void writeblocktail(FILE * file)
{
    fputs("\\end{tabular}\n\n", file);
}

int main(void)
{
    char title[1024][REQ_TITLE_LEN];/*{{{*/
    char description[1024][REQ_DESC_LEN];
    uint16_t uid[1024];
    uint16_t count;
    FILE * template;
    FILE * report;
    char buffer[5196];
    size_t len = 0;
    ssize_t read;
    char *ptr;

    template = fopen("./_static/template.tex", "r");
    report  = fopen("./report.tex", "w");

    if (template == NULL){
        exit(EXIT_FAILURE);
    }

    fread(buffer, sizeof(buffer), 1, template);
    ptr = strstr( buffer, "\%fill with requirements");
    fwrite(buffer, ptr - buffer, 1, report);
    printf("FOUND!!! %lx %d\n\r", ptr, ptr -buffer - 1);
    fclose( template );

    databaseconnect_start();
    requirement_init(0);
    count = requirement_getrequirement(title, description, uid, 1, 0);/*}}}*/
    for( uint16_t i = 0; i < count; i++){
        char titlesys[1024][REQ_TITLE_LEN];
        char descriptionsys[1024][REQ_DESC_LEN];
        uint16_t uidsys[1024];
        uint16_t syscount = requirement_getrequirement(titlesys, descriptionsys, uidsys, 2, uid[i]);
        writeblockheader(title[i], description[i], report);
        for( uint16_t j = 0; j < syscount; j++){
            char titlel3[1024][REQ_TITLE_LEN];
            char descriptionl3[1024][REQ_DESC_LEN];
            uint16_t l3count = requirement_getrequirement(titlel3, descriptionl3, NULL, 3, uidsys[j]);
            writesysrequirement( titlesys[j], descriptionsys[j], report);
            for( uint16_t z = 0; z < l3count; z++){
                writel3requirement( titlel3[z], descriptionl3[z], z==0, report);
            }
            if( l3count == 0 ){
                writeemptyl3req(report);
            }
        }
        if( syscount == 0 ){
            writeemptysysreq(report);
        }
        writeblocktail(report);
    }
    databaseconnect_close();
    fwrite(ptr + strlen("fill with requirements") + 2,
        strlen(ptr) - strlen("fill with requirements") - 2, 1, report);
    fclose( report );
    return EXIT_SUCCESS;
}
