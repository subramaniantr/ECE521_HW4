#include "macros.h"
#include "defs.h"
#include "vccs.h"
#include "sparse/spMatrix.h"

void makeGsrc(Gsrc, numGsrc, buf)
vccs *Gsrc[];
int numGsrc;
char *buf;
{
    vccs *inst;
    int j, nodeA, nodeB, nodeC, nodeD, atoi();
    char name[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeA = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeC = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeD = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(vccs, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->pCNode = nodeC;
    inst->nCNode = nodeD;
    inst->Gm = value;
    Gsrc[numGsrc] = inst;
}

void printGsrc(Gsrc, numGsrc)
vccs *Gsrc[];
int numGsrc;
{
    int i;
    vccs *inst;
    for(i = 1; i <= numGsrc; i++) {
	inst = Gsrc[i];
	printf("%s\t%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], NodeArray[inst->pCNode], NodeArray[inst->nCNode], inst->Gm);
    }
}

void setupGsrc(Matrix, Gsrc, numGsrc)
char *Matrix;
vccs *Gsrc[];
int numGsrc;
{
    int i, p,n,pC,nC;
    vccs *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numGsrc; i++) {
    inst = Gsrc[i];
    p  = inst->pNode;
    n  = inst->nNode;
    pC = inst->pCNode;
    nC = inst->nCNode;
	/* setup matrix and pointers */
	inst->pppC = spGetElement(Matrix, p, pC);
	inst->ppnC = spGetElement(Matrix, p, nC);
	inst->pnpC = spGetElement(Matrix, n, pC);
	inst->pnnC = spGetElement(Matrix, n, nC);
    }
}

void loadGsrc(Matrix, Rhs, Gsrc, numGsrc)
char *Matrix;
double *Rhs;
vccs *Gsrc[];
int numGsrc;
{
    int i;
    double Gm;
    vccs *inst;
    /* load G source*/
    for(i = 1; i <= numGsrc; i++) {
	inst = Gsrc[i];
	Gm = Gsrc[i]->Gm;
	*(inst->pppC) += Gm;
	*(inst->ppnC) -= Gm;
	*(inst->pnpC) -= Gm;
	*(inst->pnnC) += Gm;
    }
}
