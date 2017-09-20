#include "tExecutor.h"
#include "TROOT.h"
#include "ROOT/TThreadExecutor.hxx"

int PoolTest() {


   //////////////Tests 1-11///////////////
   ROOT::TThreadExecutor pool;
   auto res = TExecutorPoolTest(pool);
   if (res!=0)
      return res;
  ///////////////////////////////////////

   /***** chunking tests *****/

  auto redfunc = [](std::vector<int> a) -> int { return std::accumulate(a.begin(), a.end(), 0); };

  // init list and C++ function
   auto chunkedredres1 = pool.MapReduce(f, {0,1,0,1}, redfunc, 2);
   if(chunkedredres1 != 6)
      return 16;

  //nTimes + lambda signature
   auto chunkedredres2 = pool.MapReduce([](){return 1;}, 6, redfunc, 3);
   if(chunkedredres2 != 6)
      return 17;

  //TSeq signature, uneven chunks (8 elements in 3 chunks)
   auto chunkedredres3 = pool.MapReduce(f, ROOT::TSeq<int>(0,6), redfunc, 2);
   if(chunkedredres3 != 21)
      return 18;

  /***** Foreach tests *****/

  std::atomic<unsigned> i{};
  auto lambdaNTimes = [&](){i++;};

   pool.Foreach(lambdaNTimes, 4);
   if(i!=4)
      return 19;

   std::vector<int> vec{1,1,1,1};
   pool.Foreach([&](int &i){i=2;}, vec);
   if(redfunc(vec) != 8)
      return 20;
   return 0;
}

int main() {
	return PoolTest();
}
