typedef struct transformer{
    char *name;  /* pointer to character string naming this transformer */
    int pCNode; /* number of positive node of transformer */
    int nCNode; /* number of negative node of transformer */
    int pNode; /* number of positive control node of transformer */
    int nNode; /* number of negative control node of transformer */
    int branchNum ; /* number of branch */
    double n;  /* value of n */

    double *ppCb;    
    double *pnCb;    
    double *ppb;     
    double *pnb;     
    double *pbpC;    
    double *pbnC;    
    double *pbp;     
    double *pbn;     
} transformer ;

