/* information used to describe a single instance */

typedef struct ccvs{
    char *name;  /* pointer to character string naming this ccvs */
    int pNode; /* number of positive node of ccvs */
    int nNode; /* number of negative node of ccvs */
    int cbranchNum ; /* number of control branch */
    int branchNum ; /* number of ccvs branch */
    double Rt;  /* value of trans-resistance */

    double *ppb;     
    double *pnb;     
    double *pbp;     
    double *pbn;     
    double *pbpC;    
    double *pbbC;    
} ccvs ;

