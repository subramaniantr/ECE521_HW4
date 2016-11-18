#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "dio.h"

double pnjlim();

void makeDio(Dio, numDio, buf)
diode *Dio[];
int numDio;
char *buf;
{
    diode *inst;
    int j, nodeP, nodeN, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeP = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeN = getMappedNode(node);
    j = getNextField(buf, mname, j);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(diode, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeP;
    inst->nNode = nodeN;
    inst->mname = (char *)strdup(mname);
    inst->area = value;
    Dio[numDio] = inst;
}

void printDio(Dio, numDio)
diode *Dio[];
int numDio;
{
    int i;
    diode *inst;

    for(i = 1; i <= numDio; i++) {
	inst = Dio[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->mname, inst->area);
    }
}




void setupDio(Matrix, Rhs, Dio, numDio)
char *Matrix;
double *Rhs;
diode *Dio[];
int numDio;
{
    int i, p,n;
    diode *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numDio; i++) {
	inst = Dio[i];
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

void loadDio(Matrix, Rhs, Dio, numDio, Xk,icheck,time_step_count)
char *Matrix;
double *Rhs;
diode *Dio[];
int numDio;
double* Xk;
int *icheck;
int time_step_count;
{
  int i, p, n;
  diode *inst;
  double gk, Ik,Id, Vd,Vcrit;
  double Is, Vt,area;


////////Hard coding Is, Vt and Vcrit Values////////////////////////////////////
      Is = 1.0e-16;
      Vt = 0.0258;
      Vcrit = Vt*log(Vt/(Is*sqrt(2)));

  for(i = 1; i <= numDio; i++) {
    inst = Dio[i];
    p = inst->pNode;
    n = inst->nNode;
///////////Getting Vd from Vp and Vn////////////////////////////////////
    Vd = Xk[p]-Xk[n];
    area = inst->area;
/////////////APPLYING ONE TIME INITIAL CONDITION FOR CONVERGENCE////////
   if(time_step_count == 1 && iter_counter == 0){
      Vd = 0.5;
      inst->Vd_old = 0.8;
    }
//////////////////////////////////APPLYING LIMITING///////////////////////
     Vd= pnjlim(Vd,inst->Vd_old,Vt,Vcrit, icheck);

///////////////////////////////ASSIGNING V_OLD///////////////////////////////////
     inst->Vd_old = Vd;
     

///////////////Id and gk Calculation////////////////////////////////////
                                                                 
     Id = area*Is*(exp(Vd/Vt)-1);                                
     gk = area*Is*exp(Vd/Vt)/Vt;                       

////////////////////Ik Calculation//////////////////////////////////////

     Ik = Id-gk*Vd;
     printf(" Ik = %f \n",Ik);

        // stamp matrix
    *(inst->ppp) += gk;
    *(inst->ppn) -= gk;
    *(inst->pnp) -= gk;
    *(inst->pnn) += gk;
    
    Rhs[p] -= Ik;
    Rhs[n] += Ik;
  }
}
