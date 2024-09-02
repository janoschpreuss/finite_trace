#include "GlobalSpaceTimeFESpace.hpp"
#include <diffop_impl.hpp>

namespace ngcomp
 {
  
  /*
  class GlobalSpaceTimeFiniteElement : public FiniteElement
  {
    ELEMENT_TYPE et;
    int ndof;
  public:
    NumberFiniteElement (ELEMENT_TYPE _et, _ndof)
      : FiniteElement(_ndof, 0), et(_et) ndof(_ndof) { ; }
     // INLINE FiniteElement (int andof, int aorder) need to change andof later
    HD virtual ELEMENT_TYPE ElementType() const { return et; }
  }; */

   void GlobalSpaceTimeFiniteElement :: CalcShape (const IntegrationPoint & ip,
                                    BareSliceVector<> shape) const
    {
         
	 //auto pnt = ip.Point();
	 shape(0) = 0.0;  
	 // double tau = 
	 //
	 /*
	  
	 Vector<> time_shape(tFE->GetNDof());
         IntegrationPoint z(override_time ? time : ip.Weight());
            
         if(!IsSpaceTimeIntegrationPoint(ip))//only effectiv if sanity check is on
           throw Exception("GlobalSpaceTimeFiniteElement  :: CalcShape called with a mere space IR");
            
            tFE->CalcShape(z,time_shape);

            Vector<> space_shape(sFE->GetNDof());
            sFE->CalcShape(ip,space_shape);

            // define shape functions
            int ii = 0;
            for(int j=0; j<tFE->GetNDof(); j++) {
              for(int i=0; i<sFE->GetNDof(); i++) {
                shape(ii++) = space_shape(i)*time_shape(j);
              }
            }

	 */
       
    }

   void GlobalSpaceTimeFiniteElement :: CalcMappedShape (const BaseMappedIntegrationPoint & bmip,
                                                        BareSliceVector<> shape) const
    {
         
	 auto pnt = bmip.GetPoint();
	 //cout << "pnt = " << pnt << endl;
	 shape(0) = pnt[0]*pnt[1];   
    }



  class GlobalSpaceTimeDiffOp : public DiffOp<GlobalSpaceTimeDiffOp>
  {
  public:
    enum { DIM = 1 };
    enum { DIM_SPACE = 0 };
    enum { DIM_ELEMENT = 0 };
    enum { DIM_DMAT = 1 };
    enum { DIFFORDER = 0 };
    
    static bool SupportsVB (VorB checkvb) { return true; }

    template <typename MIP, typename MAT>
    static void GenerateMatrix (const FiniteElement & fel, const MIP & mip,
				MAT && mat, LocalHeap & lh)
    {
      if(fel.GetNDof()) {
        //mat(0,0) = 1;
        //IntegrationPoint ip(mip.IP());
        mat = 0.0;

        const GlobalSpaceTimeFiniteElement& gstfe = dynamic_cast<const GlobalSpaceTimeFiniteElement& > (fel);
        FlatVector<> dshape (gstfe.GetNDof(),lh);
        gstfe.CalcMappedShape(mip,dshape);
	for (int k = 0; k < dshape.Size(); k++)
	  mat.Row(0) = dshape;
        
      }
    }
    
  };



  GlobalSpaceTimeFESpace::GlobalSpaceTimeFESpace (shared_ptr<MeshAccess> ama, const Flags & flags, bool checkflags)
      : FESpace (ama, flags)
    {
      type = "numbers";
      // TODO 
      evaluator[VOL] = make_shared<T_DifferentialOperator<GlobalSpaceTimeDiffOp>>();
      evaluator[BND] = make_shared<T_DifferentialOperator<GlobalSpaceTimeDiffOp>>();
      //evaluator[BBND] = make_shared<T_DifferentialOperator<NumberDiffOp>>();
      //evaluator[BBBND] = make_shared<T_DifferentialOperator<NumberDiffOp>>();


    if (dimension > 1)
      {
        //for (auto vb : { VOL,BND, BBND, BBBND })
        for (auto vb : { VOL,BND })
          evaluator[vb] = make_shared<BlockDifferentialOperator> (evaluator[vb], dimension);
      }
    
      
      is_atomic_dof = BitArray(1);
      is_atomic_dof = true;
    }



  void GlobalSpaceTimeFESpace::Update()
    {
      SetNDof( ((ma->GetCommunicator().Size() > 1) && (ma->GetCommunicator().Rank() == 0)) ? 0 : 1 );
    }

  
  FiniteElement & GlobalSpaceTimeFESpace::GetFE (ElementId ei, Allocator & lh) const
    {
      if (DefinedOn(ei))
        return *new (lh) GlobalSpaceTimeFiniteElement(ma->GetElType(ei),1);
      else
        return SwitchET (ma->GetElType(ei), [&] (auto et) -> FiniteElement&
        {
          return *new (lh) DummyFE<et.ElementType()> ();
        });
    }

  void GlobalSpaceTimeFESpace::GetDofNrs (ElementId ei, Array<int> & dnums) const
    {
      if (DefinedOn(ei))
        {
          dnums.SetSize(1);
          dnums[0] = 0;
        }
      else
        dnums.SetSize(0);
    }


  void GlobalSpaceTimeFESpace :: GetGlobalDofNrs (int gnr, Array<int> & dnums) const
  {
    /** If parallel, there is one global DOF shared by all ranks except rank 0.
	If not parallel, there is also one DOF. **/
    if ( IsParallel() && (GetParallelDofs()->GetCommunicator().Size() > 1) && (GetParallelDofs()->GetCommunicator().Rank() == 0) )
      { dnums.SetSize0(); }
    else
      { dnums.SetSize(1); dnums[0] = 0; }
  }

 static RegisterFESpace<GlobalSpaceTimeFESpace> initifes ("GlobalSpaceTimeFESpace");

  /*
  void ExportGlobalSpaceTimeFESpace(py::module m)
{
  using namespace ngcomp;
  cout << "Exporting GlobalSpaceTimeFESpace" << endl;
  ExportFESpace<GlobalSpaceTimeFESpace>(m, "GlobalSpaceTimeFESpace");

}
*/


}
