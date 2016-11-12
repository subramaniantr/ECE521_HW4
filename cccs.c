#include "macros.h"
#include "defs.h"
#include "cccs.h"
#include "sparse/spMatrix.h"

void makeFsrc(Fsrc, numFsrc, buf)
cccs *Fsrc[];
int numFsrc;
char *buf;
{
    cccs *inst;
    int j, nodeA, nodeB, cbranchNum, atoi();
    char name[MAXFIELD], cname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeA = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, cname, j);
    cbranchNum = getMappedBranch(cname);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(cccs, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->cbranchNum = cbranchNum ;
    inst->gain = value;
    Fsrc[numFsrc] = inst;
}

void printFsrc(Fsrc, numFsrc)
cccs *Fsrc[];
int numFsrc;
{
    int i;
    cccs *inst;
    for(i = 1; i <= numFsrc; i++) {
	inst = Fsrc[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], BranchArray[inst->cbranchNum], inst->gain);
    }
}

void setupFsrc(Matrix, Fsrc, numFsrc)
char *Matrix;
cccs *Fsrc[];
int numFsrc;
{
    int i,p,n,bC;
    cccs *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1; i <= numFsrc; i++) {
	inst = Fsrc[i];
      	inst->cbranchNum += NumNodes;
        bC = inst->cbranchNum;
        p = inst->pNode;
        n = inst->nNode;
	inst->ppbC  = spGetElement(Matrix, p, bC);
	inst->pnbC  = spGetElement(Matrix, n, bC);
    }
}

void loadFsrc(Matrix, Rhs, Fsrc, numFsrc)
char *Matrix;
cccs *Fsrc[];
double *Rhs;
int numFsrc;
{
    int i;
    double gain ;
    cccs *inst;
    /* stamp F source*/
    for(i = 1; i <= numFsrc; i++) {
     inst = Fsrc[i];
     gain = Fsrc[i]->gain;
     //KCL for pNode & nNode 
     *(inst->ppbC) += gain; //I leaving pNode
     *(inst->pnbC) -= gain;
    }
}
