#include "macros.h"
#include "defs.h"
#include "tf.h"
#include "sparse/spMatrix.h"

void makeTf(Tf, numTf, buf)
transformer *Tf[];
int numTf;
char *buf;
{
    transformer *inst;
    int j, nodeA, nodeB, nodeC, nodeD, branchNum, atoi();
    char name[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    branchNum = getMappedBranch(name);
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

    inst = CALLOC(transformer, 1);
    inst->name = (char *)strdup(name);
    inst->pCNode = nodeA;
    inst->nCNode = nodeB;
    inst->pNode = nodeC;
    inst->nNode = nodeD;
    inst->branchNum = branchNum ;
    inst->n = value;
    Tf[numTf] = inst;
}

void printTf(Tf, numTf)
transformer *Tf[];
int numTf;
{
    int i;
    transformer *inst;
    for(i = 1; i <= numTf; i++) {
	inst = Tf[i];
	printf("%s\t%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pCNode], NodeArray[inst->nCNode], NodeArray[inst->pNode], NodeArray[inst->nNode], inst->n);
    }
}

void setupTf(Matrix, Tf, numTf)
char *Matrix;
transformer *Tf[];
int numTf;
{
    int i,p,n,pC,nC,b;
    transformer *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1; i <= numTf; i++) {
	inst = Tf[i];
	inst->branchNum += NumNodes;
        b  = inst->branchNum;
        p  = inst->pNode;
        n  = inst->nNode;
        pC = inst->pCNode;
        nC = inst->nCNode;
	/* setup matrix and pointers */
	inst->ppCb  = spGetElement(Matrix, pC, b);
	inst->pnCb  = spGetElement(Matrix, nC, b);
	inst->ppb   = spGetElement(Matrix, p, b);
	inst->pnb   = spGetElement(Matrix, n, b);
	inst->pbpC  = spGetElement(Matrix, b, pC);
	inst->pbnC  = spGetElement(Matrix, b, nC);
	inst->pbp   = spGetElement(Matrix, b, p);
	inst->pbn   = spGetElement(Matrix, b, n);
    }
}

void loadTf(Matrix, Rhs, Tf, numTf)
char *Matrix;
double *Rhs;
transformer *Tf[];
int numTf;
{
    int i;
    double n ;
    transformer *inst;
    /* stamp N source*/
    for(i = 1; i <= numTf; i++) {
        inst = Tf[i];
        n   = Tf[i]->n;
        //KCL for p2Node,n2Node,p1Node and n1Node
 	*(inst->ppCb) += 1;
 	*(inst->pnCb) -= 1;
 	*(inst->ppb) -= n; 
 	*(inst->pnb) += n;
        //BCE for Transformer : (V1p-V1n)-n*(V2p-V2n)=0
 	*(inst->pbpC) += 1;
 	*(inst->pbnC) -= 1;
 	*(inst->pbp) -= n;
 	*(inst->pbn) += n;
    }
}
