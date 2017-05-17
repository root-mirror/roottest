#ifndef test_classes_h
#define test_classes_h

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <bitset>

#include "TNamed.h"
#include "TBox.h"
#include "TRef.h"
#include "TArrayI.h"
#include "Riostream.h"

class TJsonEx1 {
   protected:
     bool       fBool;
     char       fChar;
     short      fShort;
     int        fInt;      // *OPTION={SetMethod="SetI";GetMethod="GetI"}
     long       fLong;     // *OPTION={SetMethod="SetLong";GetMethod="GetLong"}
     float      fFloat;
     double     fDouble;

   public:
     int        GetI() { return fInt; }
     long       GetLong() { return fLong; }

     void       SetI(int zn) { fInt = zn; }
     void       SetLong(long zn) { fLong = zn; }

     TJsonEx1()
     {
        fBool = false;
        fChar = 'C';
        fShort = 123;
        fInt = 123456;
        fLong = 7654321;
        fFloat = 1.2;
        fDouble = 3.8;
     }
     virtual ~TJsonEx1() {}

     bool operator<(const TJsonEx1& ex) const {
        return fInt < ex.fInt;
     }


     virtual void Print()
     {
         cout << "   fBool = " << fBool << endl;
         cout << "   fChar = " << fChar << endl;
         cout << "   fShort = " << fShort << endl;
         cout << "   fInt = " << fInt << endl;
         cout << "   fLong = " << fLong << endl;
         cout << "   fFloat = " << fFloat << endl;
         cout << "   fDouble = " << fDouble << endl;
     }

     void SetValues(int n) {
        fBool = (n % 2 != 0);
        fChar = (char) (n % 128);
        fShort = (short) (n % 0x8000);
        fInt = n;
        fLong = n*2;
        fFloat = n*1.1;
        fDouble = n*7.7;
     }

};

class TJsonEx11 : public TJsonEx1 {
   public:

      int        fInt2;

      TJsonEx11() : TJsonEx1() {
         fInt2 = 22;
      }
};

// _______________________________________________________________

class TJsonEx2 {
   protected:
     int        fTest1[4];
     int        fTest2[2][2];
     bool       fBool[2][3][4];
     char       fChar[2][3][4];
     short      fShort[2][3][4];
     int        fInt[2][3][4];
     long       fLong[2][3][4];
     float      fFloat[2][3][4];
     double     fDouble[2][3][4];
   public:
     TJsonEx2()
     {
        fTest1[0] = 11111;
        fTest1[1] = 22222;
        fTest1[2] = 33333;
        fTest1[3] = 44444;

        fTest2[0][0] = 1;
        fTest2[0][1] = 2;
        fTest2[1][0] = 3;
        fTest2[1][1] = 4;

        for (int i=0;i<2;i++)
          for (int j=0;j<3;j++)
            for (int k=0;k<4;k++) {
               fBool[i][j][k] = (i+j) % 2 !=0;
               fChar[i][j][k] = 48 + i+j+k;
               fShort[i][j][k] = i+j+k;
               fInt[i][j][k] = i*j+k;
               fLong[i][j][k] = i*j*k;
               fFloat[i][j][k] = i*j*k;
               fDouble[i][j][k] = i*j*k;
            }
     }

     virtual ~TJsonEx2() {}

     void Print()
     {
         for (int i=0;i<4;i++)
           cout << "   fTest1[" << i << "] = " << fTest1[i] << endl;

         for (int i=0;i<2;i++)
         for (int j=0;j<3;j++)
         for (int k=0;k<4;k++) {
            cout << "  for indexes ["<<i<<"]["<<j<<"]["<<k<<"]" << endl;
            cout << "     fBool = " << fBool[i][j][k] << endl;
            cout << "     fChar = " << fChar[i][j][k] << endl;
            cout << "     fShort = " << fShort[i][j][k] << endl;
            cout << "     fInt = " << fInt[i][j][k] << endl;
            cout << "     fLong = " << fLong[i][j][k] << endl;
            cout << "     fFloat = " << fFloat[i][j][k] << endl;
            cout << "     fDouble = " << fDouble[i][j][k] << endl;
         }
     }
};

