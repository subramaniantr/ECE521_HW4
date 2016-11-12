
#include <stdio.h>
#include <strings.h>
#include "macros.h"
#include "defs.h"
#include "sparse/spMatrix.h"

int getNextField(input, field, start)
char *input, *field;
int start;
{
    int i, j;
    for(i=0; i < MAXFIELD; i++, j++) {
	field[i] = '\0';
    }
    for(i = 0, j = start; i < MAXFIELD; i++, j++) {
	if (input[j] == ' ' OR input[j] == '\n') break;
	field[i] = input[j];
    }
    /* strip trailing blanks */
    while( input[j] == ' ') j++;
    return( j );
}

/* map a nodename to an integer */
int getMappedNode(nodeName)
char *nodeName;
{
    int i;
    for(i = 0; i <= NumNodes; i++) {
	if(!strcmp(NodeArray[i], nodeName)) return( i );
    }
    /* node doesn't exist in NodeArray - so insert */
    NumNodes++;
    strcpy(NodeArray[NumNodes],  nodeName);
    return(NumNodes);
}

/* map a branch name to an integer */
int getMappedBranch(branchName)
char *branchName;
{
    int i;
    for(i = 0; i <= NumBranches; i++) {
	if(!strcmp(BranchArray[i], branchName)) return( i );
    }
    /* branch doesn't exist in BranchArray - so insert */
    NumBranches++;
    strcpy(BranchArray[NumBranches],  branchName);
    return(NumBranches);
}

BOOLEAN
foundError( error )
int error;
{
    BOOLEAN matrixError;
    switch( error ) {
	case spSMALL_PIVOT:
	    printf( "\n *****Error in Decomp: SMALL_PIVOT" );
	    matrixError = TRUE;
	    break;
	case spPANIC:
	    printf( "\n *****Error in Decomp: RANGE" );
	    matrixError = TRUE;
	    break;
	case spSINGULAR:
	    printf( "\n *****Error in Decomp: SINGULAR" );
	    matrixError = TRUE;
	    break;
	case spZERO_DIAG:
	    printf( "\n *****Error in Decomp: ZERO PIVOT" );
	    matrixError = TRUE;
	    break;
	case spNO_MEMORY:
	    printf( "\n *****Error in Decomp: NO_MEMORY" );
	    matrixError = TRUE;
	    break;
	default:
	    matrixError = FALSE;
	    break;
    }
    return( matrixError );
}
