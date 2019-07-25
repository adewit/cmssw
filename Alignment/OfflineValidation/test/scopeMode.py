#/env/python 
import ROOT

tfile = ROOT.TFile("PrimaryVertexValidation_PVvalidationPixelFlag_hp2300_2335.root")
tfile.cd("PVValidation/Abs_L1Residuals")
dirList = ROOT.gDirectory.GetListOfKeys()
for k1 in dirList: 
  h1 = k1.ReadObj()
  if("TH1" in h1.ClassName()):
      print h1.ClassName(), " ", h1.GetName(), " ",h1.GetEntries()
      