// _______________________________________________________________

class TJsonEx3 {
   protected:
     int        fSize;
     bool       *fBool;     // [fSize]
     char       *fChar;     // [fSize]
     short      *fShort;    // [fSize]
     int        *fInt;      // [fSize]
     long       *fLong;     // [fSize]
     float      *fFloat;    // [fSize]
     double     *fDouble;   // [fSize]
   public:

     TJsonEx3()
     {
        fSize = 5;
        fBool = new bool[fSize];
        fChar = new char[fSize];
        fShort = new short[fSize];
        fInt = new int[fSize];
        fLong = new long[fSize];
        fFloat = new float[fSize];
        fDouble = new double[fSize];

        for (int n=0;n<fSize;n++) {
           fBool[n] = false;
           fChar[n] = 49 + n;
           fShort[n] = n*2;
           fInt[n] = n*5;
           fLong[n] = n*123;
           fFloat[n] = n*3;
           fDouble[n] = n*7;
        }

     }
     virtual ~TJsonEx3()
     {
       //delete[] fBool;
       //delete[] fChar;
       // delete[] fShort;
       // delete[] fInt;
       // delete[] fLong;
       // delete[] fFloat;
       // delete[] fDouble;
     }

     void Print()
     {
         for (int i=0;i<fSize;i++) {
            cout << "  index = " << i << endl;
            cout << "     fBool = " << fBool[i] << endl;
            cout << "     fChar = " << fChar[i] << endl;
            cout << "     fShort = " << fShort[i] << endl;
            cout << "     fInt = " << fInt[i] << endl;
            cout << "     fLong = " << fLong[i] << endl;
            cout << "     fFloat = " << fFloat[i] << endl;
            cout << "     fDouble = " << fDouble[i] << endl;
         }
     }

};

// _______________________________________________________________

class TJsonEx4 : public TJsonEx1 {
   protected:
      char        fStr1[100];
      const char* fStr2;
      int         fDummy2;
      const char* fStr3;
      const char* fStr4;
   public:
      TJsonEx4(bool setvalues = false) : TJsonEx1()
      {
        memset(fStr1, 0, sizeof(fStr1));
        fStr2 = 0;
        fDummy2 = 0;
        fStr3 = 0;
        fStr4 = 0;
        if (setvalues) {
           strcpy(fStr1, "Value of string 1");
           fDummy2 = 1234567;
           fStr3 = new char[1000];
           strcpy((char*)fStr3, "***\t\n/t/n************** Long Value of string 3 *****************************************************************************************************************************************************************************************************************************************************************************************************************");
           fStr4 = new char[1000];
           strcpy((char*)fStr4, "--- normal string value ---");
        }
      }
      virtual ~TJsonEx4()
      {
         delete[] fStr2;
         delete[] fStr3;
         delete[] fStr4;
      }

      void Print()
      {
          TJsonEx1::Print();
          cout << "   fStr1 = " << fStr1 << endl;
          cout << "   fStr2 = " << (fStr2 ? fStr2 : "null") << endl;
          cout << "   fDummy2 = " << fDummy2 << endl;
          cout << "   fStr3 = " << fStr3 << endl;
      }
};

// _______________________________________________________________

class TJsonEx5 {
  protected:

     TJsonEx1    fObj1;
     TJsonEx2    fObj2;

     TJsonEx1    *fPtr1;
     TJsonEx2    *fPtr2;

     TJsonEx1    *fSafePtr1;   //->
     TJsonEx2    *fSafePtr2;   //->

   public:
     TJsonEx1    fObj3;
     TJsonEx1    *fPtr3;
     TJsonEx1    *fSafePtr3;   //->

