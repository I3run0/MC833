#include "command_line_interface.h"
#define CMD_LEVEL 2
#define CMDS_SZ 10
#define CMD_QTT 3
#define LM_SUB_CMD_QTT 2

char AV_CMD[CMD_QTT][CMDS_SZ] = {
    "-ns", /* register a new song, should be provide six strings seperated by columns*/
    "-rs", /*to Delete the select song, should be provide song name and artist*/
    "-ls" /*List songs*/
};


/* LM command can be used with both -f and -it in the same time*/
char LM_SUB_AV_CMD[LM_SUB_CMD_QTT][CMDS_SZ] = {
    "-f", /*Filters to select song*/
    "-it" /*Info type tp to be showed*/
};


bool vldt_cmd(int argc, char* argv[]) {
    int i;
    char cmd_tp;
    
    if (argc < 2) return false;
    
    strcmp(cmd_tp, AV_CMD[0]);

    if (strcmp(cmd_tp, AV_CMD[0])) {
        if (argc != 8) return NULL;

    } (strcmp(cmd_tp, AV_CMD[1])) {
        return (argc != 4) return NULL;

    } (strcmp(cmd_tp, AV_CMD[2])) {
        return ();
    }

    return true; 
}