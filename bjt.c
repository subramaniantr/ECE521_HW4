#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "bjt.h"

double pnjlim();

void makeBjt(Bjt, numBjt, buf)
bjt *Bjt[];
int numBjt;
char *buf;
{
    bjt *inst;
    int j, nodeC, nodeB, nodeE, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeC = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeE = getMappedNode(node);
    j = getNextField(buf, mname, j);

    inst = CALLOC(bjt, 1);
    inst->name = (char *)strdup(name);
    inst->cNode = nodeC;
    inst->bNode = nodeB;
    inst->eNode = nodeE;
    inst->mname = (char *)strdup(mname);
    Bjt[numBjt] = inst;
}

void printBjt(Bjt, numBjt)
bjt *Bjt[];
int numBjt;
{
    int i;
    bjt *inst;

    for(i = 1; i <= numBjt; i++) {
	inst = Bjt[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->cNode], NodeArray[inst->bNode], NodeArray[inst->eNode], inst->mname);
    }
}




void setupBjt(Matrix, Rhs, Bjt, numBjt)
char *Matrix;
double *Rhs;
bjt *Bjt[];
int numBjt;
{
    int i,c,b,e;
    bjt *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numBjt; i++) {
	inst = Bjt[i];
//	inst->branchNum += NumNodes;
	c = inst->cNode;
	b = inst->bNode;
	e = inst->eNode;
	/* setup matrix and pointers */
	inst->pcc = spGetElement(Matrix, c, c);
	inst->pcb = spGetElement(Matrix, c, b);
	inst->pce = spGetElement(Matrix, c, e);
	inst->pbc = spGetElement(Matrix, b, c);
	inst->pbb = spGetElement(Matrix, b, b);
	inst->pbe = spGetElement(Matrix, b, e);
	inst->pec = spGetElement(Matrix, e, c);
	inst->peb = spGetElement(Matrix, e, b);
	inst->pee = spGetElement(Matrix, e, e);
        
    }
}

void loadBjt(Matrix, Rhs, Bjt, numBjt, Xk,icheck)
char *Matrix;
double *Rhs;
bjt *Bjt[];
int numBjt;
double* Xk;
int *icheck;
{
  int i, c, b, e;
  bjt *inst;
  double Ic, Ib, Ie;
  double Ick, Ibk, Iek;
  double Vce, Vbe, Vbc,Vcrit;
  double gmF, gmR;
  double gmcc, gmcb, gmce;
  double gmbc, gmbb, gmbe;
  double gmec, gmeb, gmee;

  double Is, Vt, alphaF, alphaR;

  for(i = 1; i <= numBjt; i++) {
    inst = Bjt[i];
    c = inst->cNode;
    b = inst->bNode;
    e = inst->eNode;

///////////Getting Vbe, Vce from Vb, Ve and Vc///////////////
    Vbe = Xk[b]-Xk[e];
    Vbc = Xk[b]-Xk[c];
    Vce = Xk[c]-Xk[e];
////////Hard coding Is, Vt, alphaF and alphaR Values//////////////////
      Is = 1.0e-16;
      Vt = 0.0258;
      alphaF=0.99;
      alphaR=0.01;
/////////////APPLYING ONE TIME INITIAL CONDITION FOR CONVERGENCE//////////////////////
   if(iter_counter == 0){
      Vbe = 0.7;
      Vce = 0.2;
      Vbc = 1.0;
}

////////////////////////////////APPLYING LIMITING////////////////////////////////
   Vcrit = Vt*log(Vt/(Is*sqrt(2)));
  Vbe= pnjlim(Vbe,inst->Vbe_old,Vt,Vcrit, icheck);
  Vbc= pnjlim(Vbc,inst->Vbc_old,Vt,Vcrit, icheck);

///////////////////////////////ASSIGNING V_OLD///////////////////////////////////
   inst->Vbe_old = Vbe;
   inst->Vbc_old = Vbc;

     
///////////////Ic, Ie, Ib, gmF and gmR etc. Calculation///////////////////

     Ic = Is*((exp(Vbe/Vt)-1)-(exp(Vbc/Vt)-1)/alphaR);
     Ie = Is*((exp(Vbc/Vt)-1)-(exp(Vbe/Vt)-1)/alphaF);
     Ib = -(Ic+Ie);

     gmF = Is*exp(Vbe/Vt)/Vt;
     gmR = Is*exp(Vbc/Vt)/Vt;
     
     gmcc = gmR/alphaR;
     gmcb = gmF - gmR/alphaR;
     gmce = -gmF;

     gmec = -gmR;
     gmeb = -gmF/alphaF + gmR;
     gmee = gmF/alphaF;

     gmbc = -(gmcc+gmec);
     gmbb = -(gmcb+gmeb);
     gmbe = -(gmce+gmee);

////////////////////Ik Calculation/////////////////////

     Ick = Ic-gmcc*Xk[c]-gmce*Xk[e]-gmcb*Xk[b];
     Iek = Ie-gmec*Xk[c]-gmee*Xk[e]-gmeb*Xk[b];
     Ibk = -(Ick+Iek);

    *(inst->pcc) += gmcc;
    *(inst->pcb) += gmcb;
    *(inst->pce) += gmce;
    *(inst->pbc) += gmbc;
    *(inst->pbb) += gmbb;
    *(inst->pbe) += gmbe;
    *(inst->pec) += gmec;
    *(inst->peb) += gmeb;
    *(inst->pee) += gmee;
    
    Rhs[c] -= Ick;
    Rhs[b] -= Ibk;
    Rhs[e] -= Iek;
  }
}
