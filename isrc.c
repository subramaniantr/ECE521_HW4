#include "macros.h"
#include "defs.h"
#include "isrc.h"
#include "sparse/spMatrix.h"

void makeIsrc(Isrc, numIsrc, buf)
isource *Isrc[];
int numIsrc;
char *buf;
{
    isource *inst;
    int j, nodeA, nodeB, atoi();
    char name[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeA = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(isource, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->current = value;
    Isrc[numIsrc] = inst;
}

void printIsrc(Isrc, numIsrc)
isource *Isrc[];
int numIsrc;
{
    int i;
    isource *inst;
    for(i = 1; i <= numIsrc; i++) {
	inst = Isrc[i];
	printf("%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->current);
    }
}

void setupIsrc(Matrix, Rhs, Isrc, numIsrc)
char *Matrix;
double *Rhs;
isource *Isrc[];
int numIsrc;
{
    int i,n1,n2;
    isource *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numIsrc; i++) {
	inst = Isrc[i];
	n1 = inst->pNode;
	n2 = inst->nNode;
	/* setup matrix and pointers */
	inst->prhsn1 = Rhs+n1;
	inst->prhsn2 = Rhs+n2;
    }
}

void loadIsrc(Matrix, Rhs, Isrc, numIsrc)
char *Matrix;
double *Rhs;
isource *Isrc[];
int numIsrc;
{
    int i ;
    isource *inst;
    double current;

    /* stamp current source*/
    for(i = 1; i <= numIsrc; i++) {
	inst = Isrc[i];
	current = Isrc[i]->current;

	*(inst->prhsn1) -= current;
	*(inst->prhsn2) += current;
    }
}
