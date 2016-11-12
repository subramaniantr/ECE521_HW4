/* information used to describe a single instance */

typedef struct vcvs{
    char *name;  /* pointer to character string naming this vcvs */
    int pNode; /* number of positive node of vcvs */
    int nNode; /* number of negative node of vcvs */
    int pCNode; /* number of positive control node of vcvs */
    int nCNode; /* number of negative control node of vcvs */ 
    int branchNum ; /* number of branch */
    double gain;  /* value of gain */

    double *ppb;     
    double *pnb;     
    double *pbp;     
    double *pbn;     
    double *pbpC;    
    double *pbnC;    
} vcvs ;