     TJsonEx5(bool setvalues = false)
     {
        fPtr1 = 0;
        fPtr2 = 0;
        fPtr3 = 0;

        fSafePtr1 = new TJsonEx1;
        fSafePtr2 = new TJsonEx2;
        fSafePtr3 = new TJsonEx1;

        if (setvalues) {
           fPtr1 = new TJsonEx1;
           fPtr2 = new TJsonEx2;
           fPtr3 = fPtr1;
        }
     }

     virtual ~TJsonEx5()
     {
        delete fSafePtr1;
        delete fSafePtr2;
        delete fSafePtr3;
     }

     TJsonEx2&  GetObj2() { return fObj2; }
     TJsonEx2*  GetPtr2() { return fPtr2; }
     void      SetPtr2(TJsonEx2* ptr) { fPtr2 = ptr; }
     TJsonEx2*  GetSafePtr2() { return fSafePtr2; }


     void Print()
     {
        cout << endl << "!!!!!!!!!! fObj1 !!!!!!!!!!!" << endl;
        fObj1.Print();

        cout << endl << "!!!!!!!!!! fObj2 !!!!!!!!!!!" << endl;
        fObj2.Print();

        cout << endl << "!!!!!!!!!! fObj3 !!!!!!!!!!!" << endl;
        fObj3.Print();

        cout << endl << "!!!!!!!!!! fPtr1 !!!!!!!!!!!" << endl;
        if (fPtr1) fPtr1->Print();

        cout << endl << "!!!!!!!!!! fPtr2 !!!!!!!!!!!" << endl;
        if (fPtr2) fPtr2->Print();

        cout << endl << "!!!!!!!!!! fPtr3 !!!!!!!!!!!" << endl;
        if (fPtr3) fPtr3->Print();

        cout << endl << "!!!!!!!!!! fSafePtr1 !!!!!!!!!!!" << endl;
        if (fSafePtr1) fSafePtr1->Print();

        cout << endl << "!!!!!!!!!! fSafePtr2 !!!!!!!!!!!" << endl;
        if (fSafePtr2) fSafePtr2->Print();

        cout << endl << "!!!!!!!!!! fSafePtr3 !!!!!!!!!!!" << endl;
        if (fSafePtr3) fSafePtr3->Print();
     }
};


// _______________________________________________________________


class TJsonEx6 {
  protected:

     TJsonEx1    fObj1[3];
     TJsonEx2    fObj2[3];

     TJsonEx1*   fPtr1[3];
     TJsonEx2*   fPtr2[3];

     TJsonEx1*   fSafePtr1[3];   //->
     TJsonEx2*   fSafePtr2[3];   //->

   public:
     TJsonEx1    fObj3[3];
     TJsonEx1*   fPtr3[3];

     TJsonEx1*   fSafePtr3[3];   //->
     TJsonEx2*   fSafePtr33[3][3];   //->
     std::string  fStringArr234[2][3][4];

     TJsonEx2*   GetObj2() { return fObj2; }
     TJsonEx2**  GetPtr2() { return fPtr2; }
     TJsonEx2**  GetSafePtr2() { return fSafePtr2; }

