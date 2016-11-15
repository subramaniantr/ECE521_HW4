
#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "linind.h"



void makeLinInd(LinInd, numLinInd, buf)
linind *LinInd[];
int numLinInd;
char *buf;
{
    linind *inst;
    int j, nodeP, nodeN,branchNum, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    branchNum = getMappedBranch(name);
    j = getNextField(buf, node, j);
    nodeP = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeN = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(linind, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeP;
    inst->nNode = nodeN;
    inst->branchNum = branchNum ;
    inst->value = value;
    LinInd[numLinInd] = inst;
}

void printLinInd(LinInd, numLinInd)
linind *LinInd[];
int numLinInd;
{
    int i;
    linind *inst;

    for(i = 1; i <= numLinInd; i++) {
	inst = LinInd[i];
	printf("%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->value);
    }
}




void setupLinInd(Matrix, Rhs, LinInd, numLinInd)
char *Matrix;
double *Rhs;
linind *LinInd[];
int numLinInd;
{
    int i,p,n,b;
    linind *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numLinInd; i++) {
	inst = LinInd[i];
	inst->branchNum += NumNodes;
	p = inst->pNode;
	n = inst->nNode;
        b = inst->branchNum;
	/* setup matrix and pointers */
	inst->ppb = spGetElement(Matrix, p, b);
	inst->pnb = spGetElement(Matrix, n, b);
	inst->pbp = spGetElement(Matrix, b, p);
	inst->pbn = spGetElement(Matrix, b, n);
	inst->pbb = spGetElement(Matrix, b, b);
    }
}


void loadLinInd(Matrix, Rhs, LinInd, numLinInd, Xk,h ,time_step_count)
char *Matrix;
double *Rhs;
linind *LinInd[];
int numLinInd;
double* Xk;
double h;
int time_step_count;
{
  int i, p, n, b;
  linind *inst;
  double Rk, Vk;
  double  Ib;
  double Indval;


  // code template outlining procedure for linear capacitor
  for(i = 1; i <= numLinInd; i++) {
      inst = LinInd[i];
      Indval = inst->value;
      b =  inst->branchNum;
      // Calculate voltage across capacitor. It is assumed that 
    // at the first iteration of a new timepoint
      // Sol has the solution from the previous timepoint
      Ib = Xk[b];

///////////////INITIAL CONDITIONS FOR NEWTON AND LMS/////////////////////
///INTEGRATION METHOD SHOULD BE EXECUTED ONLY ONCE IN A TIME STEP////////
      if(iter_counter == 0) {  
         // first iteration of a given timepoint
         if(time_step_count == 1) {
             // first time point
             inst->idot = 0;
             inst->alpha = 0;
             inst->beta = 0;
             Ib = 0;
         }

////////// SUBSEQUENT LMS ITERATION(time points)///////////////////////
//////// USE THE OLD ALPHA BETA AND NEW I TO GET NEW I'////////////////
         else {

             inst->idot = (inst->alpha)*Ib+(inst->beta);
         }
////////////////UPDATE THE NEW ALPHA AND BETA//////////////////////////

         intgr8(Ib,inst->idot,h,&inst->alpha,&inst->beta);
      }//IF CONDITION ONLY EXECUTED WHEN NEWTON LOOP STARTS 
         //SEPARATING THE NEWTON FROM THE LMS

/////////////////////Rk FOR THE COMPANION NETWORK///////////////////////
///////////////////////////Rk = C*ALPHA/////////////////////////////////

      Rk = (inst->alpha)*Indval;
printf("Rk = %f",Rk);
///////////////////////////Vk calculation///////////////////////////////

      Vk = (inst->beta)*Indval;
printf("Vk = %f",Vk);

////////// stamp matrix
      *(inst->ppb) += 1; 
      *(inst->pnb) -= 1; 
      *(inst->pbp) += 1; 
      *(inst->pbn) -= 1; 
      *(inst->pbb) -= Rk; 
      
            Rhs[b] += Vk;
    
      }

}
