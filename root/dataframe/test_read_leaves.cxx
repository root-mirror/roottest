#include <ROOT/TDataFrame.hxx>
#include <ROOT/TVec.hxx>
#include <TTree.h>
#include <TFile.h>
#include <TSystem.h>

#include "test_read_leaves.h"

using namespace ROOT::Experimental;

int main()
{
   {
      TFile f("test_read_leaves.root", "RECREATE");
      TTree t("t", "t");

      V v{1, 2};
      t.Branch("v", &v, "a/I:b/I");

      gROOT->ProcessLine(".L test_read_leaves.h+");
      W w;
      t.Branch("w", &w);

      t.Fill();
      t.Write();
   }

   TDataFrame d("t", "test_read_leaves.root");
   d.Filter(check_a_b, {"v.a", "v.b"}).Report();
   gSystem->Unlink("test_read_leaves.root");
   return 0;
}

