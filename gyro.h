typedef struct gyrator{
    char *name;  /* pointer to character string naming this gyrator */
    int pCNode; /* number of positive node of gyrator */
    int nCNode; /* number of negative node of gyrator */
    int pNode; /* number of positive control node of gyrator */
    int nNode; /* number of negative control node of gyrator */
    double g;  /* value of n */

    double *pnCp;     
    double *pnCn;     
    double *ppCp;     
    double *ppCn;     
    double *pppC;     
    double *ppnC;     
    double *pnpC;     
    double *pnnC;     
} gyrator ;

