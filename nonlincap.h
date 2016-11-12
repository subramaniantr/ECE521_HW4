/* information used to describe a single instance */

typedef struct nonlincap{
    char *name;  /* pointer to character string naming this instance */
    int pNode; /* number of positive node of resistor */
    int nNode; /* number of negative node of resistor */

    double alpha; 
    double beta; 

    double value;    //Cj0
    double qdot;

    double *ppp;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *ppn;     /*pointer to sparse-matrix location (pNode, nNode)*/
    double *pnp;     /*pointer to sparse-matrix location (nNode, nNode)*/
    double *pnn;     /*pointer to sparse-matrix location (nNode, pNode)*/
} nonlincap ;

