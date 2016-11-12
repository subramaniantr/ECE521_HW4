#include "macros.h"
#include "defs.h"
#include "vsrc.h"
#include "sparse/spMatrix.h"

void makeVsrc(Vsrc, numVsrc, buf)
vsource *Vsrc[];
int numVsrc;
char *buf;
{
    vsource *inst;
    int j, nodeA, nodeB, branchNum, atoi();
    char name[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

	/* incrementing the number of branches */

    j = 0;	
    j = getNextField(buf, name, j);
    branchNum = getMappedBranch(name);
    j = getNextField(buf, node, j);
    nodeA = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(vsource, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->branchNum = branchNum ;
    inst->voltage = value;
    Vsrc[numVsrc] = inst;
}

void printVsrc(Vsrc, numVsrc)
vsource *Vsrc[];
int numVsrc;
{
    int i;
    vsource *inst;

    for(i = 1; i <= numVsrc; i++) {
	inst = Vsrc[i];
	printf("%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->voltage);
    }
}

void setupVsrc(Matrix, Rhs, Vsrc, numVsrc)
char *Matrix;
double *Rhs;
vsource *Vsrc[];
int numVsrc;
{
    int i, p,n,b;
    vsource *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numVsrc; i++) {
	inst = Vsrc[i];
	inst->branchNum += NumNodes;
	p = inst->pNode;
	n = inst->nNode;
        b = inst->branchNum;
	/* setup matrix and pointers */
	inst->ppb = spGetElement(Matrix, p, b);
	inst->pnb = spGetElement(Matrix, n, b);
	inst->pbp = spGetElement(Matrix, b, p);
	inst->pbn = spGetElement(Matrix, b, n);
	inst->prhsb = Rhs+b ;
        
    }
}

void loadVsrc(Matrix, Rhs, Vsrc, numVsrc)
char *Matrix;
double *Rhs;
vsource *Vsrc[];
int numVsrc;
{
    int i;
    vsource *inst;
    double voltage;
	
    /* stamp voltage source*/
    for(i = 1; i <= numVsrc; i++) {
	inst = Vsrc[i];
	voltage = inst->voltage;

	*(inst->ppb) += 1; 
	*(inst->pnb) -= 1; 
	*(inst->pbp) += 1; 
	*(inst->pbn) -= 1; 
	*(inst->prhsb) += voltage;
    }
}
