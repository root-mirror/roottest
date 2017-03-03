#include "TFile.h"
#include "ROOT/TDataFrame.hxx"
#include "TROOT.h"

#include <cmath> // sqrt
#include <iostream>
#include <numeric> // accumulate

void FillTree(const char* filename, const char* treeName) {
   TFile f(filename, "RECREATE");
   TTree t(treeName, treeName);
   double b;
   t.Branch("b", &b);
   for(int i = 0; i < 100000; ++i) {
      b = i / 100000.;
      t.Fill();
   }
   t.Write();
   f.Close();
}

int main(int argc, char** argv) {
   auto fileName = "test_reports.root";
   auto treeName = "reportsTree";
   FillTree(fileName, treeName);

   auto cut1 = [](double b) { return b < 0.001; };
   auto cut2 = [](double b) { return b > 0.05; };
   auto noopb = [](double b) { return true; };

   // multi-thread cutflow reports with default branch, multiple runs
#ifdef R__USE_IMT
   ROOT::EnableImplicitMT();
#endif
   TFile f(fileName);
   ROOT::Experimental::TDataFrame df("reportsTree", &f, {"b"});
   auto f1 = df.Filter(cut1, {}, "mtf");

   auto ac1 = df.AddColumn("foo", []() { return 42; });
   auto f2 = ac1.Filter(noopb);
   auto f3 = f2.Filter(cut2, "mtf2");

   // Report on the original dataframe
   // "mtf", "mtf2" will be listed, in this order
   df.Report();
   std::cout << "--\n";
   // Report on a named filter
   // only "mtf" listed
   f1.Report();
   std::cout << "--\n";
   // Report on nodes with no upstream named filters (new column, unnamed filter)
   // no output
   ac1.Report();
   f2.Report();
   std::cout << "--\n";
   // Report on a named filter with upstream unnamed filters
   // only "mtf2" listed
   f3.Report();

   return 0;
}

void test_reports(int argc = 1, char** argv = nullptr) {
   main(argc, argv);
}
