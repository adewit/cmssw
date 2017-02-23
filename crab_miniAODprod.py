from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Feb20_MC_82X'
config.section_('JobType')
config.JobType.psetName = 'vbf_pu200_miniAOD.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['vbf_pu200_miniAOD_PAT.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
#config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=1', 'globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 500
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = True
config.Data.ignoreLocality= True
config.Data.allowNonValidInputDataset = True
config.Data.outLFNDirBase='/store/user/adewit/Feb20_MC_82X/'
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT',  'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
#config.Site.blacklist = ['T2_US_*']
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers
            

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()
  
    tasks.append(('VBFPu200-fast','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIIFall16DR82-PU200_90X_upgrade2023_realistic_v1-v1/AODSIM'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



