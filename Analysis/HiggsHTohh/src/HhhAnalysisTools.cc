#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/HhhAnalysisTools.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/range/algorithm.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "boost/filesystem.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/th1fmorph.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "TPad.h"
#include "TROOT.h"
#include "TEfficiency.h"
#include "TEntryList.h"
#include "TMath.h"

namespace ic {

  HhhAnalysis::HhhAnalysis(ic::channel ch, std::string year, int verbosity) : ch_(ch), year_(year), verbosity_(verbosity)  {
    lumi_ = 1.;
    qcd_os_ss_factor_ = 1.06;
    using boost::range::push_back;
    // Define some sensible defaults
    sample_names_ = {
      "Data",
      "Embedded",
      "WWJetsTo2L2Nu",
      "WZJetsTo2L2Q",
      "WZJetsTo3LNu",
      "ZZJetsTo2L2Nu",
      "ZZJetsTo2L2Q",
      "ZZJetsTo4L",
      "T-tW",
      "Tbar-tW",
    };
    if (year_ == "2012") push_back(sample_names_, std::vector<std::string>{
        "TTJetsFullLept",
        "TTJetsSemiLept",
        "TTJetsHadronicExt",
        "DYJetsToTauTauSoup"});
    if (year_ == "2011") push_back(sample_names_, std::vector<std::string>{
        "TTJets",
        "DYJetsToTauTau"});

    if (ch_ != channel::em) {
      push_back(sample_names_, std::vector<std::string>{
        "Special_3_Data",
        "Special_4_Data",
        "Special_5_WJetsToLNuSoup",
        "WJetsToLNuSoup"
      });
      if (year_ == "2012") push_back(sample_names_, std::vector<std::string>{
          "DYJetsToLLSoup",
          "DYJetsToLL-LSoup",
          "DYJetsToTauTau-LSoup",
          "DYJetsToTauTau-JJSoup",
          "DYJetsToLL-JSoup"});
      if (year_ == "2011") push_back(sample_names_, std::vector<std::string>{
          "DYJetsToLL",
          "DYJetsToLL-L",
          "DYJetsToTauTau-L",
          "DYJetsToTauTau-JJ",
          "DYJetsToLL-J"});
    } else {
      push_back(sample_names_, std::vector<std::string>{
        "Special_20_Data",
        "Special_21_Data",
        "Special_22_Data",
        "Special_23_Data",
        "Special_24_Data"
      });
      if (year_ == "2012") push_back(sample_names_, std::vector<std::string>{
          "DYJetsToLLSoup"});
      if (year_ == "2011") push_back(sample_names_, std::vector<std::string>{
          "DYJetsToLL"});
    }

    // Automatically set preference for DY Soup postfix
    if (year == "2011") {
      dy_soup_ = "";
    } else {
      dy_soup_ = "Soup";
    }

    if (ch_ != channel::em) {
      // SM Categories
      alias_map_["inclusive"]         = "pt_2>30.";

      // Categories for background estimates and control plots
      alias_map_["vbf_loose"]         = "(pt_2>30. && n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0)";
      alias_map_["w_vbf_extrap_cat"]  = "(pt_2>30. && n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0)";
      alias_map_["vbf_loose_jets20"]  = "(pt_2>30. && n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0)";
      alias_map_["twojet"]            = "(n_jets>=2 && pt_2>30.)";
      alias_map_["zl_denominator"]    = "(n_jets>=2 && pt_2>30.)";
      alias_map_["1jet"]              = "(n_jets>=1 && pt_2>30.)";

    } else {
      // SM Categories
      alias_map_["inclusive"]         = "1";
      // Categories for background estimates and control plots
      alias_map_["vbf_no_cjv"]        = "(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)";
      alias_map_["vbf_loose"]         = "(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && n_bjets==0)";
      alias_map_["vbf_loose_jets20"]  = "(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && n_bjets==0)";
      alias_map_["twojet"]            = "(n_jets>=2 && n_bjets==0)";
      alias_map_["1jet"]              = "(n_jets>=1 && n_bjets==0)";
    }
    // alias_map_["prebtag"]                   =  "n_jet<=1 && ";
    // Standard MSSM Categories
    alias_map_["btag"]                      = "(n_jets<=1 && n_bjets>=1)";
    alias_map_["nobtag"]                    = "n_bjets==0";
    // Categories for background estimates and control plots
    alias_map_["btag_loose"]                = "(n_jets<=1 && n_loose_bjets>=1)";
    // New categories for optimisation of H->hh->tautaubb
    alias_map_["Sasha"]                      = "(n_prebjets>=2 && n_bjets>=1)";
    
    
    alias_map_["jpt1_20_jpt2_20_no-tag"]     = "(n_prebjets>=2)";
    alias_map_["jpt1_30_jpt2_20_no-tag"]     = "(n_prebjets>=2) && (prebjetpt_1>30 || prebjetpt_2>30)";
    alias_map_["jpt1_40_jpt2_30_no-tag"]     = "(n_prebjets>=2 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    alias_map_["jpt1_40_jpt2_20_no-tag"]     = "(n_prebjets>=2 && (prebjetpt_1>40 || prebjetpt_2>40))";
    
    
    alias_map_["jpt1_20_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679)";
    alias_map_["jpt1_30_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && (prebjetpt_1>30 || prebjetpt_2>30))";
    alias_map_["jpt1_40_jpt2_30_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    alias_map_["jpt1_40_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && (prebjetpt_1>40 || prebjetpt_2>40))";

    alias_map_["jpt1_20_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679)";
    alias_map_["jpt1_30_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>30 || prebjetpt_2>30))";
    alias_map_["jpt1_40_jpt2_30_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    alias_map_["jpt1_40_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>40 || prebjetpt_2>40))";

    alias_map_["anyjet_jpt1_20_jpt2_20_no-tag"]    = "(n_prebjets>=2)";
    alias_map_["anyjet_jpt1_30_jpt2_20_no-tag"]    = "(n_prebjets>=2 && n_jets>=1)";
    alias_map_["anyjet_jpt1_40_jpt2_30_no-tag"]    = "(n_prebjets>=2 && n_jets>=2 && jpt_1>40 )";
    alias_map_["anyjet_jpt1_40_jpt2_20_no-tag"]    = "(n_prebjets>=2 && jpt_1>40)";
    
    alias_map_["anyjet_jpt1_20_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679)";
    alias_map_["anyjet_jpt1_30_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && n_jets>=1)";
    alias_map_["anyjet_jpt1_40_jpt2_30_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && n_jets>=2 && jpt_1>40 )";
    alias_map_["anyjet_jpt1_40_jpt2_20_med-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && jpt_1>40)";
    
    alias_map_["anyjet_jpt1_20_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679)";
    alias_map_["anyjet_jpt1_30_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && n_jets>=1 )";
    alias_map_["anyjet_jpt1_40_jpt2_30_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && n_jets>=2 && jpt_1>40)";
    alias_map_["anyjet_jpt1_40_jpt2_20_twomed-tag"]    = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && jpt_1>40)";
   
    // Attempt at exclusive categories
    alias_map_["0tag"]     = "(n_prebjets>=2 && prebjetbcsv_1<0.679 && prebjetbcsv_2<0.679)";
    alias_map_["1tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2<0.679)";
    alias_map_["2tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679)";
    
    alias_map_["excl_jpt1_30_jpt2_20_no-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1<0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>30 || prebjetpt_2>30))";
    alias_map_["excl_jpt1_30_jpt2_20_med-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>30 || prebjetpt_2>30))";
    alias_map_["excl_jpt1_30_jpt2_20_twomed-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>30 || prebjetpt_2>30))";
 
    alias_map_["excl_jpt1_40_jpt2_30_no-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1<0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    alias_map_["excl_jpt1_40_jpt2_30_med-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    alias_map_["excl_jpt1_40_jpt2_30_twomed-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>40 || prebjetpt_2>40) && prebjetpt_1>30 && prebjetpt_2>30)";
    
    alias_map_["excl_jpt1_40_jpt2_20_no-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1<0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>40 || prebjetpt_2>40))";
    alias_map_["excl_jpt1_40_jpt2_20_med-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2<0.679 && (prebjetpt_1>40 || prebjetpt_2>40))";
    alias_map_["excl_jpt1_40_jpt2_20_twomed-tag"]     = "(n_prebjets>=2 && prebjetbcsv_1>0.679 && prebjetbcsv_2>0.679 && (prebjetpt_1>40 || prebjetpt_2>40))";
    
    // Selection control regions
    // Sideband region for OS W+jets extrapolation
    // options (mt_signal_region, w_os_extrap_require_os, w_ss_extrap_require_ss, vbf_w_os_extrap_requires_os)
    // option w_extrap_diagnostics
    alias_map_["sel"]                    = "mt_1<20.";
    alias_map_["w_sdb"]                  = "mt_1>70.";
    alias_map_["w_sdb_os"]               = "os";
    alias_map_["w_vbf_sdb"]              = "mt_1>60. && mt_1<120.";
    alias_map_["w_os"]                   = "os";
    alias_map_["w_vbf_os"]               = "os";
    alias_map_["w_ss"]                   = "!os";
    alias_map_["w_shape_os"]             = "os"; 

    // Sideband region for SS W+jets extrapolation
    // Sideband region for SS W+jets estimation

    if (ch_ == channel::em) {
    // Inclusive region for e-mu fake rate normalisation
      alias_map_["em_qcd_sel"]                = "os";
      // Sideband region for e-mu SS fake shape estimation
      alias_map_["ss"]                        = "!os";
      alias_map_["sel"]                       = "pzeta>-20.";
    }

    alias_map_["QCD_Eff_Sample"]    = "Special_3_Data";
    alias_map_["ZTT_Eff_Sample"]    = "Embedded";
    alias_map_["ZL_Eff_Sample"]     = "DYJetsToLL-L"+dy_soup_;
    alias_map_["ZTT_Shape_Sample"]  = "Embedded";
    alias_map_["QCD_Shape_Sample"]  = "Special_3_Data";
    alias_map_["W_Shape_Sample"]    = "WJetsToLNuSoup";

    // Samples to combine for diboson contribution
    samples_alias_map_["vv_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW"
    };
    if (ch_ == channel::em) samples_alias_map_["vv_samples"].push_back("DYJetsToLL"+dy_soup_);

    samples_alias_map_["top_samples"] = {
     "TTJetsFullLept", "TTJetsSemiLept", "TTJetsHadronicExt"
    };
    if (year == "2011") {
      samples_alias_map_["top_samples"] = { "TTJets" };
    }

    samples_alias_map_["zj_samples"] = {
     "DYJetsToTauTau-JJ"+dy_soup_, "DYJetsToLL-J"+dy_soup_
    };

    // Samples to subtract in sideband for W+jets estimate
    samples_alias_map_["w_sub_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW", "DYJetsToTauTau"+dy_soup_,
     "DYJetsToTauTau-L"+dy_soup_,
     "DYJetsToLL-L"+dy_soup_};

    samples_alias_map_["qcd_sub_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW", "WJetsToLNuSoup",
     "DYJetsToTauTau"+dy_soup_,
     "DYJetsToTauTau-L"+dy_soup_,
     "DYJetsToLL-L"+dy_soup_};

    for (auto const& top_sample : samples_alias_map_["top_samples"]) {
      samples_alias_map_["w_sub_samples"].push_back(top_sample);
      samples_alias_map_["qcd_sub_samples"].push_back(top_sample);
    }
    for (auto const& zj_sample : samples_alias_map_["zj_samples"]) {
      samples_alias_map_["w_sub_samples"].push_back(zj_sample);
      samples_alias_map_["qcd_sub_samples"].push_back(zj_sample);
    }
  }

  void HhhAnalysis::SetQCDRatio(double const& ratio){
    qcd_os_ss_factor_ = ratio;
  }

  void HhhAnalysis::AddSample(std::string const& sample) {
    sample_names_.push_back(sample);
  }

  void HhhAnalysis::ParseParamFile(std::string const& file) {
    SimpleParamParser parser;
    parser.ParseFile(file);
    std::cout << "[HhhAnalysis::ParseParamFile] Extracting sample info from file " << file << std::endl;
    lumi_ = parser.GetParam<double>("LUMI_DATA_"+Channel2String(ch_));
    std::cout << "[HhhAnalysis::ParseParamFile] Integrated luminosity set to " << lumi_ << " /pb" << std::endl;
    if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % "Sample" % "Events" % "Cross Section" % "Sample Lumi" % "Rel. Lumi";
    if (verbosity_ > 1) std::cout << "-----------------------------------------------------------------------------------------\n";
    for (auto sample : sample_names_) {
      std::string lookup = sample;
      if (sample.find("Special") != sample.npos) {
        size_t found = sample.find('_');
        found = sample.find('_',found+1);
        lookup = sample.substr(found+1);
      }
      if (parser.ParamExists("XS_"+sample) && parser.ParamExists("EVT_"+sample)) {
        double evt = parser.GetParam<double>("EVT_"+sample);
        double xs = parser.GetParam<double>("XS_"+sample);
        if (xs <= 0) continue;
        sample_info_[sample] = std::make_pair(evt, xs);
        if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % sample % unsigned(evt+0.5) % xs % (evt/xs) % (evt/(xs*lumi_));
      }
    }
  }

  void HhhAnalysis::AddSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
      sample_names_.push_back("GluGluToHToTauTau_M-"+m);
      sample_names_.push_back("VBF_HToTauTau_M-"+m);
      sample_names_.push_back("WH_ZH_TTH_HToTauTau_M-"+m);
      //sample_names_.push_back("WH_HToTauTau_M-"+m);
      // sample_names_.push_back("TTH_HToTauTau_M-"+m);
      //sample_names_.push_back("ZH_HToTauTau_M-"+m);
    }
  }
  void HhhAnalysis::AddHWWSignalSamples(std::vector<std::string> masses) {
    if (year_ == "2012") {
      for (auto m : masses) {
        sample_names_.push_back("GluGluToHToWWTo2LAndTau2Nu_M-"+m);
        sample_names_.push_back("VBF_HToWWTo2LAndTau2Nu_M-"+m);
      }      
    } else {
      for (auto m : masses) {
        if (m == "110" || m == "115" || m == "125" || m == "135") {
          sample_names_.push_back("GluGluToHToWWTo2LAndTau2Nu_M-"+m);
          sample_names_.push_back("VBF_HToWWTo2LAndTau2Nu_M-"+m);          
        } else if (m == "145" || m == "155") { // no samples, will do horizontal morphing
          continue;
        } else {
          sample_names_.push_back("GluGluToHToWWTo2L2Nu_M-"+m);
          sample_names_.push_back("GluGluToHToWWTo2Tau2Nu_M-"+m);          
          sample_names_.push_back("GluGluToHToWWToLNuTauNu_M-"+m);
          sample_names_.push_back("VBF_HToWWTo2L2Nu_M-"+m);          
          sample_names_.push_back("VBF_HToWWTo2Tau2Nu_M-"+m);
          sample_names_.push_back("VBF_HToWWToLNuTauNu_M-"+m);          
        }
      }      
    }
  }

  void HhhAnalysis::AddMSSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
      sample_names_.push_back("GluGluToHTohhTo2Tau2B_mH-"+m);
      sample_names_.push_back("GluGluToAToZhToLLBB_mA-"+m);
    }
  }

  void HhhAnalysis::ReadTrees(std::string const& folder, std::string const& fallback_folder) {
    std::cout << "[HhhAnalysis::ReadTrees] Reading input files..." << std::endl;
    std::vector<std::string> result_summary;
    for (auto name : sample_names_) {
      // The input file is folder + sample name + channel + year
      std::string input_filename = folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
      std::string label = name;
      TFile *tmp_file = nullptr;
      if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      if (!tmp_file && fallback_folder != "") {
        if (verbosity_ > 1) std::cout << "[HhhAnalysis::ReadTrees] " << input_filename << " not found, trying fallback folder" << std::endl;
        input_filename = fallback_folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
        if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      }
      if (!tmp_file) {
        std::cout << "[HhhAnalysis::ReadTrees] Warning: " << input_filename << " cannot be opened" << std::endl;
        continue;
      }
      if (verbosity_ > 1) result_summary.push_back((boost::format("%-70s %s %-30s\n") % input_filename % "-->" % label).str());
      gDirectory->cd("/");
      TTree *tmp_tree = dynamic_cast<TTree*>(gDirectory->Get("ntuple"));
      if (!tmp_tree) {
        std::cerr << "[HhhAnalysis::ReadTrees] Warning: Unable to extract TTree from file " << input_filename << std::endl;
        continue;        
      }
      tmp_tree->SetEstimate(1000);
      tfiles_[label] = tmp_file;
      ttrees_[label] = tmp_tree;
    }
    for (auto str : result_summary) std::cout << str;
  }

  double HhhAnalysis::GetLumiScale(std::string const& sample) {
    auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;
      double xs = it->second.second;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }

  double HhhAnalysis::GetLumiScaleFixedXS(std::string const& sample, double xs) {
      auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }


  HhhAnalysis::HistValuePair HhhAnalysis::GenerateData(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    auto data_norm = this->GetRate("Data", sel, cat, wt);
    TH1F data_hist = this->GetShape(var, "Data", sel, cat, wt);
    SetNorm(&data_hist, data_norm.first);
    return std::make_pair(data_hist, data_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateZTT(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateZTT] --------------------------------------------------------\n";
    auto ztt_norm = this->GetRateViaRefEfficiency(this->ResolveAlias("ZTT_Eff_Sample"), "DYJetsToTauTau"+dy_soup_, "os", this->ResolveAlias("inclusive"), sel, cat, wt);
    if (this->AliasDefined("ztt_shape_cat")) cat = this->ResolveAlias("ztt_shape_cat");
    TH1F ztt_hist = this->GetShape(var, this->ResolveAlias("ZTT_Shape_Sample"), sel, cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % this->ResolveAlias("ZTT_Shape_Sample") % sel % cat % wt;
    SetNorm(&ztt_hist, ztt_norm.first);
    if (ch_ != channel::em) {
      auto ztt_leptonic_norm = this->GetLumiScaledRate("DYJetsToTauTau-L"+dy_soup_, sel, cat, wt);
      TH1F ztt_leptonic_hist = this->GetLumiScaledShape(var, "DYJetsToTauTau-L"+dy_soup_, sel, cat, wt);
      SetNorm(&ztt_leptonic_hist, ztt_leptonic_norm.first);
      if (verbosity_) PrintValue("ZTT-Leptonic", ztt_leptonic_norm); 
      ztt_norm = ValueAdd(ztt_norm, ztt_leptonic_norm);
      ztt_hist.Add(&ztt_leptonic_hist);
      /*
      auto ztt_hadronic_norm = this->GetLumiScaledRate("DYJetsToTauTau-JJ"+dy_soup_, sel, cat, wt);
      TH1F ztt_hadronic_hist = this->GetLumiScaledShape(var, "DYJetsToTauTau-JJ"+dy_soup_, sel, cat, wt);
      SetNorm(&ztt_hadronic_hist, ztt_hadronic_norm.first);
      if (verbosity_) PrintValue("ZTT-Hadronic", ztt_hadronic_norm);
      ztt_norm = ValueAdd(ztt_norm, ztt_hadronic_norm);
      ztt_hist.Add(&ztt_hadronic_hist);
      */
    }
    return std::make_pair(ztt_hist, ztt_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateZL(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateZL] ---------------------------------------------------------\n";
    Value zl_norm;
    TH1F zl_hist;
    if (method == 5) {
      if (this->ResolveAlias("ZL_Eff_Sample") != "DYJetsToLL-L"+dy_soup_) {
        zl_norm = this->GetRateViaRefEfficiency(this->ResolveAlias("ZL_Eff_Sample"), "DYJetsToLL-L"+dy_soup_, sel, this->ResolveAlias("zl_denominator"), sel, cat, wt);
      } else {
        zl_norm = this->GetLumiScaledRate("DYJetsToLL-L"+dy_soup_, sel, cat, wt);
      }
      zl_hist = this->GetLumiScaledShape(var, "DYJetsToLL-L"+dy_soup_, sel, this->ResolveAlias("vbf_loose_jets20"), wt);
      if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
        % ("DYJetsToLL-L"+dy_soup_) % sel % this->ResolveAlias("vbf_loose_jets20") % wt;
    } else {
      std::string zll_shape_cat = cat;
      if (method == 6)  zll_shape_cat = this->ResolveAlias("btag_low_loose");
      if (method == 7)  zll_shape_cat = this->ResolveAlias("btag_high_loose");
      if (method == 12) zll_shape_cat = this->ResolveAlias("btag_loose");
      zl_norm = this->GetLumiScaledRate("DYJetsToLL-L"+dy_soup_, sel, cat, wt);
      zl_hist = this->GetLumiScaledShape(var, "DYJetsToLL-L"+dy_soup_, sel, zll_shape_cat, wt);
      if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
        % ("DYJetsToLL-L"+dy_soup_) % sel % zll_shape_cat % wt;
    }
    SetNorm(&zl_hist, zl_norm.first);
    return std::make_pair(zl_hist, zl_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateZJ(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateZJ] ---------------------------------------------------------\n";
    std::vector<std::string> zj_samples = this->ResolveSamplesAlias("zj_samples");
    if (verbosity_) {
      std::cout << "zj_samples: ";
      for (unsigned i = 0; i < zj_samples.size(); ++i) {
        std::cout << zj_samples[i];
        if (i != zj_samples.size()-1) std::cout << ", ";
      }
      std::cout << std::endl;
    }

    Value zj_norm;
    TH1F zj_hist;
    if (method == 5) {
      zj_norm = this->GetLumiScaledRate(zj_samples, sel, cat, wt);
      zj_hist = this->GetLumiScaledShape(var,zj_samples, sel, this->ResolveAlias("vbf_loose"), wt);
      if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
        % "zj_samples" % sel % this->ResolveAlias("vbf_loose") % wt;
    } else {
      std::string zll_shape_cat = cat;
      if (method == 6)  zll_shape_cat = this->ResolveAlias("btag_low_loose");
      if (method == 7)  zll_shape_cat = this->ResolveAlias("btag_high_loose");
      if (method == 12) zll_shape_cat = this->ResolveAlias("btag_loose");
      zj_norm = this->GetLumiScaledRate(zj_samples, sel, cat, wt);
      zj_hist = this->GetLumiScaledShape(var, zj_samples, sel, zll_shape_cat, wt);
      if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
        % "zj_samples" % sel % zll_shape_cat % wt;
    }
    SetNorm(&zj_hist, zj_norm.first);
    return std::make_pair(zj_hist, zj_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateTOP(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateTOP] --------------------------------------------------------\n";
    std::vector<std::string> top_samples = this->ResolveSamplesAlias("top_samples");
    if (verbosity_) {
      std::cout << "top_samples: ";
      for (unsigned i = 0; i < top_samples.size(); ++i) {
        std::cout << top_samples[i];
        if (i != top_samples.size()-1) std::cout << ", ";
      }
      std::cout << std::endl;
    }
    auto top_norm = this->GetLumiScaledRate(top_samples, sel, cat, wt);
    // auto top_norm = this->GetLumiScaledRate("TTJets", sel, cat, wt);
    // std::string top_shape_sample = (year_ == "2011") ? "TTJets" : "TT";
    // std::string top_shape_sample = "TTJets";
    std::string top_shape_cat = cat;
    if (method == 5) {
      if (ch_ == channel::em) {
        top_shape_cat = this->ResolveAlias("vbf_no_cjv");
      } else {
        top_shape_cat = this->ResolveAlias("vbf_loose");
      }
    }
    TH1F top_hist = this->GetLumiScaledShape(var, top_samples, sel, top_shape_cat, wt);
    // TH1F top_hist = this->GetLumiScaledShape(var, top_shape_sample, sel, top_shape_cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "top_samples" % sel % top_shape_cat % wt;
    SetNorm(&top_hist, top_norm.first);
    return std::make_pair(top_hist, top_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateVV(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateVV] ---------------------------------------------------------\n";
    std::vector<std::string> vv_samples = this->ResolveSamplesAlias("vv_samples");
    auto vv_norm = this->GetLumiScaledRate(vv_samples, sel, cat, wt);
    // if (ch_ == channel::em && (method == 0 || method == 1)) {
    //   vv_norm = ValueProduct(vv_norm, std::make_pair(1.23, 0.0));
    //   PrintValue("Scaling Norm", std::make_pair(1.23, 0.0));
    // }
    std::string vv_shape_cat = cat;
    if (method == 5) {
      vv_shape_cat = this->ResolveAlias("vbf_loose");
    }
    TH1F vv_hist = this->GetLumiScaledShape(var, vv_samples, sel, vv_shape_cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "vv_samples" % sel % vv_shape_cat % wt;
    SetNorm(&vv_hist, vv_norm.first);
    return std::make_pair(vv_hist, vv_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateW(unsigned method, std::string var, std::string /*sel*/, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateW] ----------------------------------------------------------\n";
    std::vector<std::string> w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
    std::string w_extrap_cat = cat;
    std::string w_extrp_sdb_sel = this->ResolveAlias("w_os")+" && "+this->ResolveAlias("w_sdb");
    std::string w_extrp_sig_sel = this->ResolveAlias("w_os")+" && "+this->ResolveAlias("sel");
    std::string w_sdb_sel = this->ResolveAlias("w_sdb_os")+" && "+this->ResolveAlias("w_sdb");
    if (method == 5) {
      w_extrap_cat    = this->ResolveAlias("w_vbf_extrap_cat");
      w_extrp_sdb_sel = this->ResolveAlias("w_vbf_os")+" && "+this->ResolveAlias("w_vbf_sdb");
      w_extrp_sig_sel = this->ResolveAlias("w_vbf_os")+" && "+this->ResolveAlias("sel");
      w_sdb_sel       = this->ResolveAlias("w_sdb_os")+" && "+this->ResolveAlias("w_vbf_sdb");
    }
    if (method == 6)  w_extrap_cat = this->ResolveAlias("btag_low_loose");
    if (method == 7)  w_extrap_cat = this->ResolveAlias("btag_high_loose");
    if (method == 12) w_extrap_cat = this->ResolveAlias("btag_loose");
    
    auto w_norm = this->GetRateViaWMethod("WJetsToLNuSoup", w_extrap_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
        "Data", cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());
    std::string w_shape_cat = cat;
    std::string w_shape_sel = this->ResolveAlias("w_shape_os") + " && " + this->ResolveAlias("sel");
    if (method == 5)  w_shape_cat = cat;
    // if (method == 5)  w_shape_cat = this->ResolveAlias("vbf_loose");
    if (method == 6)  w_shape_cat = this->ResolveAlias("btag_low_loose");
    if (method == 7)  w_shape_cat = this->ResolveAlias("btag_high_loose");
    if (method == 12) w_shape_cat = this->ResolveAlias("btag_loose");
    TH1F w_hist = this->GetShape(var, this->ResolveAlias("W_Shape_Sample"), w_shape_sel, w_shape_cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % this->ResolveAlias("W_Shape_Sample") % w_shape_sel % w_shape_cat % wt;
    SetNorm(&w_hist, w_norm.first);
    return std::make_pair(w_hist, w_norm);
  }
  
  HhhAnalysis::HistValuePair HhhAnalysis::GenerateQCD(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HhhAnalysis::GenerateQCD] --------------------------------------------------------\n";
    Value qcd_norm;
    TH1F qcd_hist;
    if (ch_ != channel::em) {
      std::vector<std::string> qcd_sub_samples = this->ResolveSamplesAlias("qcd_sub_samples");
      std::vector<std::string> w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
      std::string qcd_sdb_sel = "!os && " + this->ResolveAlias("sel");
      std::string w_extrp_sdb_sel = this->ResolveAlias("w_ss")+" && "+this->ResolveAlias("w_sdb");
      std::string w_extrp_sig_sel = this->ResolveAlias("w_ss")+" && "+this->ResolveAlias("sel");
      std::string w_sdb_sel = "!os && "+this->ResolveAlias("w_sdb");
      std::string qcd_cat = cat;
      if (method == 5 || method == 4) qcd_cat = this->ResolveAlias("inclusive");
      Value w_ss_norm = this->GetRateViaWMethod("WJetsToLNuSoup", qcd_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
            "Data", qcd_cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());
      qcd_norm = this->GetRateViaQCDMethod(std::make_pair(qcd_os_ss_factor_,0.), "Data", qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt, {
        {"WJetsToLNuSoup", [&]()->HhhAnalysis::Value {
          return w_ss_norm;}
        }
      });
      if (method == 5 || method == 4) {
        Value qcd_eff = this->SampleEfficiency(this->ResolveAlias("QCD_Eff_Sample"), qcd_sdb_sel, qcd_cat, qcd_sdb_sel, cat, wt);
        if (verbosity_) {
          std::cout << "CategoryEff:   " << boost::format("%s,'%s','%s'/'%s','%s'\n") % this->ResolveAlias("QCD_Eff_Sample")  % qcd_sdb_sel 
            % cat % qcd_cat % wt;
          PrintValue("CategoryEff", qcd_eff);
        }
        qcd_norm = ValueProduct(qcd_norm, qcd_eff);
      }
      if (qcd_norm.first <= 0.0) {
        double default_rate = 0.0000001;
        std::cout << "[HhhAnalysis::GenerateQCD] Warning, QCD rate is negative (" 
          << qcd_norm.first << "), setting to " << default_rate << " and maintaining error" << std::endl;
        qcd_norm.first = default_rate;
      }
      if (method == 0 || method == 8 || method == 11) {
        qcd_hist = this->GetShapeViaQCDMethod(var, "Data", qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt, {
          {"WJetsToLNuSoup", [&]()->HhhAnalysis::Value {
            return w_ss_norm;} 
          }
        });
      } else {
        if (method == 4)  qcd_cat = cat;
        if (method == 5)  qcd_cat = this->ResolveAlias("vbf_loose_jets20");
        if (method == 6)  qcd_cat = this->ResolveAlias("btag_low_loose");
        if (method == 7)  qcd_cat = this->ResolveAlias("btag_high_loose");
        if (method == 12) qcd_cat = this->ResolveAlias("btag_loose");        
        qcd_hist = this->GetShape(var, this->ResolveAlias("QCD_Shape_Sample"), qcd_sdb_sel, qcd_cat, wt);
        if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
          % this->ResolveAlias("QCD_Shape_Sample") % qcd_sdb_sel % qcd_cat % wt;

      }
    } else {
      Value qcd_dilepton = this->GetRateViaFakesMethod(this->ResolveAlias("em_qcd_sel"), "", wt);
      qcd_dilepton = ValueProduct(qcd_dilepton, std::make_pair(0.83,0.));
      std::string ss_sel = "!os && "+this->ResolveAlias("sel");
      Value qcd_eff = this->SampleEfficiency("Data", this->ResolveAlias("ss"), "", ss_sel, cat, wt);
      if (verbosity_) PrintValue("FR Inclusive", qcd_dilepton);
      if (verbosity_) PrintValue("SS Eff", qcd_eff);
      if (method == 0 || method == 2) {
        qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
        qcd_hist = this->GetShape(var, "Special_23_Data", ss_sel, cat, wt);
        if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
          % "Special_23_Data" % ss_sel % cat % wt;
      } else if (method == 1) {
        qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
        qcd_hist = this->GetShape(var, "Special_24_Data", ss_sel, cat, wt);
        if (verbosity_) std::cout << "SS Shape(0.2): " << boost::format("%s,'%s','%s','%s'\n")
          % "Special_24_Data" % ss_sel % cat % wt;
        TH1F fr_hist =  this->GetShapeViaFakesMethod(var, sel, cat, wt);
        if (verbosity_) std::cout << "FR Shape(0.8): " << boost::format("%s,'%s','%s','%s'\n")
          % "               " % sel % cat % wt;
        qcd_hist.Scale(0.2 / Integral(&qcd_hist));
        fr_hist.Scale(0.8 / Integral(&fr_hist));
        qcd_hist.Add(&fr_hist,1.0);
      } else if (method == 3) {
        qcd_norm = this->GetRateViaFakesMethod(sel, cat, wt);
        qcd_hist = this->GetShapeViaFakesMethod(var, sel, cat, wt); 
        if (verbosity_) std::cout << "FR Shape: " << boost::format("%s,'%s','%s','%s'\n")
          % "               " % sel % cat % wt;
      } else if (method == 5) {
        qcd_norm = this->GetRateViaFakesMethod(sel, cat, wt);
        qcd_hist = this->GetShapeViaFakesMethod(var, sel, this->ResolveAlias("vbf_no_cjv"), wt);
        if (verbosity_) std::cout << "FR Shape: " << boost::format("%s,'%s','%s','%s'\n")
          % "               " % sel % this->ResolveAlias("vbf_no_cjv") % wt;
      } else {
       qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
       qcd_hist = this->GetShape(var, "Data", ss_sel, cat, wt);
       if (verbosity_) std::cout << "SS Shape: " << boost::format("%s,'%s','%s','%s'\n")
         % "Data" % ss_sel % cat % wt;
      }
      if (method == 12) {
        qcd_norm = ValueProduct(qcd_norm, std::make_pair(0.67,0.));
      }
    }
    SetNorm(&qcd_hist, qcd_norm.first);
    return std::make_pair(qcd_hist, qcd_norm);
  }

  HhhAnalysis::HistValuePair HhhAnalysis::GenerateSignal(std::string sample, std::string var, std::string sel, std::string cat, std::string wt, double xs) {
    Value signal_norm;
    if (xs > 0) {
      if (verbosity_ > 1) std::cout << "[HhhAnalysis::GenerateSignal] " << sample << " scaled to lumi using cross section " << xs << " pb" << std::endl;
      signal_norm = GetRate(sample, sel, cat, wt);
			if((std::get<0>(signal_norm)==0)&&(std::get<1>(signal_norm)==0)){
  			std::cout<<"[HhhAnalysis::GenerateSignal] Warning: Rate for sample "<<sample<<" is 0. Setting signal to 0"<<std::endl;
   			TH1::AddDirectory(false);
	  		TH1F* htemp = new TH1F("hempty","hempty",1,0,1);
	  		return std::make_pair(*htemp,signal_norm);
			}
      signal_norm = ValueProduct(signal_norm, std::make_pair(this->GetLumiScaleFixedXS(sample, xs), 0.0));
    } else {
      signal_norm = GetLumiScaledRate(sample, sel, cat, wt);
			if((std::get<0>(signal_norm)==0)&&std::get<1>(signal_norm)==0){
	  		std::cout<<"[HhhAnalysis::GenerateSignal] Warning: Rate for sample "<<sample<<" is 0. Setting signal to 0"<<std::endl;
	  		TH1::AddDirectory(false);
	  		TH1F* htemp = new TH1F("hempty","hempty",1,0,1);
	  		return std::make_pair(*htemp,signal_norm);
	  	}
    }
    TH1F signal_shape = this->GetShape(var, sample, sel, cat, wt);
    SetNorm(&signal_shape, signal_norm.first);
    return std::make_pair(signal_shape, signal_norm);
  }

  void HhhAnalysis::FillSMSignal(HistValueMap & hmap, 
                    std::vector<std::string> const& masses,
                    std::string const& var,
                    std::string const& sel,
                    std::string const& cat,
                    std::string const& wt,
                    std::string const& infix,
                    std::string const& postfix,
                    double fixed_xs) {
    for (auto const& m : masses) {
      hmap["ggH"+infix+m+postfix] = this->GenerateSignal("GluGluToHToTauTau_M-"+m,    var, sel, cat, wt, fixed_xs);
      hmap["qqH"+infix+m+postfix] = this->GenerateSignal("VBF_HToTauTau_M-"+m,        var, sel, cat, wt, fixed_xs);
      hmap["VH"+infix+m+postfix]  = this->GenerateSignal("WH_ZH_TTH_HToTauTau_M-"+m,  var, sel, cat, wt, fixed_xs);
      //hmap["WH"+infix+m+postfix]  = this->GenerateSignal("WH_HToTauTau_M-"+m,  var, sel, cat, wt, fixed_xs);
      // hmap["ttH"+infix+m+postfix] = this->GenerateSignal("TTH_HToTauTau_M-"+m,  var, sel, cat, wt, fixed_xs);
      //hmap["ZH"+infix+m+postfix]  = this->GenerateSignal("ZH_HToTauTau_M-"+m,  var, sel, cat, wt, fixed_xs);
    }
  }
  
  void HhhAnalysis::FillHWWSignal(HistValueMap & hmap, 
                    std::vector<std::string> const& masses,
                    std::string const& var,
                    std::string const& sel,
                    std::string const& cat,
                    std::string const& wt,
                    std::string const& infix,
                    std::string const& postfix,
                    double fixed_xs) {
    if (year_ == "2012") {
      for (auto const& m : masses) {
        hmap["ggH"+infix+m+postfix] = this->GenerateSignal("GluGluToHToWWTo2LAndTau2Nu_M-"+m,    var, sel, cat, wt, fixed_xs);
        hmap["qqH"+infix+m+postfix] = this->GenerateSignal("VBF_HToWWTo2LAndTau2Nu_M-"+m,        var, sel, cat, wt, fixed_xs);
      }      
    } else {
      for (auto const& m : masses) {
        if (m == "110" || m == "115" || m == "125" || m == "135") {
          hmap["ggH"+infix+m+postfix] = this->GenerateSignal("GluGluToHToWWTo2LAndTau2Nu_M-"+m,    var, sel, cat, wt, fixed_xs);
          hmap["qqH"+infix+m+postfix] = this->GenerateSignal("VBF_HToWWTo2LAndTau2Nu_M-"+m,        var, sel, cat, wt, fixed_xs);          
        } else if (m == "145" || m == "155") { // no samples, will do horizontal morphing
          continue;
    } else {
          HistValuePair ggh_tmp_1 = this->GenerateSignal("GluGluToHToWWTo2L2Nu_M-"+m,    var, sel, cat, wt, fixed_xs * (4./9.));
          HistValuePair ggh_tmp_2 = this->GenerateSignal("GluGluToHToWWTo2Tau2Nu_M-"+m,    var, sel, cat, wt, fixed_xs * (1./9.));
          HistValuePair ggh_tmp_3 = this->GenerateSignal("GluGluToHToWWToLNuTauNu_M-"+m,    var, sel, cat, wt, fixed_xs * (4./9.));
          ggh_tmp_1.first.Add(&(ggh_tmp_2.first));
          ggh_tmp_1.first.Add(&(ggh_tmp_3.first));
          ggh_tmp_1.second = ValueAdd(ggh_tmp_1.second, ggh_tmp_2.second);
          ggh_tmp_1.second = ValueAdd(ggh_tmp_1.second, ggh_tmp_3.second);
          hmap["ggH"+infix+m+postfix] = ggh_tmp_1;
          HistValuePair qqh_tmp_1 = this->GenerateSignal("VBF_HToWWTo2L2Nu_M-"+m,    var, sel, cat, wt, fixed_xs * (4./9.));
          HistValuePair qqh_tmp_2 = this->GenerateSignal("VBF_HToWWTo2Tau2Nu_M-"+m,    var, sel, cat, wt, fixed_xs * (1./9.));
          HistValuePair qqh_tmp_3 = this->GenerateSignal("VBF_HToWWToLNuTauNu_M-"+m,    var, sel, cat, wt, fixed_xs * (4./9.));
          qqh_tmp_1.first.Add(&(qqh_tmp_2.first));
          qqh_tmp_1.first.Add(&(qqh_tmp_3.first));
          qqh_tmp_1.second = ValueAdd(qqh_tmp_1.second, qqh_tmp_2.second);
          qqh_tmp_1.second = ValueAdd(qqh_tmp_1.second, qqh_tmp_3.second);
          hmap["qqH"+infix+m+postfix] = qqh_tmp_1;     
        }
      }
      for (auto const& m : masses) {
        if (m == "145" || m == "155") { // no samples, will do horizontal morphing
          double x1, x2, x;
          std::string s1, s2;
          if (m == "145") {
            x = 145; x1 = 140; x2 = 150; s1 = "140"; s2 = "150";
          }
          if (m == "155") {
            x = 155; x1 = 150; x2 = 160; s1 = "150"; s2 = "160";
          }
          std::vector<std::string> procs = {"ggH", "qqH"};
          for (auto p : procs) {
            double yield = 1.0;
            double y1 = hmap[p+infix+s1+postfix].second.first;
            double y2 = hmap[p+infix+s2+postfix].second.first;
            if (x2 == x1) {
              yield =  0.5*(y1+y2);
            } else {
              yield = y1 + ((y2 - y1)/(x2 - x1))*(x-x1);
            }
            hmap[p+infix+m+postfix].first = *(th1fmorph("morphed","morphed", &(hmap[p+infix+s1+postfix].first), &(hmap[p+infix+s2+postfix].first), x1, x2, x, yield, 0));
            hmap[p+infix+m+postfix].second = std::make_pair(yield,0.0);
          }
        }
      }
    }
  }
  void HhhAnalysis::InterpolateSMSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& var_final,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double interpolate,
                        double fixed_xs){
    std::vector<std::string> procs = {"ggH", "qqH", "VH"};
    std::vector<std::string> names = {"GluGluToHToTauTau_M-", "VBF_HToTauTau_M-", "WH_ZH_TTH_HToTauTau_M-", 
      "WH_HToTauTau_M-", /*"TTH_HToTauTau_M-",*/ "ZH_HToTauTau_M-"};
    for (unsigned i = 0; i < masses.size()-1; ++i) {
      double m_low = boost::lexical_cast<double>(masses[i]);
      double m_high = boost::lexical_cast<double>(masses[i+1]);
      std::vector<double> new_points;
      double point = m_low;
      while (m_low < m_high && (point+interpolate) < m_high*0.99999) {
        point+=interpolate;
        new_points.push_back(point);
      }
      std::cout << "[HhhAnalysis::InterpolateSMSignal] Running horizontal morphing for: ";
      for (auto p : new_points) std::cout << p << " ";
      std::cout << std::endl;
      for (unsigned j = 0; j < procs.size(); ++j) {
        TH1F h_low = this->GenerateSignal(names[j]+masses[i], var, sel, cat, wt, fixed_xs).first;
        TH1F h_high = this->GenerateSignal(names[j]+masses[i+1], var, sel, cat, wt, fixed_xs).first;
        for (unsigned k = 0; k < new_points.size(); ++k) {
          double yield = 1.0;
          double y1 = h_low.Integral();
          double y2 = h_high.Integral();
          if (m_high == m_low) {
            yield =  0.5*(y1+y2);
          } else {
            yield = y1 + ((y2 - y1)/(m_high - m_low))*(new_points[k]-m_low);
          }
          TH1F result = *(th1fmorph("morphed","morphed", &h_low, &h_high, m_low, m_high, new_points[k], yield, 0));
          TH1F h_ref = this->GenerateSignal(names[j]+masses[i], var_final, sel, cat, wt, fixed_xs).first;
          result = * ( (TH1F*)result.Rebin(h_ref.GetNbinsX(),"",h_ref.GetXaxis()->GetXbins()->GetArray())  );
          std::string m_str = boost::lexical_cast<std::string>(int(new_points[k]+0.5));
          hmap[procs[j]+infix+m_str+postfix].first = result;
          hmap[procs[j]+infix+m_str+postfix].second = std::make_pair(yield,0.0);
        }
      }
    }
  }

  void HhhAnalysis::FillMSSMSignal(HistValueMap & hmap, 
                    std::vector<std::string> const& masses,
                    std::string const& var,
                    std::string const& sel,
                    std::string const& cat,
                    std::string const& wt,
                    std::string const& infix,
                    std::string const& postfix,
                    double fixed_xs) {
    for (auto const& m : masses) {
      auto signal_pair = this->GenerateSignal("GluGluToHTohhTo2Tau2B_mH-"+m, var, sel, cat, wt, fixed_xs);
      auto signal_pair_AZh = this->GenerateSignal("GluGluToAToZhToLLBB_mA-"+m, var, sel, cat, wt, fixed_xs);
			if(std::get<0>(std::get<1>(signal_pair))!=0){
      hmap["ggHTohh"+infix+m+postfix] = signal_pair;
			}
			if(std::get<0>(std::get<1>(signal_pair_AZh))!=0){
      hmap["ggAToZh"+infix+m+postfix] = signal_pair_AZh;
      //PrintValue("ggHTohh"+postfix, signal_pair.second);
      //hmap["bbH"+infix+m+postfix] = this->GenerateSignal("SUSYBBHToTauTau_M-"+m,       var, sel, cat, wt, fixed_xs);
		}
    }
  }

  void HhhAnalysis::FillHistoMap(HistValueMap & hmap, unsigned method,
                        std::string var,
                        std::string sel,
                        std::string cat,
                        std::string wt,
                        std::string postfix) {
    Value total_bkr;
    // Data
    auto data_pair = this->GenerateData(method, var, sel, cat, wt);
    PrintValue("data_obs"+postfix, data_pair.second);
    hmap["data_obs"+postfix] = data_pair;
    // Top
    auto top_pair = this->GenerateTOP(method, var, sel, cat, wt);
    std::string top_map_label = (ch_ == channel::em) ? "ttbar" : "TT";
    PrintValue(top_map_label+postfix, top_pair.second);
    total_bkr = ValueAdd(total_bkr, top_pair.second);
    hmap[top_map_label+postfix] = top_pair;
    // Diboson
    auto vv_pair = this->GenerateVV(method, var, sel, cat, wt);
    std::string vv_map_label = (ch_ == channel::em) ? "EWK" : "VV";
    PrintValue(vv_map_label+postfix, vv_pair.second);
    total_bkr = ValueAdd(total_bkr, vv_pair.second);
    hmap[vv_map_label+postfix] = vv_pair;
    // Z->ll
    if (ch_ != channel::em) {
      auto zl_pair = this->GenerateZL(method, var, sel, cat, wt);
      auto zj_pair = this->GenerateZJ(method, var, sel, cat, wt);
      Value zll_norm = ValueAdd(zl_pair.second, zj_pair.second);
      TH1F zll_hist = zl_pair.first;
      zll_hist.Add(&zj_pair.first);
      PrintValue("ZLL"+postfix, zll_norm);
      PrintValue("ZL"+postfix, zl_pair.second);
      PrintValue("ZJ"+postfix, zj_pair.second);
      total_bkr = ValueAdd(total_bkr, zll_norm);
      hmap["ZLL"+postfix] = std::make_pair(zll_hist, zll_norm);
      hmap["ZL"+postfix]  = zl_pair;
      hmap["ZJ"+postfix]  = zj_pair;
    }
    // Z->tautau
    auto ztt_pair = this->GenerateZTT(method, var, sel, cat, wt);
    std::string ztt_map_label = (ch_ == channel::em) ? "Ztt" : "ZTT";
    PrintValue(ztt_map_label+postfix, ztt_pair.second);
    total_bkr = ValueAdd(total_bkr, ztt_pair.second);
    hmap[ztt_map_label+postfix] = ztt_pair;
    // W+jets
    if (ch_ != channel::em) {
      auto w_pair = this->GenerateW(method, var, sel, cat, wt);
      PrintValue("W"+postfix, w_pair.second);
      total_bkr = ValueAdd(total_bkr, w_pair.second);
      hmap["W"+postfix] = w_pair;
    }
    // QCD/Fakes
    auto qcd_pair = this->GenerateQCD(method, var, sel, cat, wt);
    std::string qcd_map_label = (ch_ == channel::em) ? "Fakes" : "QCD";
    PrintValue(qcd_map_label+postfix, qcd_pair.second);
    total_bkr = ValueAdd(total_bkr, qcd_pair.second);
    hmap[qcd_map_label+postfix] = qcd_pair;
    // Print the total background yield
    PrintValue("Total"+postfix, total_bkr);
    return;
  }

  std::string HhhAnalysis::BuildCutString(std::string const& selection,
      std::string const& category,
      std::string const& weight) {
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";

    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    return full_selection;                                      
  }

  std::string HhhAnalysis::BuildVarString(std::string const& variable) {
    std::string full_variable = variable;
    if (full_variable.find_last_of("(") != full_variable.npos 
        && full_variable.find("[") == full_variable.npos
        && full_variable.find("]") == full_variable.npos) {
      full_variable.insert(full_variable.find_last_of("("),">>htemp");
    }
    return full_variable;
  }


  TH1F HhhAnalysis::GetShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1::SetDefaultSumw2(true);
    std::string full_variable = BuildVarString(variable);
    std::size_t begin_var = full_variable.find("[");
    std::size_t end_var   = full_variable.find("]");
    TH1F *htemp = nullptr;
    if (begin_var != full_variable.npos && end_var != full_variable.npos) {
      std::string binning = full_variable.substr(begin_var+1, end_var-begin_var-1);
      std::vector<std::string> string_vec;
      boost::split(string_vec, binning, boost::is_any_of(","));
      std::vector<double> bin_vec;
      for (auto str : string_vec) bin_vec.push_back(boost::lexical_cast<double>(str));
      TH1::AddDirectory(true);
      htemp = new TH1F("htemp","htemp", bin_vec.size()-1, &(bin_vec[0]));
      TH1::AddDirectory(false);
      full_variable.erase(begin_var, full_variable.npos);
      full_variable += ">>htemp";
    }
    std::string full_selection = BuildCutString(selection, category, weight);
    // std::cout << full_selection << std::endl;
    // std::cout << full_variable << std::endl;
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
    gDirectory->Delete("htemp;*");
    auto rate = GetRate(sample, selection, category, weight);
    SetNorm(&result, rate.first);
    return result;
  }

  TH1F HhhAnalysis::GetLumiScaledShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1F result = GetShape(variable, sample, selection, category, weight);
    result.Scale(GetLumiScale(sample));
    return result;
  }

  TH1F HhhAnalysis::GetLumiScaledShape(std::string const& variable,
                                       std::vector<std::string> const& samples, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {

    TH1F result = GetLumiScaledShape(variable, samples.at(0), selection, category, weight);
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        TH1F tmp = GetLumiScaledShape(variable, samples.at(i), selection, category, weight);
        result.Add(&tmp);
      }
    }
    return result;
  }


  std::pair<double, double> HhhAnalysis::GetRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    if (verbosity_ > 1) std::cout << "--GetRate-- Sample:\"" << sample << "\" Selection:\"" << selection << "\" Category:\"" 
      << category << "\" Weight:\"" << weight << "\"" << std::endl;
    if(!ttrees_[sample]){
	  	std::cout<<"[HhhAnalysis::GetRate] Warning: sample "<<sample<<" not found. Setting rate to 0"<<std::endl;
	  	return std::make_pair(0.,0.); 
		}
    std::string full_selection = BuildCutString(selection, category, weight);
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw("0.5>>htemp(1,0,1)", full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    auto result = std::make_pair(Integral(htemp), Error(htemp));
    gDirectory->Delete("htemp;*");
		return result;
		
  }

  std::pair<double, double> HhhAnalysis::GetLumiScaledRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetRate(sample, selection, category, weight);
    double sf = GetLumiScale(sample);
    result.first *= sf;
    result.second *= sf;
    return result;
  }
  std::pair<double, double> HhhAnalysis::GetLumiScaledRate(std::vector<std::string> const& samples, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetLumiScaledRate(samples.at(0), selection, category, weight);
    double err_sqr = result.second * result.second;
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        auto tmp = GetLumiScaledRate(samples.at(i), selection, category, weight);
        result.first += tmp.first;
        err_sqr += (tmp.second * tmp.second);
      }
    }
    result.second = sqrt(err_sqr);
    return result;
  }

  std::pair<double, double> HhhAnalysis::SampleEfficiency(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    auto num = GetRate(sample, target_selection, target_category, weight);
    auto den = GetRate(sample, ref_selection, ref_category, weight);
    double num_eff = std::pow(num.first / num.second, 2.0) ;
    unsigned num_eff_rounded = unsigned(num_eff+0.5);
    double den_eff = std::pow(den.first / den.second, 2.0) ;
    unsigned den_eff_rounded = unsigned(den_eff+0.5);
    double eff = num.first / den.first;
    TEfficiency teff;
    double eff_err_up   = teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,1)-(num_eff/den_eff);
    double eff_err_down = (num_eff/den_eff)-teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,0);
    double eff_err = (eff_err_up/(num_eff/den_eff)) * eff;
    if (num.first == 0.0) {
      std::cout << "[HhhAnalysis::SampleEfficiency] Numerator is zero, setting error to zero" << std::endl;
      eff_err = 0.0;
    }
    auto result = std::make_pair(eff, eff_err);
    if (verbosity_ > 0) {
      std::cout << "[HhhAnalysis::SampleEfficiency]" << std::endl;
      std::cout << "Numerator:   " << boost::format("%s,'%s','%s','%s'\n") % sample % target_selection
                % target_category % weight;
      std::cout << "Denominator: " << boost::format("%s,'%s','%s','%s'\n") % sample % ref_selection
                % ref_category % weight;
      PrintValue("Numerator",num);
      PrintValue("Denominator",den);
      std::cout << "Effective Numerator:   " << num_eff_rounded << std::endl;
      std::cout << "Effective Denominator: " << den_eff_rounded << std::endl;
      std::cout << "Error down (relative): " << eff_err_down/(num_eff/den_eff) << std::endl;
      std::cout << "Error up   (relative): " << eff_err_up/(num_eff/den_eff) << std::endl;
    }
    return result;
  }

  HhhAnalysis::Value HhhAnalysis::SampleRatio(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    Value num = GetRate(sample, target_selection, target_category, weight);
    Value den = GetRate(sample, ref_selection, ref_category, weight);
    return ValueDivide(num, den);
  }

  HhhAnalysis::Value HhhAnalysis::GetRateViaRefEfficiency(std::string const& target_sample, 
                          std::string const& ref_sample,
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    if (verbosity_) {
      std::cout << "[HhhAnalysis::GetRateViaRefEfficiency]\n";
      std::cout << "ReferenceRate:   " << boost::format("%s,'%s','%s','%s'\n") % ref_sample % ref_selection 
                % ref_category % weight;
      std::cout << "Efficiency:      " << boost::format("%s,'%s','%s','%s'\n") %target_sample % target_selection 
                % target_category % weight;
    }
    auto ref_rate = GetLumiScaledRate(ref_sample, ref_selection, ref_category, weight);
    if (verbosity_) PrintValue("ReferenceRate", ref_rate);
    auto target_eff = SampleEfficiency(target_sample, ref_selection, ref_category, target_selection, target_category, weight);
    if (verbosity_) PrintValue("Efficiency", target_eff);
    return ValueProduct(ref_rate, target_eff);
  }

  HhhAnalysis::Value HhhAnalysis::GetRateViaFakesMethod(std::string const& sel,
                              std::string const& cat, 
                              std::string const& wt) {
    auto e_fakes = this->GetRate("Special_20_Data", sel, cat, wt);
    auto m_fakes = this->GetRate("Special_21_Data", sel, cat, wt);
    auto em_fakes = this->GetRate("Special_22_Data", sel, cat, wt);
    if (verbosity_) PrintValue("Electron Fakes", e_fakes);
    if (verbosity_) PrintValue("Muon Fakes", m_fakes);
    if (verbosity_) PrintValue("Double Fakes", em_fakes);
    auto norm = ValueAdd(e_fakes, m_fakes);
    norm = ValueSubtract(norm, em_fakes);
    return norm;
  }


  HhhAnalysis::Value HhhAnalysis::GetRateViaWMethod(std::string const& w_sample,
                          std::string const& ratio_cat,
                          std::string const& ratio_control_sel,
                          std::string const& ratio_signal_sel,
                          std::string const& data_sample,
                          std::string const& cat,
                          std::string const& control_sel,
                          std::vector<std::string> const& sub_samples,
                          std::string const& wt,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HhhAnalysis::GetRateViaWMethod]\n";
      std::cout << "ExtrapFactor:   " << boost::format("%s,'%s'/'%s','%s','%s'\n") % w_sample % ratio_signal_sel 
                % ratio_control_sel % ratio_cat % wt;
      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % control_sel % cat % wt;
    }
    Value ratio = SampleRatio(w_sample, ratio_control_sel, ratio_cat, ratio_signal_sel, ratio_cat, wt);
    Value data_control = GetRate(data_sample, control_sel, cat, wt);
    if (verbosity_) PrintValue(data_sample, data_control);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_sel, cat, wt);
      }
      if (verbosity_) PrintValue("-"+sub_samples[i], bkr);
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    if (verbosity_) PrintValue("TotalBkg", total_bkg);
    double w_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value w_control(data_control.first - total_bkg.first, w_control_err);
    if (verbosity_) PrintValue("WSideband", w_control);
    if (verbosity_) PrintValue("ExtrapFactor", ratio);
    Value w_signal = ValueProduct(w_control, ratio);
    return w_signal;
  }

  HhhAnalysis::Value HhhAnalysis::GetRateViaQCDMethod(HhhAnalysis::Value const& ratio,
                          std::string const& data_sample,
                          std::string const& control_selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HhhAnalysis::GetRateViaQCDMethod]\n";
      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % control_selection % category % weight;
    }
    Value data_control = GetRate(data_sample, control_selection, category, weight);
    if (verbosity_) PrintValue(data_sample, data_control);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_selection, category, weight);
      }
      if (verbosity_) PrintValue("-"+sub_samples[i], bkr);
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    if (verbosity_) PrintValue("TotalBkg", total_bkg);
    double qcd_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value qcd_control(data_control.first - total_bkg.first, qcd_control_err);
    if (verbosity_) PrintValue("QCDSideband", qcd_control);
    Value qcd_signal = ValueProduct(qcd_control, ratio);
    if (verbosity_) PrintValue("OS/SS Factor", ratio);
    return qcd_signal;
  }

  TH1F HhhAnalysis::GetShapeViaQCDMethod(std::string const& variable,
                          std::string const& data_sample,
                          std::string const& selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HhhAnalysis::GetShapeViaQCDMethod]\n";
      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % selection % category % weight;

    }
    TH1F result = GetLumiScaledShape(variable, data_sample, selection, category, weight);
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      if (dict.count(sub_samples[i])) {
        Value bkr_rate = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
        TH1F tmp = GetShape(variable, sub_samples.at(i), selection, category, weight);
        SetNorm(&tmp, bkr_rate.first);
        result.Add(&tmp, -1.);
      } else {
        TH1F tmp = GetLumiScaledShape(variable, sub_samples[i], selection, category, weight);
        result.Add(&tmp, -1.);
      }
    }
    return result;
  }

  TH1F HhhAnalysis::GetShapeViaFakesMethod(std::string const& var,
                              std::string const& sel,
                              std::string const& cat,
                              std::string const& wt) {
    TH1F fr_e_hist =  this->GetShape(var, "Special_20_Data", sel, cat, wt);
    TH1F fr_m_hist =  this->GetShape(var, "Special_21_Data", sel, cat, wt);
    TH1F fr_em_hist = this->GetShape(var, "Special_22_Data", sel, cat, wt);
    fr_e_hist.Add(&fr_m_hist,1.0);
    fr_e_hist.Add(&fr_em_hist,-1.0);
    return fr_e_hist;
  }

  HhhAnalysis::Value HhhAnalysis::ValueProduct(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 || p2.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[HhhAnalysis::ValueProduct] At least one value is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first * p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HhhAnalysis::Value HhhAnalysis::ValueDivide(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 && p2.first == 0.0) {
      std::cout << "[HhhAnalysis::ValueDivide] Numerator and denominator both zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p1.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[HhhAnalysis::ValueDivide] Numerator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p2.first == 0.0) {
      std::cout << "[HhhAnalysis::ValueDivide] Denominator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first / p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HhhAnalysis::Value HhhAnalysis::ValueAdd(Value const& p1, Value const& p2) {
    double f = p1.first + p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }
  HhhAnalysis::Value HhhAnalysis::ValueSubtract(Value const& p1, Value const& p2) {
    double f = p1.first - p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }

  void HhhAnalysis::PrintValue(std::string const& label, Value const& val) {
    std::cout << (boost::format("%-45s %10.3f +/-   %10.3f  (%.4f)") % (label+":") % val.first % val.second % (val.second/val.first)) << std::endl;
  }

  std::string HhhAnalysis::ResolveAlias(std::string const& al) {
    if (alias_map_.count(al)) {
      return alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HhhAnalysis::ResolveAlias>: No alias for " << al << " found" << std::endl;
      return al;
    }
  }

  std::vector<std::string> HhhAnalysis::ResolveSamplesAlias(std::string const& al) {
    if (samples_alias_map_.count(al)) {
      return samples_alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HhhAnalysis::ResolveSamplesAlias>: No alias for " << al << " found" << std::endl;
      return std::vector<std::string>();
    }
  }

  void HhhAnalysis::SetAlias(std::string const& al, std::string const& val) {
    alias_map_[al] = val;
  }

  double HhhAnalysis::KolmogorovTest(std::string const& variable,
      std::string const& sample1, std::string const& selection1, std::string const& category1,
      std::string const& sample2, std::string const& selection2, std::string const& category2,
      std::string const& weight) {
    std::cout << "[HhhAnalysis::KolmogorovTest] Calculating statistic for shapes:" << std::endl;
    std::cout << "[1] " << boost::format("%s,'%s','%s','%s'\n") % sample1 % selection1 % category1 % weight;
    std::cout << "[2] " << boost::format("%s,'%s','%s','%s'\n") % sample2 % selection2 % category2 % weight;
    std::string full1 = BuildCutString(selection1, category1, weight);
    std::string full2 = BuildCutString(selection2, category2, weight);
    TH1::AddDirectory(true);
    ttrees_[sample1]->Draw(">>elist", full1.c_str(), "entrylist");
    TEntryList *elist1 = (TEntryList*)gDirectory->Get("elist");
    TH1::AddDirectory(false);
    unsigned entries1 = elist1->GetN();
    double x1;
    double wt1;
    std::vector<std::pair<double,double>> a(entries1,std::make_pair(0.,0.));
    ttrees_[sample1]->SetBranchAddress(variable.c_str(), &x1);
    ttrees_[sample1]->SetBranchAddress(weight.c_str(), &wt1);
    for (unsigned i = 0; i < entries1; ++i) {
      unsigned entry = elist1->GetEntry(i);
      ttrees_[sample1]->GetEntry(entry);
      a[i].first = x1;
      a[i].second = wt1;
    }
    gDirectory->Delete("elist;*");
    TH1::AddDirectory(true);
    ttrees_[sample2]->Draw(">>elist", full2.c_str(), "entrylist");
    TEntryList *elist2 = (TEntryList*)gDirectory->Get("elist");
    TH1::AddDirectory(false);
    unsigned entries2 = elist2->GetN();
    double x2;
    double wt2;
    std::vector<std::pair<double,double>> b(entries2,std::make_pair(0.,0.));
    ttrees_[sample2]->SetBranchAddress(variable.c_str(), &x2);
    ttrees_[sample2]->SetBranchAddress(weight.c_str(), &wt2);
    for (unsigned i = 0; i < entries2; ++i) {
      unsigned entry = elist2->GetEntry(i);
      ttrees_[sample2]->GetEntry(entry);
      b[i].first = x2;
      b[i].second = wt2;
    }
    gDirectory->Delete("elist;*");
    std::sort(a.begin(), a.end(), [](const std::pair<double, double>& first, const std::pair<double, double>& second)
      {
        return first.first < second.first;
      });
    std::sort(b.begin(), b.end(), [](const std::pair<double, double>& first, const std::pair<double, double>& second)
      {
        return first.first < second.first;
      });
    double prob = -1;
    int na = a.size();
    int nb = b.size();
    double rna = 0.;
    double rnb = 0.;
    for (unsigned i = 0; i < a.size(); ++i) rna += a[i].second;
    for (unsigned i = 0; i < b.size(); ++i) rnb += b[i].second;

    /* 
      The implementation below shamelessly stolen from:
      http://root.cern.ch/root/html/TMath.html#TMath:KolmogorovTest
      and modified to take into account weighted events.  Quite possible
      that this is not statistically valid, but seems to behave well enough
      when weights are close to unity.
    */
    // double rna = a.size();
    // double rnb = b.size();
    std::cout << "Entries: (1) " << rna << " (2) " << rnb << std::endl;
    // double sa  = 1./rna;
    // double sb  = 1./rnb;
    double rdiff = 0;
    double rdmax = 0;
    int ia = 0;
    int ib = 0;
    bool ok = false;
    for (int i=0;i<na+nb;i++) {
      if (a[ia].first < b[ib].first) {
         rdiff -= a[ia].second/rna;
         ia++;
         if (ia >= na) {ok = true; break;}
      } else if (a[ia].first > b[ib].first) {
         rdiff += b[ib].second/rnb;
         ib++;
         if (ib >= nb) {ok = true; break;}
      } else {
         // special cases for the ties
         double x = a[ia].first;
         while(a[ia].first == x && ia < na) {
            rdiff -= a[ia].second/rna;
            ia++;
         }
         while(b[ib].first == x && ib < nb) {
            rdiff += b[ib].second/rnb;
            ib++;
         }
         if (ia >= na) {ok = true; break;}
         if (ib >= nb) {ok = true; break;}
      }
      rdmax = TMath::Max(rdmax,TMath::Abs(rdiff));
    }
    if (ok) {
      rdmax = TMath::Max(rdmax,TMath::Abs(rdiff));
      Double_t z = rdmax * TMath::Sqrt(rna*rnb/(rna+rnb));
      prob = TMath::KolmogorovProb(z);
    }
    std::cout << " Kolmogorov Probability = " << prob << ", rmax=" << rdmax << std::endl;
    return prob;
  }




}