     TJsonEx6(bool setvalues = false)
     {
        for (int n=0;n<3;n++) {
           fPtr1[n] = 0;
           fPtr2[n] = 0;
           fPtr3[n] = 0;

           fSafePtr1[n] = new TJsonEx1();
           fSafePtr2[n] = new TJsonEx2();
           fSafePtr3[n] = new TJsonEx1();
           for (int k=0;k<3;k++)
              fSafePtr33[n][k] = new TJsonEx2();
         }

         if (setvalues) {
            for (int n=0;n<3;n++) {
              fPtr1[n] = new TJsonEx1();
              fPtr2[n] = new TJsonEx2();
              fPtr3[n] = fPtr1[n];
            }

            for (int n=0;n<2;n++)
              for (int k=0;k<3;k++)
                 for (int j=0;j<4;j++)
                  fStringArr234[n][k][j] = Form("%d-%d-%d",n,k,j);

         }

     }
     virtual ~TJsonEx6()
     {
     }
     void Print()
     {
        for (int n=0;n<3;n++) {
           cout << endl << "!!!!!!!!!! fObj1["<<n<<"] !!!!!!!!!!!" << endl;
           fObj1[n].Print();

           cout << endl << "!!!!!!!!!! fObj2["<<n<<"] !!!!!!!!!!!" << endl;
           fObj2[n].Print();

           cout << endl << "!!!!!!!!!! fObj3["<<n<<"] !!!!!!!!!!!" << endl;
           fObj3[n].Print();

           cout << endl << "!!!!!!!!!! fPtr1["<<n<<"] !!!!!!!!!!!" << endl;
           if (fPtr1[n]) fPtr1[n]->Print();

           cout << endl << "!!!!!!!!!! fPtr2["<<n<<"] !!!!!!!!!!!" << endl;
           if (fPtr2[n]) fPtr2[n]->Print();

           cout << endl << "!!!!!!!!!! fPtr3["<<n<<"] !!!!!!!!!!!" << endl;
           if (fPtr3[n]) fPtr3[n]->Print();

           cout << endl << "!!!!!!!!!! fSafePtr1["<<n<<"] !!!!!!!!!!!" << endl;
           if (fSafePtr1[n]) fSafePtr1[n]->Print();

           cout << endl << "!!!!!!!!!! fSafePtr2["<<n<<"] !!!!!!!!!!!" << endl;
           if (fSafePtr2[n]) fSafePtr2[n]->Print();

           cout << endl << "!!!!!!!!!! fSafePtr3["<<n<<"] !!!!!!!!!!!" << endl;
           if (fSafePtr3[n]) fSafePtr3[n]->Print();
        }
     }
};

// _______________________________________________________________


class TJsonEx7 {
   public:
      std::string            fStr1;
      std::string            fStr2;
      std::string           *fStrPtr1;
      std::string           *fStrPtr2;

      std::string            fStrArr[3];
      std::string           *fStrPtrArr[3];

      bool                        fBoolArr[10];
      std::vector<double>         fVectDouble;
      std::vector<bool>           fVectBool;
      std::vector<double>         fVectDoubleArr[3];

      std::vector< double >      *fVectPtrDouble;
      std::vector< double >      *fVectPtrDoubleArr[3];

      std::vector<TJsonEx1>        fVectEx1;
      std::vector<TJsonEx1*>       fVectEx1Ptr;
      std::vector<TJsonEx2>        fVectEx2;
      std::vector<TJsonEx3>        fVectEx3;

      std::vector<TBox>           fVectBox;

      std::vector<TNamed>         fVectNames;

      std::vector<TJsonEx11>      fVectEx11;

      std::vector<std::string>    fVectString;
      std::vector<std::string*>   fVectStringPtr;

      std::list<double>           fListDouble;
      std::list<bool>             fListBool;
      std::list<TJsonEx1>         fListEx1;
      std::list<TJsonEx1*>        fListEx1Ptr;

      std::deque<double>          fDequeDouble;
      std::deque<bool>            fDequeBool;
      std::deque<TJsonEx1>        fDequeEx1;
      std::deque<TJsonEx1*>       fDequeEx1Ptr;

      std::map<TString,int>       fMapStrInt;
      std::map<int,double>        fMapIntDouble;
      std::map<int,TJsonEx1>      fMapIntEx1;
      std::map<int,TJsonEx1*>     fMapIntEx1Ptr;
      std::multimap<int,double>   fMultimapIntDouble;

      std::set<double>            fSetDouble;
      std::multiset<double>       fMultisetDouble;

      std::bitset<16>             fBitsSet16;
      std::bitset<64>             fBitsSet64;


