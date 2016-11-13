/* information used to describe a single instance */

typedef struct linind{
    char *name;  /* pointer to character string naming this instance */
    int pNode; /* number of positive node of resistor */
    int nNode; /* number of negative node of resistor */
    int branchNum ; /* number of branch */

    double alpha; 
    double beta; 

    double value;    //L value
    double idot;

    double *ppb;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pnb;     /*pointer to sparse-matrix location (pNode, nNode)*/
    double *pbp;     /*pointer to sparse-matrix location (nNode, nNode)*/
    double *pbn;     /*pointer to sparse-matrix location (nNode, pNode)*/
    double *pbb;     /*pointer to sparse-matrix location (nNode, pNode)*/
} linind ;  

