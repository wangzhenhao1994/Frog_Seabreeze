#include "TMatrixD.h"
void test(){
  TMatrixD S(1,100);
  TMatrixD D(1,100);
  S=S*double(1.00/100)+1.00;
  TMatrixDRow(S,0)=TMatrixDRow(D,0);
  S.Print();
}
