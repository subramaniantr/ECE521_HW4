/* information used to describe a single instance */

typedef struct diode{
    char *name;  /* pointer to character string naming this instance */
    int pNode; /* number of positive node of resistor */
    int nNode; /* number of negative node of resistor */

    char *mname;  /* pointer to character string naming the model */
    double area;  /* area factor */
    double Vd_old; 

    double *ppp;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *ppn;     /*pointer to sparse-matrix location (pNode, nNode)*/
    double *pnp;     /*pointer to sparse-matrix location (nNode, nNode)*/
    double *pnn;     /*pointer to sparse-matrix location (nNode, pNode)*/
} diode ;

