#!/bin/tcsh
set remote=(/store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_25_22_DATA_PixQualityFlagRun2017H-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_27_23_DATA_PixQualityFlagRun2017G-TkAlMinBias-17Nov2017-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_28_54_DATA_PixQualityFlagRun2017B-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_30_53_DATA_PixQualityFlagRun2017B-TkAlMinBias-PromptReco-v2 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_31_22_DATA_PixQualityFlagRun2017C-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_32_06_DATA_PixQualityFlagRun2017C-TkAlMinBias-PromptReco-v2 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_34_21_DATA_PixQualityFlagRun2017C-TkAlMinBias-PromptReco-v3 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_36_32_DATA_PixQualityFlagRun2017D-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_38_45_DATA_PixQualityFlagRun2017E-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/test_2019_04_21_16_40_28_DATA_PixQualityFlagRun2017F-TkAlMinBias-PromptReco-v1)
set myDirs=(EOY17)

###Do the same for alignment candidate
#set remote=(/store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017B-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017B-TkAlMinBias-PromptReco-v2 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017C-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017C-TkAlMinBias-PromptReco-v2 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017C-TkAlMinBias-PromptReco-v3 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017D-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017E-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017F-TkAlMinBias-PromptReco-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017G-TkAlMinBias-17Nov2017-v1 /store/group/alca_trackeralign/adewit/test_out/2017UltraLegacyRun2017H-TkAlMinBias-PromptReco-v1)
#set myDirs=(aligncand)


foreach i (`seq $#myDirs`)
    if (! -d ./$myDirs[$i] ) then
        mkdir ./$myDirs[$i]
    else
        echo "$myDirs[$i] already exists!"
    endif
end

foreach j (`seq $#remote`)
    foreach file (`eos ls $remote[$j]`)
        foreach i (`seq $#myDirs`)
            if($file =~ *"$myDirs[$i]"*) then
                if(-f ./$myDirs[$i]/$file) then
                    echo "$file already exists in $myDirs[$i] ---> exiting"
                else
                    echo "copying file $file to $myDirs[$i]"
                    xrdcp root://eoscms.cern.ch//eos/cms/$remote[$j]/$file ./$myDirs[$i]
                endif
            endif
        end
    end
end
