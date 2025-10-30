from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config
import os

# -------------------------
# Basic CRAB configuration
# -------------------------
config = config()
config.General.workArea = 'crab_test11'
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'private_nano_config.py'
config.JobType.outputFiles = ['HIG-Run3Summer22EENanoAODv12-00588.root']

config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.totalUnits = -1
config.Data.publication = True

config.Site.storageSite = 'T2_FR_GRIF'

# List of datasets
datasets = [
    "/GluGluHTo2TauUncorrelatedDecay_Filtered_M-125_CP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv4-130X_mcRun3_2022_realistic_postEE_v6-v2/MINIAODSIM"
]

# -------------------------
# Submit each dataset
# -------------------------
for dataset in datasets:
    config.General.requestName = dataset.split('/')[1].split('-')[0]
    config.Data.inputDataset = dataset
    config.Data.outputDatasetTag = 'nanov12-22EE'

    print("Submitting CRAB task for dataset: %s" % dataset)
    try:
        crabCommand('submit', config=config)
        print("Successfully submitted %s" % dataset)
    except Exception as e:
        print("Error submitting %s:\n%s\n" % (dataset, e))
