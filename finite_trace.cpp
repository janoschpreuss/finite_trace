#include <solve.hpp>
using namespace ngsolve;
using namespace ngfem;
#include <python_comp.hpp>
//#include <SpaceTimeFE.hpp>
//#include "Coeff/myCoefficient.hpp"
#include "comp/GlobalSpaceTimeFESpace.hpp"



// a global function ...
void Hello()
{
  cout << "Hello " << endl;
}

PYBIND11_MODULE(finite_trace,m) {
  // import ngsolve such that python base classes are defined
  auto ngs = py::module::import("ngsolve");
 
  static size_t global_heapsize = 1000000;
  static LocalHeap glh(global_heapsize, "finite_trace_lh", true);
 
  m.def("Hello", &Hello);

  //ExportGlobalSpaceTimeFESpace(m);
  ExportFESpace<GlobalSpaceTimeFESpace>(m, "GlobalSpaceTimeFESpace");


  // This adds documented flags to the docstring of objects.
  ngs.attr("_add_flags_doc")(m);
}