   TJsonEx7(bool setvalues = false)
   {
      fStrPtr1 = 0;
      fStrPtr2 = 0;

      fVectPtrDouble = 0;
      for (int n=0;n<3;n++) {
        fStrPtrArr[n] = 0;
        fVectPtrDoubleArr[n] = 0;
      }

      if (!setvalues) return;

      fStr1 = "String with special characters: \" & < >";
      fStr2 = "Very long Value of STL string// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8// ***********************************************************8";

      fStrArr[0] = "Value of fStrArr[0]";
      fStrArr[1] = "Value of fStrArr[1]";
      fStrArr[2] = "Value of fStrArr[2]";

      fStrPtr1 = new string("Value of < >  &lt; &gt; string fStrPtr1");
      fStrPtr2 = 0; // new string("Value of string fStrPtr2");

      fStrPtrArr[0] = new string("value of fStrPtrArr[0]");
      fStrPtrArr[1] = new string("value of fStrPtrArr[1]");
      fStrPtrArr[2] = new string("value of fStrPtrArr[2]");

      for (int n=0;n<10;n++) {
         fVectDouble.push_back(n*3);
         fBoolArr[n] = (n%2 == 1);
         fVectBool.push_back(n%2 == 1);

         for (int k=0;k<3;++k)
            fVectDoubleArr[k].push_back(k*10+n);

         fVectEx1.push_back(TJsonEx1());
         fVectEx2.push_back(TJsonEx2());
         fVectEx3.push_back(TJsonEx3());

         fVectEx11.push_back(TJsonEx11());
      }

      for (int n=0;n<3;n++) {
        TJsonEx1 *ex1 = new TJsonEx1;
        fVectEx1Ptr.push_back(ex1);
        fVectEx1Ptr.push_back(ex1);
        fVectEx1Ptr.push_back(ex1);
      }

      fVectPtrDouble = new std::vector<double>;
      for (int n=0;n<10;n++)
         fVectPtrDouble->push_back(n*3);

      for (int i=0;i<3;i++) {
         fVectPtrDoubleArr[i] = new std::vector<double>;
         for (int n=0;n<10;n++)
            fVectPtrDoubleArr[i]->push_back(i*100 + n*3);
      }

      for (int n=0;n<10;n++) {
         TBox box(n*11,n*22,n*33,n*44);
         fVectBox.push_back(box);

         TNamed name(Form("Name%d",n),Form("Title%d",n));
         fVectNames.push_back(name);
      }

      for (int n=0;n<5;n++) {
        fVectString.push_back(Form("string %d content",n));
        fVectStringPtr.push_back(new string(Form("string pointer %d content",n)));
      }


      for (int n=0;n<10;n++) {

         fVectDoubleArr[0].push_back(n*3);
         fVectDoubleArr[1].push_back(n*3);
         fVectDoubleArr[2].push_back(n*3);

         fListDouble.push_back(n*4);
         fListBool.push_back(n%2 == 1);
         fListEx1.push_back(TJsonEx1());
         fListEx1Ptr.push_back(new TJsonEx1);

         fDequeDouble.push_back(n*4);
         fDequeBool.push_back(n%2 == 1);
         fDequeEx1.push_back(TJsonEx1());
         fDequeEx1Ptr.push_back(new TJsonEx1);

         fMapStrInt[Form("Str%d",n)] = n + 10;
         fMapIntDouble[n] = n*5;
         fMapIntEx1[n] = TJsonEx1();
         fMapIntEx1Ptr[n] = &(fMapIntEx1[n]); // new TJsonEx1;

         fMultimapIntDouble.insert(pair<int,double>(n,n*6));
         fMultimapIntDouble.insert(pair<int,double>(n,1000+n*6));

         fSetDouble.insert(n);
         fMultisetDouble.insert(n);
      }

      for (int k=0;k<16;++k)
         fBitsSet16.set(k, k%2);

      for (int k=0;k<64;++k)
        fBitsSet64.set(k, k%3);

   }


   virtual ~TJsonEx7()
   {
   }

