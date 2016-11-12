#include "macros.h"
#include "defs.h"
#include "gyro.h"
#include "sparse/spMatrix.h"

void makeGyro(Gyro, numGyro, buf)
gyrator *Gyro[];
int numGyro;
char *buf;
{
    gyrator *inst;
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

    inst = CALLOC(gyrator, 1);
    inst->name = (char *)strdup(name);
    inst->pCNode = nodeA;
    inst->nCNode = nodeB;
    inst->pNode = nodeC;
    inst->nNode = nodeD;
    inst->g = value;
    Gyro[numGyro] = inst;
}

void printGyro(Gyro, numGyro)
gyrator *Gyro[];
int numGyro;
{
    int i;
    gyrator *inst;
    for(i = 1; i <= numGyro; i++) {
	inst = Gyro[i];
	printf("%s\t%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pCNode], NodeArray[inst->nCNode], NodeArray[inst->pNode], NodeArray[inst->nNode], inst->g);
    }
}

void setupGyro(Matrix,Gyro, numGyro)
char *Matrix;
gyrator *Gyro[];
int numGyro;
{
    int i,p,n,pC,nC;
    gyrator *inst;

    /* do any preprocessing steps here */
   //Pushing Branch number columns after the Node numbers
    for(i = 1;i <= numGyro; i++) {
      inst = Gyro[i];
      p  = inst->pNode;
      n  = inst->nNode;
      pC = inst->pCNode;
      nC = inst->nCNode;
	/* setup matrix and pointers */
	inst->ppCp  = spGetElement(Matrix, pC, p);
	inst->ppCn  = spGetElement(Matrix, pC, n);
	inst->pnCp  = spGetElement(Matrix, nC, p);
	inst->pnCn  = spGetElement(Matrix, nC, n);
	inst->pppC  = spGetElement(Matrix, p, pC);
	inst->ppnC  = spGetElement(Matrix, p, nC);
	inst->pnpC  = spGetElement(Matrix, n, pC);
	inst->pnnC  = spGetElement(Matrix, n, nC);
 
    }
}

void loadGyro(Matrix, Rhs, Gyro, numGyro)
char *Matrix;
double *Rhs;
gyrator *Gyro[];
int numGyro;
{
    int i;
    gyrator *inst;
    double g ;
    /* stamp T source*/
    for(i = 1; i <= numGyro; i++) {
        inst = Gyro[i];
        g    = Gyro[i]->g;
        //KCL for p2Node,n2Node,p1Node and n1Node
 	*(inst->ppCp) -= g; 
 	*(inst->ppCn) += g;
 	*(inst->pnCp) += g;
 	*(inst->pnCn) -= g;
 	*(inst->pppC) += g;
 	*(inst->ppnC) -= g;
 	*(inst->pnpC) -= g;
 	*(inst->pnnC) += g;
    }
}
