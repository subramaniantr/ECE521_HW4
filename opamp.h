typedef struct opamp{
    char *name;  /* pointer to character string naming this opamp */
    int oNode; /* number of output node of opamp */
    int pNode; /* number of positive control node of opamp */
    int nNode; /* number of negative control node of opamp */
    int branchNum ; /* number of branch */
    double A;  /* value of n */

    double *pbp;     
    double *pbn;     
    double *pbo;     
    double *pob;     
} opamp ;

