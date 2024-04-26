#include "L1Trigger/L1THGCal/interface/backend_emulator/HGCalStage1Emulator_SA.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace l1thgcfirmware;

HGCalStage1EmulatorSA::HGCalStage1EmulatorSA() {}

struct {
   bool operator()(l1thgcfirmware::HGCalTriggerCell a, l1thgcfirmware::HGCalTriggerCell b) const { return a.phi() < b.phi(); }
} sortByPhi;

unsigned HGCalStage1EmulatorSA::run(const l1thgcfirmware::HGCalTriggerCellSACollection& tcs_in,
                                          const l1thgcfirmware::Stage1TruncationConfig& theConf,
                                          l1thgcfirmware::HGCalTriggerCellSACollection& tcs_out) const {
  std::unordered_map<std::tuple<int,unsigned,unsigned,unsigned>, std::vector<l1thgcfirmware::HGCalTriggerCell>> tcs_per_bin; //tuple is z side, layer, waferu, waferv

 
  // group TCs per unique module
  for (const auto& tc : tcs_in) {
    unsigned layer = tc.layer();
    int zside = tc.zside();
    unsigned waferu = tc.waferU();
    unsigned waferv = tc.waferV();

    //Correct packing is still missing, eventually should follow something like this:
    //unsigned packed_bin = packBin(layer, phibin);
    //tcs_per_bin[packed_bin].push_back(tc);
    
    tcs_per_bin[{zside,layer,waferu,waferv}].push_back(tc); //Just to have something running. Should be properly packed
  }

  for (auto& bin_tcs : tcs_per_bin) {
    unsigned layer = std::get<1>(bin_tcs.first);
    unsigned waferv = std::get<3>(bin_tcs.first);
    int zside = std::get<0>(bin_tcs.first);
    unsigned waferu = std::get<2>(bin_tcs.first);
    
    std::vector<l1thgcfirmware::HGCalTriggerCell> sorted_tcs;

    sorted_tcs = bin_tcs.second;

    std::sort(sorted_tcs.begin(), sorted_tcs.end(),sortByPhi);

    std::vector<std::pair<unsigned,std::vector<unsigned>>> cols_and_frames; //Map of columns and frames. Pretend they're the same for each module. 
    for (unsigned i=0; i<20; ++i){
       std::vector<unsigned> theFrames;
       theFrames.push_back(i+3);
       theFrames.push_back(i+4);
       theFrames.push_back(i+5);
       cols_and_frames.push_back(std::make_pair(i,theFrames));
    }//Dummy: 20 columns, 0-9, with n+3 and n+4 as frames (only 3 frames). This information should come from the mapping files and should be set in the config file, not here. 
    unsigned maxTCsPerFrame=4;

    unsigned cellCounter=0; //number of trigger cells per column
    unsigned cellframeCounter=0;//number of trigger cells per frame
    unsigned colCounter=0;
    unsigned frameCounter=0;

    
    for (auto& tcs : sorted_tcs){
        if(cellCounter>=maxTCsPerFrame*cols_and_frames.at(colCounter).second.size()){
          cellCounter=0;
          cellframeCounter=0;
          frameCounter=0;
          colCounter+=1;
          if(colCounter>cols_and_frames.size()) std::cout<<"ERROR, NOT ENOUGH COLUMNS FOR ALL THE TCS"<<std::endl;//Need to handle this better, of course
        }
        if(cellframeCounter>=maxTCsPerFrame){
          cellframeCounter=0;
          frameCounter+=1;
        }
      tcs_out.push_back(tcs);
      tcs_out.back().setColumn(cols_and_frames.at(colCounter).first);
      tcs_out.back().setFrame(cols_and_frames.at(colCounter).second.at(frameCounter));
      cellCounter+=1;
      cellframeCounter+=1;
    }
  }

  return 0;
}

//These functions are not used for the moment, copied from the previous emulator, as some of them may be needed in the future 
uint32_t HGCalStage1EmulatorSA::packBin(unsigned roverzbin, unsigned phibin) const {
  unsigned packed_bin = 0;
  packed_bin |= ((roverzbin & mask_roz_) << offset_roz_);
  packed_bin |= (phibin & mask_phi_);
  return packed_bin;
}

void HGCalStage1EmulatorSA::unpackBin(unsigned packedbin, unsigned& roverzbin, unsigned& phibin) const {
  roverzbin = ((packedbin >> offset_roz_) & mask_roz_);
  phibin = (packedbin & mask_phi_);
}

int HGCalStage1EmulatorSA::phiBin(unsigned roverzbin, double phi, const std::vector<double>& phiedges) const {
  unsigned phi_bin = 0;
  if (roverzbin >= phiedges.size())
    return -1;
  double phi_edge = phiedges[roverzbin];
  if (phi > phi_edge)
    phi_bin = 1;
  return phi_bin;
}

double HGCalStage1EmulatorSA::rotatedphi(double phi, unsigned sector) const {
  if (sector == 1) {
    if (phi < M_PI and phi > 0)
      phi = phi - (2. * M_PI / 3.);
    else
      phi = phi + (4. * M_PI / 3.);
  } else if (sector == 2) {
    phi = phi + (2. * M_PI / 3.);
  }
  return phi;
}

unsigned HGCalStage1EmulatorSA::smallerMultOfFourGreaterThan(unsigned N) const {
  unsigned remnant = (N + 4) % 4;
  if (remnant == 0)
    return N;
  else
    return (N + 4 - remnant);
}
