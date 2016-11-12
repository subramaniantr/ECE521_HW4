/* information used to describe a single instance */

typedef struct cccs{
    char *name;  /* pointer to character string naming this cccs */
    int pNode; /* number of positive node of cccs */
    int nNode; /* number of negative node of cccs */
    int cbranchNum ; /* number of control branch */
    double gain;  /* value of current gain */

    double *ppbC;     
    double *pnbC;     
} cccs ;

