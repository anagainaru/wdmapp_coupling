#ifndef IMPORTPART3MESH_H
#define IMPORTPART3MESH_H

#include "couplingTypes.h"
#include "testutilities.h"

namespace coupler {

//forward declare the Part1ParalPar3D class; we don't care how 
// the class is defined in this header since we don't call
// any of its functions or access its member variables
class Array2d;
class Part1ParalPar3D;

class Part3Mesh3D{
  public:
/*variables of XGC for both gene-xgc and gem-xgc coupling*/
    LO nsurf;  // number of flux surfaces on the coupled domain
    LO npsi_surf; // The notal number of surfaces provided by XGC
    LO block_count;  // number of nodes
    LO* versurf = NULL; // numbers of vertices on the flux surfaces locating on the part1 domain.
    double* xcoords = NULL;
    LO* li0
    LO  li1,li2;
    LO** xboxinds=NULL;  //The indexes of all boxes on the radial dimension
    LO lj0;
    LO* mylk0=NULL;
    LO* mylk1=NULL;
    LO* mylk2=NULL; // The indexes of box along z dimension
    LO* nstart; // Store the the index of the minimal element of the array of 
                // the z coordinates on each cross section

//  temporary members which would be deleted in the next time refactor
    double* zcoordall = NULL; //zcoordall will be removed in the next refacotr.
    int* cce = NULL; //store cce_ variables only


    // parameters for receiving and sending global 2d arrays
    GO  blockstart,blockend,blockcount; // The  indexes of the 2d box global in z dimension
    LO  cce_first_surface; // The number of the first flux surface where GENE begins.
    LO  cce_last_surface;  // The number of the last flux surface where GENE ends.
    GO  cce_first_node; // The number of the first active node sent by part3;
    GO  cce_last_node; // The number of the last active node send by part3;
    GO  cce_node_number;  // The number of active nodes sent by part3.
    GO  totnodes; // the total number of nodes sent by part3;
    GO  activenodes; // The number of nodes part1 has on the poloidal cross section. 
    LO  shiftx;  // The number of surfaces shifting from part3 first surface to part1 first surface.

    // paramters to create the background of mgnetic field, density and temperature profile
   
    double** pzcoords=NULL;  // The z coordinates of all points within the 2d box.
    double** zcoordsurf=NULL;


/*------------------------------------------------------*/
/*variables specially owned by XGC for GEM-XGC coupling*/
    LO* lgi0;
    double* Rcoordall; // store R of all nodes in XGC mesh
    double* Zcoordall; // store Z of all nodes in XGC mesh
    double* versurf; //store the number of nodes on each surface
    double** surf_idx; //store the vertex indices of each surface
    double** theta; //store the theta value of the nodes on the surface of xgc in the local process
    double** theta_flx; //store  the flux_theta of the nodes on the surface of xgc mesh in the local process
 
    /* constructor - versurf has length = numsurf & versurf[i] = the number of nodes surface[i]
     * xcoords saves the radial coordinate of each surface.
     * zcoords saves the poloidal angle of each node on each surface.
     */

    Part3Mesh3D(Part1ParalPar3D &p1pp3d,
        LO nsurf_,
        LO block_count_,
        LO* versurf_,
        int* cce_,
        double* xcoords_,
        double* zcoord_,
        bool pproc = true)
      : nsurf(nsurf_),
        block_count(block_count_),
        versurf(versurf_),
        cce(cce_),
        xcoords(xcoords_),
        zcoordall(zcoord_),
	preproc(pproc), test_case(TestCase::off) {
        init(p1pp3d);
    }
    /* testing constructor
     * arguments support reading
     * the test case number and directory
     * from the user
     */
    Part3Mesh3D(Part1ParalPar3D &p1pp3d,
        bool pproc,
        TestCase tcase,
        std::string tdir)
      : preproc(pproc), test_case(tcase) {
      assert(tcase < TestCase::invalid);
      init(p1pp3d,tdir);
    }
    Part3Mesh3D(gemParaMesh3D &gem3d,
                Array2d<CV>* xgcnodes)
    {
                initXgcGem();
    }

