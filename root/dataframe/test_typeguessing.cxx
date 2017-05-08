#include "ROOT/TDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"

#include <string>
#include <stdexcept>

auto fileName("testtypeguessing.root");
auto treeName("myTree");

int main() {
   {
      TFile f(fileName, "RECREATE");
      TTree t(treeName, treeName);
      int b = 42;
      t.Branch("b", &b);
      std::string s = "fortytwo";
      t.Branch("s", &s);
      t.Fill();
      t.Write();
   }

   TFile f(fileName);
   ROOT::Experimental::TDataFrame d(treeName, fileName);
   // TTreeReader should cause a runtime error (type mismatch) when the event-loop is run
   auto hb = d.Histo1D<double>("b");
   // Histo1D("s") should compile and execute (jitting recognizes the std::string type)
   // although the histogram will be filled with meaningless values
   auto hs = d.Histo1D("s");
   *hb;

   return 0;
}
