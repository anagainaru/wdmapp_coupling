#ifndef COMMPART1_H
#define COMMPART1_H

#include "adios2Routines.h"
#include "testutilities.h"
#include <cstddef>
namespace coupler {
class Array1d;
// create class for gene's mesh as well as its domain decomposition
class Part1ParalPar3D {
  public:
/*variables for gene*/
    LO mype ; // my process rank in mpi_comm_world
    LO mype_x; // my process rank in comm_x
    LO mype_y; // my process rank in comm_y
    LO mype_z; // my process rank in comm_z
    MPI_Comm comm_x;
    MPI_Comm comm_y;
    MPI_Comm comm_z;
    MPI_Comm comm_cart;
    LO  NP; // The total number of processes
    LO npx,npy,npz;
    LO nx0,nxb,li1,li2,lg0,lg1,lg2;
    LO ny0,nyb,llj0,llj1,llj2,lm0,lm1,lm2;
    LO nz0,nzb,lk1,lk2,ln0,ln1,ln2;
 
    LO* li0,lk0;  

    LO lj0; //the number of elements in Fourier space.
    LO lj1; //the number of elements in Fourier space.
    LO lj2; //the number of elements in Fourier space.   

    GO totnodes; // the total nodes number of the poloidal cross setion of part1.
    LO n_cuts; //The number of toroidal planes.
    int periods[3]={1,0,1};
    double* xcoords=NULL; // The 1d array storing the radial position of all flux surfaces
    double* pzcoords=NULL; // The 1d array storing the poloidal angle of all vertices along the poloidal cross section.
    double* pzp=NULL; // The 1d array storing the poloial on each process.
   
    // parameters for creating the magnetic field, density and temperature background. 
    double* q_prof=NULL;  //safty factor
    LO n0_global;
    LO ky0_ind;

    double rhostar; //This quantity is obtained by complex formula
    double* C_y=NULL;
    double minor_r;
    double lx_a;
    LO sign_phi;
    double dx;
    double Tref,nref,mref,Bref,Lref;
    
    double dz;  // The equal step length along the poloidal flux curve.

    int res_fact; //This is different from 4 which has no substantial implementation in wdmapp/GENE 
    double L_tor;
    double* phi_cut=NULL;
    double dy;
    LO y_res;
    LO y_res_back;
    double norm_fact_dens;
    double norm_fact_field; 


    // parameters for Adios transfer
    GO blockstart,blockend,blockcount;    

/*------------------------------------------------------*/
/*variables specially owned by GEM*/
    LO numprocs;
    LO ntude,imx,jmx,kmx,ntheta; //imx,jmx,kmx are the number of cells on the respective dimension.
    MPI_Comm grid_comm,tube_comm;

    double dth;
    double* thetagrid; //store the \theta value of all the nodes on one poloidal cross section of GEM
    double** thflx; //store GEM's flux coordinates theta*; sent from GEM
   
    /* constructor
     * optionally read preproc, test_case and test_dir from user
     */
    Part1ParalPar3D(LO * parpar, 
        double* xzcoords,
        double* q_prof_,
        double* gene_cy,
	bool pproc = true,
        std::string tdir="")
      : preproc(pproc),q_prof(q_prof_),
        test_case(TestCase::off){
      init(parpar, xzcoords, q_prof, gene_cy, tdir);
      if(!mype) std::cerr << mype << " Done with Part1ParalPar3D class intialization \n"; 
    }
    /*Test case constructor*/
    Part1ParalPar3D(bool pproc,
        TestCase tcase,
      	std::string tdir)
      : preproc(pproc), test_case(tcase){
      init(NULL, NULL, NULL, NULL, tdir);
    }

    /*The constructor for GEM*/
    Part1ParalPar3D(Array1d* gemmesh){
      initGem(gemmesh);
    }
    ~Part1ParalPar3D()
    {
      if(xcoords!=NULL)  delete[] xcoords;
      if(pzcoords!=NULL) delete[] pzcoords;
      if(pzp!=NULL)      delete[] pzp;
      if(q_prof!=NULL)   delete[] q_prof;
    }
    
  private:
    const bool preproc;
    const TestCase test_case;
    void init(LO* parpar, double* xzcoords, double* q_prof, double* gene_cy, std::string test_dir="");
    void initGem()
    void initTest0(std::string test_dir);
    /* init* helper function */
    void CreateSubCommunicators();
   /* destructor helper function */
    void MpiFreeComm();
    void blockindice(); 
};

//create class for GEM's mesh as well as its domain decomposition
class gemParaMesh3D {
  public:
    LO myid,numprocs; 
    LO ntude,imx,jmx,kmx;
    MPI_Comm grid_comm,tube_comm,x_comm,gx_comm; 
   
    double* thetagrid; //store the \theta value of all the nodes on one poloidal cross section of GEM
    double** thflx; //store GEM's flux coordinates theta*; sent from GEM     
}


template<typename T, size_t SIZE>
size_t getSize(T (&)[SIZE]) {
    return SIZE;
}

}

#endif
