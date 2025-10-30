# Private re-nanoAOD

Some instructions for re-nanoAODing can be found here: https://gitlab.cern.ch/cms-nanoAOD/nanoaod-doc/-/wikis/Instructions/Private%20production  
Note that some of it is outdated so we will go through what is needed in a bit more detail here.

For nanoAOD v12, the CMSSW release is CMSSW_13_0_13, we can start by checking this out:

```
cmsrel CMSSW_13_0_13
cd CMSSW_13_0_13/src
cmsenv
git cms-addpkg PhysicsTools/NanoAOD
git cms-checkout-topic adewit:from_CMSSW_13_0_13
scram b -j 4
cd PhysicsTools/NanoAOD
```

This merges in the changes needed to add the TauSpinner weights in nanoAOD v12

Then we need to find the cmsDriver command that was used to generate the nanoAOD configs; this can be extracted from McM:
https://cms-pdmv-prod.web.cern.ch/mcm/requests?page=0&shown=127

Click on `output dataset` and paste the nanoAOD dataset name that we need to recreate, then search. This should give a one-line output. 
Under "actions", click the button that looks like a tick mark inside a circle ('get test command'). This will open for example: https://cms-pdmv-prod.web.cern.ch/mcm/public/restapi/requests/get_test/HIG-Run3Summer22EENanoAODv12-00588

This contains a cmsDriver command:

```
cmsDriver.py  --eventcontent NANOEDMAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring
--datatier NANOAODSIM --conditions 130X_mcRun3_2022_realistic_postEE_v6 --step NANO --scenario pp
--era Run3 --python_filename HIG-Run3Summer22EENanoAODv12-00588_1_cfg.py
--fileout file:HIG-Run3Summer22EENanoAODv12-00588.root
--filein "dbs:/GluGluHTo2TauUncorrelatedDecay_Filtered_M-125_CP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv4-130X_mcRun3_2022_realistic_postEE_v6-v2/MINIAODSIM"
--number 1763 --number_out 1763 --no_exec --mc
```

This needs to be modified a little bit, to:

```
cmsDriver.py  --eventcontent NANOAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring
--datatier NANOAODSIM --conditions 130X_mcRun3_2022_realistic_postEE_v6 --step NANO --scenario pp
--era Run3 --python_filename private_nano_config_22EE.py --fileout file:HIG-Run3Summer22EENanoAODv12-00588.root
--filein "file:input.root" -n -1 --no_exec --mc
```

Now we have a config that can be submitted to crab using crab_submit.py. Note, we need a separate config for each period (22,22EE, 23BPix, 23) as the conditions tag could be different, but one config can be used for different datasets (which should be listed in crab_submit.py, which currently contains only one dataset)

Before submitting with crab, need to
`source /cvmfs/cms.cern.ch/common/crab-setup.sh`

then you can run
`python3 crab_submit.py` after setting up a grid certificate (`voms-proxy-init --voms cms`)

It's also a good idea to check you can write to `T2_FR_GRIF`, otherwise the files cannot be copied over at the end of the jobs:
`crab checkwrite --site=T2_FR_GRIF`

More instructions for CRAB can be found here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCrab 
