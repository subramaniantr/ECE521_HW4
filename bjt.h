/* information used to describe a single instance */

typedef struct bjt{
    char *name;  /* pointer to character string naming this instance */
    int cNode; /* number of positive node of resistor */
    int bNode; /* number of negative node of resistor */
    int eNode; /* number of negative node of resistor */
    char *mname;  /* pointer to character string naming the model */
    double Vbe_old;
    double Vbc_old;
    
    double *pcc;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pcb;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pce;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pbc;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pbb;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pbe;     /*pointer to sparse-matrix location (pNode, pNode)*/
    double *pec;     /*pointer to sparse-matrix location (pNode, nNode)*/
    double *peb;     /*pointer to sparse-matrix location (nNode, nNode)*/
    double *pee;     /*pointer to sparse-matrix location (nNode, pNode)*/
} bjt ;

