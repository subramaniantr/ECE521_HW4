#include "macros.h"
#include "defs.h"
#include "vcvs.h"
#include "sparse/spMatrix.h"

void makeEsrc(Esrc, numEsrc, buf)
vcvs *Esrc[];
int numEsrc;
char *buf;
{
    vcvs *inst;
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

    inst = CALLOC(vcvs, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->pCNode = nodeC;
    inst->nCNode = nodeD;
    inst->branchNum = branchNum ;
    inst->gain = value;
    Esrc[numEsrc] = inst;
}

void printEsrc(Esrc, numEsrc)
vcvs *Esrc[];
int numEsrc;
{
    int i;
    vcvs *inst;
    for(i = 1; i <= numEsrc; i++) {
	inst = Esrc[i];
	printf("%s\t%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], NodeArray[inst->pCNode], NodeArray[inst->nCNode], inst->gain);
    }
}

void setupEsrc(Matrix, Esrc, numEsrc)
char *Matrix;
vcvs *Esrc[];
int numEsrc;
{
    int i,p,n,pC,nC,b;
    vcvs *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1; i <= numEsrc; i++) {
	inst = Esrc[i];
	inst->branchNum += NumNodes;
        b = inst->branchNum;
        p = inst->pNode;
        n = inst->nNode;
        pC = inst->pCNode;
        nC = inst->nCNode;
	/* setup matrix and pointers */
	inst->ppb  = spGetElement(Matrix, p, b);
	inst->pnb  = spGetElement(Matrix, n, b);
	inst->pbp  = spGetElement(Matrix, b, p);
	inst->pbn  = spGetElement(Matrix, b, n);
	inst->pbpC = spGetElement(Matrix, b, pC);
	inst->pbnC = spGetElement(Matrix, b, nC);
    }
}

void loadEsrc(Matrix, Rhs, Esrc, numEsrc)
char *Matrix;
double *Rhs;
vcvs *Esrc[];
int numEsrc;
{
    int i;
    double gain ;
    vcvs *inst;
    /* stamp E source*/
    for(i = 1; i <= numEsrc; i++) {
        inst = Esrc[i];
        gain   = Esrc[i]->gain;
        
        //KCL for pNode & nNode
 	*(inst->ppb) += 1; //I leaving pNode
 	*(inst->pnb) -= 1;
        //BCE for VCVS : Vp-Vn-gain*(Vpc-Vnc)=0
 	*(inst->pbp)  += 1;
 	*(inst->pbn)  -= 1;
 	*(inst->pbpC) -= gain;
 	*(inst->pbnC) += gain;
    }
}
