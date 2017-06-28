#include "ROOT/TDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReaderArray.h"
#include "ROOT/TSeq.hxx"


void fill_tree(const char* filename, const char* treeName) {
   TFile f(filename,"RECREATE");
   TTree t(treeName,treeName);
   double b1[3];
   t.Branch("b1", &b1, "b1[3]/D");
   for(auto i : ROOT::TSeqI(10)) {
      b1[0] = i;
      b1[1] = i+1;
      b1[2] = i+2;
      t.Fill();
   }
   t.Write();
   f.Close();
   return;
}

int main() {
   auto fileName = "myfile_readerarray.root";
   auto treeName = "myTree";
   fill_tree(fileName,treeName);

   ROOT::Experimental::TDataFrame d(treeName, fileName, {"b1"});
   auto c = d.Filter([](std::array_view<double> a) { std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl; return true; }).Count();
   auto val = *c;
   std::cout << "count " << val << std::endl;
   return 0;
}
