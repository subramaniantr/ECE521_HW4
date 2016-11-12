#include "macros.h"
#include "defs.h"
#include "opamp.h"
#include "sparse/spMatrix.h"

void makeOp(Op, numOp, buf)
opamp *Op[];
int numOp;
char *buf;
{
    opamp *inst;
    int j, nodeA, nodeB, nodeC, branchNum, atoi();
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
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(opamp, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeA;
    inst->nNode = nodeB;
    inst->oNode = nodeC;
    inst->branchNum = branchNum ;
    inst->A = value;
    Op[numOp] = inst;
}

void printOp(Op, numOp)
opamp *Op[];
int numOp;
{
    int i;
    opamp *inst;
    for(i = 1; i <= numOp; i++) {
	inst = Op[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->oNode], NodeArray[inst->pNode], NodeArray[inst->nNode], inst->A);
    }
}

void setupOp(Matrix,Op, numOp)
char *Matrix;
opamp *Op[];
int numOp;
{
    int i, p, n, o, b;
    opamp *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1; i <= numOp; i++) {
	inst = Op[i];
	inst->branchNum += NumNodes;
        p = inst->pNode;
        n = inst->nNode;
        o = inst->oNode ;
        b = inst->branchNum;
	/* setup matrix and pointers */
	inst->pbp  = spGetElement(Matrix, b, p);
	inst->pbn  = spGetElement(Matrix, b, n);
	inst->pbo   = spGetElement(Matrix,b, o);
	inst->pob   = spGetElement(Matrix, o, b);
    }
}

void loadOp(Matrix, Rhs, Op, numOp)
char *Matrix;
double *Rhs;
opamp *Op[];
int numOp;
{
    int i;
    double A ;
    opamp *inst;
    /* stamp N source*/
    for(i = 1; i <= numOp; i++) {
        inst = Op[i];
        A    = Op[i]->A;
        //KCL for output Node
 	*(inst->pob) += 1;
        //BCE for  Opamp : A(Vp-Vn)+ Vo=0
 	*(inst->pbp) += A;
 	*(inst->pbn) -= A;
 	*(inst->pbo) += 1;
    }
}
