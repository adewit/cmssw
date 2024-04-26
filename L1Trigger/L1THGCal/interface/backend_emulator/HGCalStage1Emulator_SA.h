#ifndef __L1Trigger_L1THGCal_HGCalStage1Emulator_SA_h__
#define __L1Trigger_L1THGCal_HGCalStage1Emulator_SA_h__

#include "L1Trigger/L1THGCal/interface/backend_emulator/HGCalTriggerCell_SA.h"
#include "L1Trigger/L1THGCal/interface/backend_emulator/HGCalStage1TruncationConfig_SA.h"
#include "L1Trigger/L1THGCal/interface/backend_emulator/HGCalStage1SortingAlg_SA.h"

#include <vector>
#include <cstdint>        // uint32_t, unsigned
#include <unordered_map>  // std::unordered_map

namespace l1thgcfirmware {

  class HGCalStage1EmulatorSA {
  public:
    HGCalStage1EmulatorSA();
    ~HGCalStage1EmulatorSA() {}

    void runAlgorithm() const;

    unsigned run(const l1thgcfirmware::HGCalTriggerCellSACollection& tcs_in,
                 const l1thgcfirmware::Stage1TruncationConfig& theConf,
                 l1thgcfirmware::HGCalTriggerCellSACollection& tcs_out) const;

  private:
    static constexpr unsigned offset_roz_ = 1;
    static constexpr unsigned mask_roz_ = 0x3f;  // 6 bits, max 64 bins
    static constexpr unsigned mask_phi_ = 1;

    bool do_truncate_;
    double roz_min_ = 0.;
    double roz_max_ = 0.;
    unsigned roz_bins_ = 42;
    std::vector<unsigned> max_tcs_per_bin_;
    std::vector<double> phi_edges_;

    uint32_t packBin(unsigned roverzbin, unsigned phibin) const;
    void unpackBin(unsigned packedbin, unsigned& roverzbin, unsigned& phibin) const;
    int phiBin(unsigned roverzbin, double phi, const std::vector<double>& phiedges) const;
    double rotatedphi(double x, double y, double z, unsigned sector) const;
    double rotatedphi(double phi, unsigned sector) const;

    unsigned smallerMultOfFourGreaterThan(unsigned n) const;
  };

}  // namespace l1thgcfirmware

//Hopefully only needed temporarily since eventually the map will not be between tuple and number of bins. 
#include <tuple>
// function has to live in the std namespace 
// so that it is picked up by argument-dependent name lookup (ADL).
namespace std{
    namespace
    {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     https://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
            hash_combine(seed, get<Index>(tuple));
          }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            hash_combine(seed, get<0>(tuple));
          }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> 
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {                                              
            size_t seed = 0;                             
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
            return seed;                                 
        }                                              

    };
}

#endif
