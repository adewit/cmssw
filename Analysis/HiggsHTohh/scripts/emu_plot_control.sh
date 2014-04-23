
if (( "$#" != "4" ))
then
  echo "Usage: new_plot_control.sh [PROD] [YEAR] [ANALYSIS] [POSTFIT]"
  exit
fi


PROD=$1
YEAR=$2
ANA=$3
POSTFIT=$4
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"

################################################################################
##### Inclusive selection plots
################################################################################


#### MET

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --norm_bins=true --datacard="jpt1_20_jpt2_20_no-tag" --extra_pad=1.1 \
  --background_scheme="em_default" --scale_signal_datacard=true --verbosity=2 $EM_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="met_phi(80,-3.15,3.15)" --x_axis_label="E_{T}^{miss} #phi [rad]" \
  --custom_x_axis_range=true --x_axis_min=-3.15 --x_axis_max=3.15 \
  --norm_bins=true --datacard="jpt1_20_jpt2_20_no-tag" --extra_pad=1.1 \
  --background_scheme="em_default" --scale_signal_datacard=true  $EM_INC_SHIFT


#### pt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pt_1(50,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### pt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pt_2(50,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### pt_tt

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### pt_tt log
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" --log_y=true --draw_ratio=true \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### eta_1


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1"\
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Electron #eta" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### eta_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1"\
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="eta_2(30,-3,3)" --extra_pad=1.8 \
  --x_axis_label="Muon #eta" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true $EM_INC_SHIFT

#### m_2

#### n_jets (log)


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99 --scale_signal_datacard=true

#### n_bjets (log)


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99 --scale_signal_datacard=true

#### n_prebjets (log)


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of prebtag Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99 --scale_signal_datacard=true

#### mt_1 / pzeta


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pzeta(100,-300,300)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="D_{#zeta} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pzetavis(25,0,50)"  $EM_BAND_ONLY \
  --x_axis_label="P_{#zeta}^{vis} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true

 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="pzetamiss(100,-300,300)"  $EM_BAND_ONLY \
  --x_axis_label="#slash{P}_{#zeta} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=2.0 --scale_signal_datacard=true

 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="em_gf_mva(40,-1,1)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="em_gf_mva(40,-1,1)" --extra_pad=100. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="jpt1_20_jpt2_20_no-tag" --log_y=true --draw_ratio=true \
  --background_scheme="em_default" --scale_signal_datacard=true
 
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="mt_ll(40,0,160)" $EM_BAND_ONLY \
  --x_axis_label="m_{T}(ll) [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="emu_dphi(40,0,3.15)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="#Delta#phi_{e,#mu}" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="emu_dxy_1(40,-0.02,0.02)" --extra_pad=100 --log_y=true --draw_ratio=true \
  --x_axis_label="Electron d_{0}^{vtx} [cm]" --datacard="jpt1_20_jpt2_20_no-tag" $EM_BAND_ONLY \
  --background_scheme="em_default" --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="emu_dxy_2(40,-0.02,0.02)" --extra_pad=100 --log_y=true --draw_ratio=true \
  --x_axis_label="Muon d_{0}^{vtx} [cm]" --datacard="jpt1_20_jpt2_20_no-tag" $EM_BAND_ONLY \
  --background_scheme="em_default" --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="emu_dxy(40,0.0,0.04)" --extra_pad=100 --log_y=true --draw_ratio=true \
  --x_axis_label="E/Muon d_{0}^{vtx} [cm]" --datacard="jpt1_20_jpt2_20_no-tag" $EM_BAND_ONLY \
  --background_scheme="em_default" --scale_signal_datacard=true




#### n_vtx


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="n_vtx(30,0,30)" $EM_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="jpt1_20_jpt2_20_no-tag" --extra_pad=1.8 \
  --background_scheme="em_default" --scale_signal_datacard=true

#### tau_decay_mode

###############################################################################
#### Jet selection plots
###############################################################################

#### jpt_1


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="jpt_1(20,0,200)" $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true

#### jeta_1


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="jeta_1(20,-5,5)" --extra_pad=1.9 $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true

#### jpt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="jpt_2(20,0,200)" $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true

#### jeta_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="jeta_2(20,-5,5)" --extra_pad=1.9 $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --scale_signal_datacard=true

#### bpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_med-tag" --var="bpt_1(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_med-tag" \
  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_med-tag" --var="em_gf_mva(40,-1,1)" $EM_BTAG_SHIFT \
  --x_axis_label="BDT output" --datacard="jpt1_20_jpt2_20_med-tag" \
  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true


#### beta_1
BETA_BINS="beta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="beta_1(15,-3,3)"
fi

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_med-tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="jpt1_20_jpt2_20_med-tag" $EM_BTAG_SHIFT \
  --background_scheme="em_default" --scale_signal_datacard=true

#### bpt_2

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
#  --method=8 --cat="jpt1_20_jpt2_20_twomed-tag" --var="bpt_2(20,0,200)" $EM_BTAG_SHIFT \
#  --x_axis_label="Subleading b-tagged jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_twomed-tag" \
#  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true

#### beta_2
BETA_BINS="beta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="beta_2(15,-3,3)"
fi

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
#  --method=8 --cat="jpt1_20_jpt2_20_twomed-tag" --var=$BETA_BINS --extra_pad=2.0 \
#  --x_axis_label="Subleading b-tagged jet #eta" --datacard="jpt1_20_jpt2_20_twomed-tag" $EM_BTAG_SHIFT \
#  --background_scheme="em_default" --scale_signal_datacard=true



#### prebjetpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="prebjetpt_1(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true

#### prebjeteta_1
BETA_BINS="prebjeteta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_1(15,-3,3)"
fi

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading selected jet #eta" --datacard="jpt1_20_jpt2_20_no-tag" $EM_BTAG_SHIFT \
  --background_scheme="em_default" --scale_signal_datacard=true

#### prebjetpt_2


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="prebjetpt_2(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.3 --scale_signal_datacard=true

#### prebjeteta_2
BETA_BINS="prebjeteta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_2(15,-3,3)"
fi

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading selected jet #eta" --datacard="jpt1_20_jpt2_20_no-tag" $EM_BTAG_SHIFT \
  --background_scheme="em_default" --scale_signal_datacard=true

#### prebjetcsv_1


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="jpt1_20_jpt2_20_no-tag"  $EM_BTAG_SHIFT \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true

#### prebjetcsv_2


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="jpt1_20_jpt2_20_no-tag"  $EM_BTAG_SHIFT \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true


#### prebjetjdeta


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="jpt1_20_jpt2_20_no-tag" --var="prebjet_deta(20,0,10)" $EM_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="jpt1_20_jpt2_20_no-tag" \
  --background_scheme="em_default" --extra_pad=1.2 --scale_signal_datacard=true