    ~Part3Mesh3D()
    {
     if(versurf!=NULL) delete[] versurf;
     if(xboxinds!=NULL) delete[] xboxinds;
     if(xcoords!=NULL) delete[] xcoords;
     if(mylk0!=NULL) delete[] mylk0;
     if(mylk1!=NULL) delete[] mylk1;
     if(mylk2!=NULL) delete[] mylk2;
     if(Rcoords!=NULL) delete[] Rcoords;
     if(Zcoords!=NULL) delete[] Zcoords;
     if(pzcoords!=NULL) delete[] pzcoords;
   }
     void  JugeFirstSurfaceMatch(double xp1);
 
  private:
    const bool preproc;
    const TestCase test_case;
    void init(const Part1ParalPar3D &p1pp3d, const std::string test_dir="");
    void initXgcGem();
    /* helper function called by init */
    void DistriPart3zcoords(const Part1ParalPar3D  &p1pp3d,
        const std::string test_dir="");
    /* helper function called by DistriPart3zcoords */
    void DistributePoints(const double* exterarr, const LO gstart, const LO li,
        const double* interarr, const Part1ParalPar3D  &p1pp3d);
    void BlockIndexes(const MPI_Comm comm_x,const LO mype_x,const LO npx);
    /* default constructor 
     * put this in private to prevent users from calling it
     */
    Part3Mesh3D() : preproc(true), test_case(TestCase::off) {};
};


class xgcCoupleGemMesh3D {
  public:
    LO cce_first_surface; // The number of the first surface on the coupled domain
    LO cce_last_surface;  // The number of the last surface on the coupled domain
    LO cce_first_node; // The number of the first node on the coupled domain
    LO cce_last_node; // The number of the last node on the coupled domain
    LO nnode; // the total number of nodes provided by XGC
    LO npsi_surf; // The total number of the surfaces provided by XGC
    LO* versurf; // store the number of the nodes on each surface
    LO nsurf;  // The total number of surface on the coupled domain
   
    LO cce_surface_start; // The number of the first surface of myid rank
    LO cce_surface_end; // The number of the last surface of myid rank

    LO* cce;
    double* Rcoordall; // store R of all nodes in XGC mesh
    double* Zcoordall; // store Z of all nodes in XGC mesh
    double* versurf; //store the number of nodes on each surface
    double** surf_idx; //store the vertex indices of each surface
    double** theta_geo; //store the theta value of the nodes on the surface of xgc in the local process
    double** theta_flx; //store  the flux_theta of the nodes on the surface of xgc mesh in the local process
    LO surf_len_local; // the number of nodes on the local surface;
    LO surf_idx_local; // store the indices by the order of nodes on the surface of xgc mesh.
 
    
    xgcCoupleGemMesh3D(gemParaMesh3D &gem3d,
        LO* cce_, 
        double* Rcoordall_,
        double* Zcoordall_)                          
      : cce(cce_),
        Rcoordall(Rcoordall_),
        Zcoordall(Zcoordall_){ 
        init(gem3d);
     }

    private:
      void init(const gemParaMesh3D &gem3d);
 
};



// The following utility functions should go into a header for utilities if they
// need to be called by other other classes or code.
// If they are not needed elsewhere, the prototypes/definitions below should
// be removed.
LO  minloc(const double* zcoords, const LO n); 

template<class T>
void reshuffleforward(T* array,const LO nstart,const LO vertnum)
{
  T* tmp=new T[vertnum];
  for(LO i=0;i<vertnum-nstart;i++)
    tmp[i]=array[nstart+i];
  for(LO j=vertnum-nstart;j<vertnum;j++)
    tmp[j]=array[j-vertnum+nstart];
  for(LO k=0;k<vertnum;k++)
    array[k]=tmp[k];
  delete[] tmp;
}


template<class T>
void reshufflebackward(T* array,const LO nstart,const LO vertnum)
{
  T* tmp=new T[vertnum];
  for(LO i=vertnum-nstart;i<vertnum;i++)
    tmp[i-vertnum+nstart]=array[i];
  for(LO j=0;j<vertnum-nstart;j++)
    tmp[j+nstart]=array[j];
  for(LO k=0;k<vertnum;k++)
    array[k]=tmp[k];
  delete[] tmp;
}

double minimalvalue(const double* array, const LO n);

}

#endif
