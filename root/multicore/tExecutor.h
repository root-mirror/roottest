#include "TH1F.h"
#include "TRandom.h"
#include <functional>
#include <vector>
#include <memory>
#include <numeric> //accumulate

int f(int a)
{
   return a+1;
}

class fClass {
   public:
   int operator()(int a)
   {
      return a+1;
   }
};

template<class T>
int TExecutorPoolTest(T &pool) {
   fClass c;
   auto boundF = std::bind(f, 1);

   /**** TProcessExecutor::Map ****/
   std::vector<int> truth = {1,1,1,1};

   // init list and lambda
   auto res = pool.Map([](int a) -> int { return a+1; }, {0,0,0,0});
   if( res != truth)
      return 1;

   // ... and move-only types
   pool.Map([](int) { return std::make_unique<TList>(); }, { 1, 2, 3 });

   // vector and C++ function
   std::vector<int> vargs = {0,0,0,0};
   auto res2 = pool.Map(f, vargs);
   if(res2 != truth)
      return 2;

   // vector and functor class
   auto res3 = pool.Map(c, vargs);
   if(res3 != truth)
      return 3;

   //nTimes signature and bound function
   auto res6 = pool.Map(boundF, 100);
   if(res6 != std::vector<int>(100,2))
      return 4;

   /**** TProcessExecutor::MapReduce ****/
   int redtruth = 4;
   auto redfunc = [](std::vector<int> a) -> int { return std::accumulate(a.begin(), a.end(), 0); };

   // init list and lambda
   auto redres = pool.MapReduce([](int a) { return a+1; }, {0,0,0,0}, redfunc);
   if(redres != redtruth)
      return 5;

   // vector and C++ function
   std::vector<int> vargs2 = {0,0,0,0};
   auto redres2 = pool.MapReduce(f, vargs2, redfunc);
   if(redres2 != redtruth)
      return 6;

   // vector and functor class
   auto redres3 = pool.MapReduce(c, vargs2, redfunc);
   if(redres3 != redtruth)
      return 7;

   //nTimes signature and bound function
   auto redres6 = pool.MapReduce(boundF, 100, redfunc);
   if(redres6 != 200)
      return 8;

   //TObject::Merge() reduction signature.
   TH1F *htot = new TH1F("htot", "htot", 10, 0, 1);
   std::vector<TH1 *> vhist(5);
   vhist[0] = new TH1F("h0", "h0", 10, 0, 1);
   vhist[1] = new TH1F("h1", "h1", 10, 0, 1);
   vhist[2] = new TH1F("h2", "h2", 10, 0, 1);
   vhist[3] = new TH1F("h3", "h3", 10, 0, 1);
   vhist[4] = new TH1F("h4", "h4", 10, 0, 1);

   for(auto i=0; i<50; i++){
       auto x = gRandom->Gaus(-3,2);
       vhist[i/10]->Fill(x);
       htot->Fill(x);
   }
   auto hred = pool.Reduce(vhist);

   for(auto i = 0; i<52; i++){
        if(htot->GetBinContent(i) != hred->GetBinContent(i))
            return 9;
   }

   delete htot;
   delete hred;
   for(auto el: vhist){
      delete el;
   }

    /***** other tests *****/

   //returning a c-string
    auto extrares1 = pool.Map([]() { return "42"; }, 25);
    for(auto c_str : extrares1)
       if(strcmp(c_str, "42") != 0)
          return 10;

   //returning a string
   auto extrares2 = pool.Map([]() { return std::string("fortytwo"); }, 25);
   for(auto str : extrares2)
      if(str != "fortytwo")
         return 11;

   return 0;
}
