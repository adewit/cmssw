#ifndef L1Trigger_L1THGCal_HGCalTriggerCell_SA_h
#define L1Trigger_L1THGCal_HGCalTriggerCell_SA_h

#include <vector>
#include <memory>
#include <iostream>
namespace l1thgcfirmware {

  class HGCalTriggerCell {
  public:
    HGCalTriggerCell() : HGCalTriggerCell(false, false, 0, 0, 0, 0) {}
    HGCalTriggerCell(
        bool frameValid, bool dataValid, unsigned int rOverZ, unsigned int phi, unsigned int layer, unsigned int energy)
        : clock_(0),
          index_(0),
          rOverZ_(rOverZ),
          layer_(layer),
          energy_(energy),
          phi_(phi),
          sortKey_(0),
          deltaR2_(0),
          deltaPhi_(0),
          column_(0),
          dX_(0),
          Y_(0),
          frameValid_(frameValid),
          dataValid_(dataValid),
          index_cmssw_(std::pair{-1, 01}) {}

    ~HGCalTriggerCell() {}

    // Setters
    void setClock(const unsigned int clock) { clock_ = clock; }
    void addLatency(const unsigned int latency) { clock_ += latency; }
    void setIndex(const unsigned int index) { index_ = index; }
    void setSortKey(const unsigned int sortKey) { sortKey_ = sortKey; }
    void setDX(const int dX) { dX_ = dX; }
    void setY(const unsigned int Y) { Y_ = Y; }
    void setDeltaR2(const unsigned int deltaR2) { deltaR2_ = deltaR2; }
    void setDeltaPhi(const unsigned int deltaPhi) {deltaPhi_ = deltaPhi; }
    void setCmsswIndex(const std::pair<int, int> index) { index_cmssw_ = index; }
    void setColumn(const unsigned int col) { column_ = col; }
    void setFrame(const unsigned int frame ) { frame_ = frame; }
    void setZside(const int zside) {zside_ = zside; }
    void setWaferU(const int waferU) {waferU_ = waferU;}
    void setWaferV(const int waferV) {waferV_ = waferV;}

    // Getters
    unsigned int clock() const { return clock_; }
    unsigned int index() const { return index_; }
    unsigned int rOverZ() const { return rOverZ_; }
    unsigned int layer() const { return layer_; }
    unsigned int energy() const { return energy_; }
    unsigned int phi() const { return phi_; }
    unsigned int sortKey() const { return sortKey_; }
    unsigned int deltaR2() const { return deltaR2_; }
    unsigned int deltaPhi() const { return deltaPhi_; }
    int dX() const { return dX_; }
    unsigned int Y() const { return Y_; }
    bool frameValid() const { return frameValid_; }
    bool dataValid() const { return dataValid_; }
    const std::pair<int, int>& cmsswIndex() const { return index_cmssw_; }
    unsigned int column() const {return column_; }
    unsigned int frame() const {return frame_; }
    int zside() const {return zside_; }
    int waferU() const {return waferU_;}
    int waferV() const {return waferV_; }

    // Operators
    bool operator==(const HGCalTriggerCell& rhs) const;
    bool operator==(const std::shared_ptr<HGCalTriggerCell>& rhs) const;

  private:
    unsigned int clock_;
    unsigned int index_;
    unsigned int rOverZ_;
    unsigned int layer_;
    unsigned int energy_;
    unsigned int phi_;
    unsigned int sortKey_;
    unsigned int deltaR2_;
    unsigned int deltaPhi_;
    unsigned int column_;
    unsigned int frame_;
    int dX_;
    unsigned int Y_;
    bool frameValid_;
    bool dataValid_;
    int zside_;
    int waferU_;
    int waferV_;
    std::pair<int, int> index_cmssw_;
  };

  typedef std::vector<HGCalTriggerCell> HGCalTriggerCellSACollection;

  typedef std::unique_ptr<HGCalTriggerCell> HGCalTriggerCellSAPtr;
  typedef std::vector<HGCalTriggerCellSAPtr> HGCalTriggerCellSAPtrCollection;
  typedef std::vector<std::vector<std::unique_ptr<HGCalTriggerCell> > > HGCalTriggerCellSAPtrCollections;
  typedef std::vector<std::vector<std::vector<std::unique_ptr<HGCalTriggerCell> > > > HGCalTriggerCellSAPtrCollectionss;

  typedef std::shared_ptr<HGCalTriggerCell> HGCalTriggerCellSAShrPtr;
  typedef std::vector<HGCalTriggerCellSAShrPtr> HGCalTriggerCellSAShrPtrCollection;
  typedef std::vector<std::vector<std::shared_ptr<HGCalTriggerCell> > > HGCalTriggerCellSAShrPtrCollections;
  typedef std::vector<std::vector<std::vector<std::shared_ptr<HGCalTriggerCell> > > >
      HGCalTriggerCellSAShrPtrCollectionss;
}  // namespace l1thgcfirmware

#endif