   void Print()
   {
      cout << "Do not print everything, just something..." << endl;

      cout << " fStr1 = " << fStr1 << endl;

      cout << " fStrArr[1] = " << fStrArr[1] << endl;

      cout << " fVectEx1.back().Print()" << endl;
      fVectEx1.back().Print();

      cout << " fVectEx1Ptr.size() = " << fVectEx1Ptr.size() << endl;

      cout << " fVectStringPtr.size() = " << fVectStringPtr.size() << endl;

      cout << " fMapIntEx1Ptr.size() = " << fMapIntEx1Ptr.size() << endl;
   }
};

// _______________________________________________________________

class TJsonEx8 : public std::vector<int> {
   public:
     int                    fInt;
     std::string            fStdString;

   TJsonEx8(bool setvalues = false)
   {
      if (setvalues) {
         for (int n=0;n<10;n++)
           push_back(n*14);
         fInt = 12345;
         fStdString = "Value of STL string";
      }

   }
   virtual ~TJsonEx8() {}

   void Print()
   {
      cout << "vector size = " << size() << endl;
      for (unsigned n=0;n<size(); n++)
        cout << "vector.at(" << n << ") = " << at(n) << endl;
      cout << " fInt = " << fInt << endl;
      cout << " fStdString = " << fStdString << endl;
   }
};

//__________________________________________________________________

class TJsonEx9 {
   public:
      int fCnt;
      TString* fStr;     //[fCnt]
      TString* fStr2[3]; //[fCnt]
//      TString** fStr3;   //[fCnt]
      TString* fStr4[2][5]; //[fCnt]
      TString** fStr6[3]; //[fCnt]
      TNamed* fNm2[3]; //[fCnt]
      TNamed* fNm4[2][5]; //[fCnt]
      TNamed** fNm6[3]; //[fCnt]

      TArrayI* fArr2[3]; //[fCnt]
      TArrayI* fArr4[2][5]; //[fCnt]
      TArrayI** fArr6[3]; //[fCnt]

   TJsonEx9()
   {
      fCnt = 0;
      fStr = 0;
      for (int k=0;k<3;++k) {
         fStr2[k] = 0;
         fNm2[k] = 0;
         fStr6[k] = 0;
         fNm6[k] = 0;
         fArr2[k] = 0;
         fArr6[k] = 0;
      }
      for (int k1=0;k1<2;++k1)
         for (int k2=0;k2<5;++k2) {
            fStr4[k1][k2] = 0;
            fNm4[k1][k2] = 0;
            fArr4[k1][k2] = 0;
         }
//      fStr3 = 0;
   }

