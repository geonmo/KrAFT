KrAFT
=====

Korea CMS Analysis Framwork for Top quark physics
#Installation

##Step 1 : Initialize PAT configuration


    cmsrel CMSSW_5_3_13
    cd CMSSW_5_3_13/src
    cmsenv
    git-cms-addpkg EgammaAnalysis/ElecronTools
    git-cms-addpkg TopQuarkAnalysis/Configuration
    git clone git@github.com:cms-kr/KrAFT
    git cms-merge-topic -u cms-analysis-tools:5_3_13-updateTopRefSel
    cd EgammaAnalysis/ElectronTools/data
    cat download.url | xargs wget
    cd -


##Step 2 : Install KrAFT codes

    git clone git@github.com:cms-kr/KrAFT
    scram b -j4 
