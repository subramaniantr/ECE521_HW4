
#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "nonlincap.h"



void makeNonLinCap(NonLinCap, numNonLinCap, buf)
nonlincap *NonLinCap[];
int numNonLinCap;
char *buf;
{
    nonlincap *inst;
    int j, nodeP, nodeN, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeP = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeN = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(nonlincap, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeP;
    inst->nNode = nodeN;
    inst->value = value;
    NonLinCap[numNonLinCap] = inst;
}

void printNonLinCap(NonLinCap, numNonLinCap)
nonlincap *NonLinCap[];
int numNonLinCap;
{
    int i;
    nonlincap *inst;

    for(i = 1; i <= numNonLinCap; i++) {
	inst = NonLinCap[i];
	printf("%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->value);
    }
}




void setupNonLinCap(Matrix, Rhs, NonLinCap, numNonLinCap)
char *Matrix;
double *Rhs;
nonlincap *NonLinCap[];
int numNonLinCap;
{
    int i, p,n;
    nonlincap *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numNonLinCap; i++) {
	inst = NonLinCap[i];
//	inst->branchNum += NumNodes;
	p = inst->pNode;
	n = inst->nNode;
	/* setup matrix and pointers */
	inst->ppp = spGetElement(Matrix, p, p);
	inst->ppn = spGetElement(Matrix, p, n);
	inst->pnp = spGetElement(Matrix, n, p);
	inst->pnn = spGetElement(Matrix, n, n);
        

    }
}


void loadNonLinCap(Matrix, Rhs, NonLinCap, numNonLinCap, Xk,h ,time_step_count)
char *Matrix;
double *Rhs;
nonlincap *NonLinCap[];
int numNonLinCap;
double* Xk;
double h;
int time_step_count;
{
  int i, p, n;
  nonlincap *inst;
  double Gk, Ik;
  double VL, Vc;
  double Q, dQdV;
  double Cjo;
  int na, nb;


    // code template outlining procedure for nonlinear capacitor
    for(i = 1; i <= numNonLinCap; i++) {
        inst = NonLinCap[i];
        Cjo = inst->value;
        na = inst->pNode;
        nb = inst->nNode;
        // Calculate voltage across capacitor. It is assumed that 
	// at the first iteration of a new timepoint
        // Sol has the solution from the previous timepoint
        Vc = Xk[na]-Xk[nb];

        // calculate charge and VL
        VL = 0.75-0.1*log(1+exp(-10*(Vc-.75)));
        Q = 1.6*Cjo*(1-sqrt(1-VL/0.8));


////////////////EQUIVALENT CAPACITANCE CALCULATION/////////////////////////

        dQdV = (Cjo*exp(-10*Vc+7.5)) / (sqrt(0.0625+0.125*log(1+exp(-10*Vc+7.5)))*(1+exp(-10*Vc+7.5)));

/////////////////INITIAL CONDITIONS FOR NEWTON AND LMS/////////////////////

        if(iter_counter == 0) {
           // first iteration of a given timepoint
           if(time_step_count == 1) {
               // first time point
               inst->qdot = 0;
             inst->alpha = 0;
             inst->beta = 0;
             Vc = 0.7;
           }

//////////// SUBSEQUENT LMS ITERATION(time points)///////////////////////
////////// USE THE OLD ALPHA BETA AND NEW Q TO GET NEW Q'////////////////
           else {

               inst->qdot = (inst->alpha)*Q+(inst->beta);
           }
//////////////////UPDATE THE NEW ALPHA AND BETA//////////////////////////

           intgr8(Q,inst->qdot,h,&inst->alpha,&inst->beta);
        }

/////////////////////Gk FOR THE COMPANION NETWORK///////////////////////
///////////////////////////Gk = C*ALPHA/////////////////////////////////

        Gk = (inst->alpha)*dQdV;

///////////////////////////Ik calculation///////////////////////////////

        Ik = (inst->alpha)*Q-(inst->alpha)*dQdV*Vc+(inst->beta);

////////// stamp matrix
    *(inst->ppp) += Gk;
    *(inst->ppn) -= Gk;
    *(inst->pnp) -= Gk;
    *(inst->pnn) += Gk;
    
    Rhs[na] -= Ik;
    Rhs[nb] += Ik;
    }

}