   void SetValues(int cnt = 3) {
      fCnt = cnt;

      fStr = new TString[fCnt];
//      fStr3 = new TString*[fCnt];
      for (int n=0;n<fCnt;++n) {
         fStr[n].Form("String%d",n);
/*         if (n%3==2) {
            fStr3[n] = 0;
         } else {
            fStr3[n] = new TString();
            fStr3[n]->Form("DynamicString%d",n);
         }
*/
      }

      for (int k=0;k<3;++k) {
         fStr2[k] = new TString[fCnt];
         for (int n=0;n<fCnt;++n)
            fStr2[k][n].Form("String[%d][%d]",k,n);
         fNm2[k] = new TNamed[fCnt];
         for (int n=0;n<fCnt;++n) {
            fNm2[k][n].SetName(Form("Name[%d][%d]",k,n));
            fNm2[k][n].SetTitle(Form("Title[%d][%d]",k,n));
         }
         fArr2[k] = new TArrayI[fCnt];
         for (int n=0;n<fCnt;++n) {
            fArr2[k][n].Set(5);
            fArr2[k][n].Reset(n);
         }

         fStr6[k] = new TString*[fCnt];
         for (int n=0;n<fCnt;++n) {
            fStr6[k][n] = 0;
            if ((k+n) % 3 == 0) continue;

            fStr6[k][n] = new TString;
            fStr6[k][n]->Form("String[%d][%d]",k,n);
         }

         fNm6[k] = new TNamed*[fCnt];
         for (int n=0;n<fCnt;++n) {
            fNm6[k][n] = 0;
            if ((k+n) % 3 == 0) continue;

            fNm6[k][n] = new TNamed;
            fNm6[k][n]->SetName(Form("Name[%d][%d]",k,n));
            fNm6[k][n]->SetTitle(Form("Title[%d][%d]",k,n));
         }

         fArr6[k] = new TArrayI*[fCnt];
         for (int n=0;n<fCnt;++n) {
            fArr6[k][n] = 0;
            if ((k+n) % 3 == 0) continue;

            fArr6[k][n] = new TArrayI(5);
            fArr6[k][n]->Reset(n);
         }

      }

      for (int k1=0;k1<2;++k1)
         for (int k2=0;k2<5;++k2) {
            fStr4[k1][k2] = new TString[fCnt];
            fNm4[k1][k2] = new TNamed[fCnt];
            fArr4[k1][k2] = new TArrayI[fCnt];
            for (int n=0;n<fCnt;++n) {
               fStr4[k1][k2][n].Form("String[%d][%d][%d]",k1,k2,n);
               fNm4[k1][k2][n].SetName(Form("Name[%d][%d][%d]",k1,k2,n));
               fNm4[k1][k2][n].SetTitle(Form("Name[%d][%d][%d]",k1,k2,n));
               fArr4[k1][k2][n].Set(5);
               fArr4[k1][k2][n].Reset(n);
            }
         }

   }

   virtual ~TJsonEx9() {
   //   if (fStr) { delete [] fStr; fStr = 0; }
   }
};

// _________________________________________________________________________

class TJsonEx10 {
   public:
      TString fStr0;
      TNamed  fName0;
      TObject fObj0;

      TString fStr[10];
      TNamed  fName[10];
      TObject fObj[10];

      TJsonEx10() {}
      virtual ~TJsonEx10() {}

      void SetValues() {
         fStr0 = "SimpleString";
         fName0.SetName("SimpleName");
         fName0.SetTitle("SimpleTitle");
         fObj0.SetUniqueID(10000);

         for (int n=0;n<10;++n) {
            fStr[n].Form("String%d",n);
            fName[n].SetName(Form("Name%d",n));
            fName[n].SetTitle(Form("Title%d",n));
            fObj[n].SetUniqueID(n+100);
         }
      }
};

// ______________________________________________________________________________________

class TJsonEx12  {
public:
   std::vector<TJsonEx1> vect1;
   std::vector<TJsonEx9> vect9;

   TJsonEx12() : vect1(), vect9() {}
   virtual ~TJsonEx12() {}

    void SetValues(int cnt) {
      vect1.clear();
      vect9.clear();
      TJsonEx1 ex1;
      TJsonEx9 ex9;
      for (int n=0;n<cnt;++n) {
         if (n>0) ex9.SetValues(n);
         vect1.push_back(ex1);
         vect9.push_back(ex9);
      }
   }


};

// ______________________________________________________________________________________

class TJsonEx13  {
public:
   std::set<TJsonEx1> set1;
   std::map<TJsonEx1, std::set<TJsonEx1>> map1;
   std::map<int, TRef> map2;
   std::vector<TRef> vect1;

   TJsonEx13() : set1(), map1(), map2(), vect1() {}
   virtual ~TJsonEx13() {}

   void SetValues(int cnt) {
      set1.clear();
      map1.clear();
      map2.clear();
      vect1.clear();
      TJsonEx1 ex1;
      for (int n=0;n<cnt;++n) {
         TRef ref;
         ex1.SetValues(n);
         set1.insert(ex1);
         map1[ex1] = set1;
         map2[n] = ref;
         vect1.push_back(ref);
      }
   }

};


#endif
