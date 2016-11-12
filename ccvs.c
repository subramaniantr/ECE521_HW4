#include "macros.h"
#include "defs.h"
#include "ccvs.h"
#include "sparse/spMatrix.h"

void makeHsrc(Hsrc, numHsrc, buf)
ccvs *Hsrc[];
int numHsrc;
char *buf;
{
    ccvs *inst;
    int j, nodeA, nodeB, cbranchNum, branchNum, atoi();
    char name[MAXFIELD], cname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    branchNum = getMappedBranch(name);
    j = getNextField(buf, node, j);
    nodeA = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, cname, j);
    cbranchNum = getMappedBranch(cname);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(ccvs, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->branchNum = branchNum ;
    inst->cbranchNum = cbranchNum ;
    inst->Rt = value;
    Hsrc[numHsrc] = inst;
}

void printHsrc(Hsrc, numHsrc)
ccvs *Hsrc[];
int numHsrc;
{
    int i;
    ccvs *inst;
    for(i = 1; i <= numHsrc; i++) {
	inst = Hsrc[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], BranchArray[inst->cbranchNum],  inst->Rt);
    }
}

void setupHsrc(Matrix, Hsrc, numHsrc)
char *Matrix;
ccvs *Hsrc[];
int numHsrc;
{
    int i, p,n,bC,b;
    ccvs *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1; i <= numHsrc; i++) {
	inst = Hsrc[i];
	inst->branchNum += NumNodes;
	inst->cbranchNum += NumNodes;
	p  = inst->pNode;
	n  = inst->nNode;
	bC = inst->cbranchNum;
	b  = inst->branchNum;
	inst->ppb  = spGetElement(Matrix, p, b);
	inst->pnb  = spGetElement(Matrix, n, b);
	inst->pbp  = spGetElement(Matrix, b, p);
	inst->pbn  = spGetElement(Matrix, b, n);
	inst->pbbC  = spGetElement(Matrix, b, bC);
    }
}

void loadHsrc(Matrix, Rhs, Hsrc, numHsrc)
char *Matrix;
double *Rhs;
ccvs *Hsrc[];
int numHsrc;
{
    int i;
    double Rt ;
    ccvs *inst;
    /* stamp H source*/
    for(i = 1; i <= numHsrc; i++) {
        inst = Hsrc[i];
        Rt = inst->Rt;
        //KCL for pNode & nNode
 	*(inst->ppb) += 1; //I leaving pNode
 	*(inst->pnb) -= 1; //I entering nNode
        //BCE for the CCVS : Vp-Vn-I*Rt = 0
 	*(inst->pbp) += 1;
 	*(inst->pbn) -= 1;
 	*(inst->pbbC) -= Rt;
    
    }
}
