#include "EpInfo.h"

#include <cmath>  // for fabs, M_PI
#include <iostream>

void EpInfo::Reset()
{
  for (auto &vec: QrawOneSide)
  {
    std::fill(vec.begin(),vec.end(),NAN);
  }
  std::fill(PsiRaw.begin(),PsiRaw.end(),NAN);
  std::fill(WheelSumWeightsRaw.begin(),WheelSumWeightsRaw.end(),NAN);
}

// ===================== Access to Q-vectors ==========================

//------------------------------ Raw Q --------------------------------
std::pair<double, double> EpInfo::RawQ(unsigned int order)
{
  if (ArgumentOutOfBounds(order))
  {
    return std::make_pair(NAN,NAN);
  }
  return std::make_pair(QrawOneSide[order - 1][0], QrawOneSide[order - 1][1]);
}

// --------------------- Wheel sum-of-weights, raw ----------------------
double EpInfo::SWRaw(unsigned int order)
{
  if (ArgumentOutOfBounds(order)) return NAN;
  return WheelSumWeightsRaw[order - 1];
}

// ===================== Access to Event-plane angles ====================

//------------------------- raw EP angles --------------------------------
double EpInfo::RawPsi(unsigned int order)
{
  if (ArgumentOutOfBounds(order)) return NAN;
  return Range(PsiRaw.at(order - 1), order);
}
//-----------------------------------------------------------------------

//----- Simple method to put angles in a convenient range: (0,2pi/n) ----
double EpInfo::Range(double psi, unsigned int order)
{
  if (ArgumentOutOfBounds(order)) return NAN;
  double wrap = (2.0 * M_PI) / (double) order;
  if (psi < 0.0)
  {
    psi += (1.0 + (int) (fabs(psi) / wrap)) * wrap;
  }
  else
  {
    if (psi > wrap) psi -= ((int) (psi / wrap)) * wrap;
  }
  return psi;
}

//--------- protection against argument out-of-bounds -------
bool EpInfo::ArgumentOutOfBounds(unsigned int order)
{
  if ((order < 1) || ((unsigned int)order > MaxOrder()))
  {
    std::cout << "\n *** Invalid order requested " << order << "***\n";
    std::cout << "  order must be between 1 (for first-order EP) and " << MaxOrder()
              << ".    To change the upuper limit, edit StEpdUtil/EpInfo.h\n";
    std::cout << "  I will now return you an invalid result.  Have a nice day\n";
    return true;
  }
  return false;
}

void EpInfo::CopyPsiRaw(const std::vector<double> &vec)
{
  if (PsiRaw.size() != vec.size())
  {
    PsiRaw.resize(vec.size());
  }
PsiRaw = vec;
}

void EpInfo::CopyWheelSumWeightsRaw(const std::vector<double> &vec)
{
  if (WheelSumWeightsRaw.size() != vec.size())
  {
    WheelSumWeightsRaw.resize(vec.size());
  }
WheelSumWeightsRaw = vec;
}

void EpInfo::CopyQrawOneSide(const std::vector<std::vector<double>> &vecvec)
{
  if (QrawOneSide.size() != vecvec.size())
  {
  QrawOneSide.resize(vecvec.size());
  int i = 0;
  for (auto &vec: vecvec)
  {
    QrawOneSide.at(i).resize(vec.size());
    i++;
  }
  }
QrawOneSide = vecvec;
}
