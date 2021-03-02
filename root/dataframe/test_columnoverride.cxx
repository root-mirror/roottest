// test that RDF correctly throws exceptions when users re-define a column already present in the dataframe
#include "ROOT/RDataFrame.hxx"
#include <stdexcept>
#include <string>

int main()
{
   auto exceptionCount = 0u;
   const auto expectedExceptionCount = 4u;

   // create tree
   ROOT::RDataFrame newd(1);
   newd.Define("x", [] { return 1; }).Snapshot<int>("t", "coloverride.root", {"x"});

   ROOT::RDataFrame d("t", "coloverride.root");
   // re-define TTree variable
   try {
      auto c = d.Define("x", [] { return 2; }).Count();
   } catch (const std::runtime_error &e) {
      std::string msg(e.what());
      const auto expected_msg = "branch \"x\" already present in TTree";
      if (msg.find(expected_msg) == std::string::npos)
         throw;
      exceptionCount++;
   }

   // re-define TTree variable (jitted `Define`)
   try {
      auto c = d.Define("x", "2").Count();
   } catch (const std::runtime_error &e) {
      std::string msg(e.what());
      const auto expected_msg = "branch \"x\" already present in TTree";
      if (msg.find(expected_msg) == std::string::npos)
         throw;
      exceptionCount++;
   }

   // re-define `Define`d column
   try {
      auto c = d.Define("y", [] { return 0; }).Define("y", [] { return 1; }).Count();
   } catch (const std::runtime_error &e) {
      std::string msg(e.what());
      const auto expected_msg = "Redefinition of column \"y\"";
      if (msg.find(expected_msg) == std::string::npos)
         throw;
      exceptionCount++;
   }

   // re-define `Define`d column (with a jitted Define)
   try {
      auto c = d.Define("y", [] { return 0; }).Define("y", "1").Count();
   } catch (const std::runtime_error &e) {
      std::string msg(e.what());
      const auto expected_msg = "Redefinition of column \"y\"";
      if (msg.find(expected_msg) == std::string::npos)
         throw;
      exceptionCount++;
   }

   if (exceptionCount != expectedExceptionCount)
      return 1;

   return 0;
}
