#ifndef FILE_GLOBALSPACETIMEFESPACE
#define FILE_GLOBALSPACETIMEFESPACE

// Partially based on ngsolve/comp/numberfespace.hpp

#include <fespace.hpp>

namespace ngcomp
{


  class GlobalSpaceTimeFiniteElement : public FiniteElement
  {
    
    ELEMENT_TYPE et;
    int ndof;
  public:
    GlobalSpaceTimeFiniteElement (ELEMENT_TYPE _et, int _ndof)
      : FiniteElement(_ndof, 0), et(_et), ndof(_ndof) { ; }
     // INLINE FiniteElement (int andof, int aorder) need to change andof later
    HD virtual ELEMENT_TYPE ElementType() const { return et; }
    
    virtual void CalcShape (const IntegrationPoint & ip,
                              BareSliceVector<> shape) const;
    
    virtual void CalcMappedShape (const BaseMappedIntegrationPoint & bmip,
                                  BareSliceVector<> shape) const;

  };

  class GlobalSpaceTimeFESpace : public FESpace
  {
  public:
    GlobalSpaceTimeFESpace (shared_ptr<MeshAccess> ama, const Flags & flags, bool checkflags=false);
    void Update() override;

    // virtual size_t GetNDof() const { return 1; }

    virtual FiniteElement & GetFE (ElementId ei, Allocator & lh) const override;
    
    virtual void GetDofNrs (ElementId ei, Array<int> & dnums) const override;
    
    virtual void GetGlobalDofNrs (int gnr, Array<int> & dnums) const override;
    
  };

  //void ExportGlobalSpaceTimeFESpace (py::module m);

}
#endif
