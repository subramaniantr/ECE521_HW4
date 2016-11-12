/* MOSFET Template code derived from SPICE - K. Mayaram */

#include "macros.h"
#include "math.h"
#include "defs.h"
#include "mosfet.h"
#include "sparse/spMatrix.h"

double limvds();
double fetlim();

void makeMosfet(Mosfet, numMosfet, buf)
mosfet *Mosfet[];
int numMosfet;
char *buf;
{
    mosfet *inst;
    int j, nodeD, nodeS, nodeG, nodeB, branchNum, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof(),W,L;

	/* incrementing the number of branches */

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeD = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeG = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeS = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, mname, j);
    j = getNextField(buf, num, j);
    W = atof(num);
    j = getNextField(buf, num, j);
    L = atof(num);

    inst = CALLOC(mosfet, 1);
    inst->name = (char *)strdup(name);
    inst->dNode = nodeD;
    inst->gNode = nodeG;
    inst->sNode = nodeS;
    inst->bNode = nodeB;
    inst->mname = (char *)strdup(mname);
    inst->W = W;
    inst->L = L;
    Mosfet[numMosfet] = inst;
}


void printMosfet(Mosfet, numMosfet)
mosfet *Mosfet[];
int numMosfet;
{
    int i;
    mosfet *inst;

    for(i = 1; i <= numMosfet; i++) {
	inst = Mosfet[i];
	printf("%s\t%s\t%s\t%s\t%s\t%s\t%f\t%f\t\n", inst->name, NodeArray[inst->dNode], NodeArray[inst->gNode], NodeArray[inst->sNode], NodeArray[inst->bNode],inst->mname,inst->W, inst->L);
    }
}

void setupMosfet(Matrix, Rhs, Mosfet, numMosfet)
char *Matrix;
double *Rhs;
mosfet *Mosfet[];
int numMosfet;
{
    int i, d,g,s,b;
    mosfet *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numMosfet; i++) {
	inst = Mosfet[i];
//	inst->branchNum += NumNodes;
	d = inst->dNode;
	g = inst->gNode;
	s = inst->sNode;
	b = inst->bNode;
	/* setup matrix and pointers */
	inst->pdd = spGetElement(Matrix, d, d);
	inst->pds = spGetElement(Matrix, d, s);
	inst->pdg = spGetElement(Matrix, d, g);
	inst->pdb = spGetElement(Matrix, d, b);
	inst->psd = spGetElement(Matrix, s, d);
	inst->pss = spGetElement(Matrix, s, s);
	inst->psg = spGetElement(Matrix, s, g);
	inst->psb = spGetElement(Matrix, s, b);
        
    }
}



void loadMosfet(Matrix, Rhs, Mosfet, numMosfet, Xk)
char *Matrix;
double *Rhs;
mosfet *Mosfet[];
int numMosfet;
double* Xk;
{
  int i, g, d, s, b;
  mosfet *inst;
  double gm, gds, gmbs, Ik, Id, Vgs, Vgd, Vds, Vbs, Vbd;
  double  Vgs_old, Vds_old;
  double lambda, K, Vto, gamma, phi, W, L, beta, betap, vgst;
  double type, norm, rev, von, arg, sarg;
  int j;
  for(i = 1; i <= numMosfet; i++) {
    inst = Mosfet[i];
    g = inst->gNode;
    d = inst->dNode;
    s = inst->sNode;
    b = inst->bNode;
    
    Vgs = Xk[g]-Xk[s];
    Vgd = Xk[g]-Xk[d];
    Vds = Xk[d]-Xk[s];
    Vbs = Xk[b]-Xk[s];
    Vbd = Xk[b]-Xk[d];
     



    W = inst->W;
    L = inst->L;

    gamma = 0.5;
    phi = 0.6;

    if (!strcmp(inst->mname, "pmos")) {
      /* case pmos */
      lambda = 0.05;
      K = 50.0e-6;
      Vto = -0.9;
      type = -1.0;
    } else {
      /* case nmos */
      lambda = 0.05;
      K = 100.0e-6;
      Vto = 0.7;
      type = 1;
    }
    beta = K*W/L;
    Vto = type*Vto;
    Vgs = type*Vgs;
    Vgd = type*Vgd;
    Vds = type*Vds;
    Vbs = type*Vbs;
    Vbd = type*Vbd;
    
    norm = 1.0;
    rev = 0.0;
/////////////APPLYING ONE TIME INITIAL CONDITION FOR CONVERGENCE/////////////////
   if(iter_counter == 0)
    {
      Vgs = 1.1;
      Vds = 1.1;
      inst->Vgs_old = 1.0;
      inst->Vds_old = 1.0;
      Vbs = 0.0;
    }
//////////////////////////////////APPLYING LIMITING////////////////////////////////
   Vgs=fetlim(Vgs,inst->Vgs_old,Vto);
   Vds=limvds(Vds,inst->Vds_old);

///////////////////////////////ASSIGNING V_OLD///////////////////////////////////
   inst->Vgs_old = Vgs;
   inst->Vds_old = Vds;

//////////////////////MODES AND CALCULATION OF CURRENTS AND INCREMENTAL GK///////
    if(Vds < 0.0 ) {
	/* reverse mode */
	Vds = -Vds;
	Vgs = Vgd;
	Vbs = Vbd;
	norm = 0.0;
	rev = 1.0;
    }
    /* calculation of Vt for Vsb < 0 */

    if (Vbs <= 0 ) {
	sarg=sqrt(phi-Vbs);
    } else {
	sarg=sqrt(phi);
	sarg=sarg-Vbs/(sarg+sarg);
	sarg=MAX(0,sarg);
    }
    von=Vto+gamma*(sarg-sqrt(phi));
    vgst=Vgs-von;
    if (sarg <= 0) {
	arg=0;
    } else {
	arg=gamma/(sarg+sarg);
    }
    if (vgst <= 0) {
	/*
	 *     cutoff region
	 */
	Id=0;
	gm=0;
	gds=0;
	gmbs=0;
    } else{
	/*
	 *     saturation region
	 */
	betap=beta*(1+lambda*Vds);
	if (vgst <= Vds) {
	    Id=betap*vgst*vgst*.5;
	    gm=betap*vgst;
	    gds=lambda*beta*vgst*vgst*.5;
	    gmbs=gm*arg;
	} else {
	/*
	 *     linear region
	 */
	    Id=betap*Vds*(vgst-.5*Vds);
	    gm=betap*Vds;
	    gds=betap*(vgst-Vds)+lambda*beta*Vds*(vgst-.5*Vds);
	    gmbs=gm*arg;
	}
    }
    if (norm == 1.0) {
	Ik = type*(Id-gm*Vgs-gds*Vds-gmbs*Vbs);    
    } else {
	Ik = -type *(Id-gm*Vgs-gds*Vds-gmbs*Vbs);    
    }
    /* stamp mosfet*/
    *(inst->pdd) += gds + rev*(gm+gmbs);
    *(inst->pds) -= gds + norm*(gm+gmbs);
    *(inst->pdg) += (norm - rev)*gm;
    *(inst->pdb) += (norm - rev)*gmbs;
    *(inst->psd) -= gds + rev*(gm+gmbs);
    *(inst->pss) += gds + norm*(gm+gmbs);
    *(inst->psg) -= (norm - rev)*gm;
    *(inst->psb) -= (norm - rev)*gmbs;
    
    Rhs[d] -= Ik;
    Rhs[s] += Ik;
  }
}
