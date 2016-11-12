/* information used to describe a single instance */

typedef struct vsource{
    char *name;  /* pointer to character string naming this vsource */
    int pNode; /* number of positive node of vsource */
    int nNode; /* number of negative node of vsource */
    int branchNum ; /* number of branch */
    double voltage;  /* value of vsource */
    double *ppb;     /*pointer to sparse-matrix location (nNode, pNode)*/
    double *pnb;     /*pointer to sparse-matrix location (nNode, nNode)*/
    double *pbp;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pbn;     /*pointer to sparse-matrix location (pNode, nNode)*/
    double *prhsb;     /*pointer to sparse-matrix location (nNode, pNode)*/
    
} vsource ;

