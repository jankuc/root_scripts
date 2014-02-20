#!/usr/bin/env bash

# Yun-Tse 2011/05/02: Copied from the BNN
##############################################################################
# Do these channels
####################

recovers=${1:-`cat do_these_channels.txt | grep -v "#" | grep "reco_versions" | awk '{print $3}'`}
if [ "${recovers}" = "p17,p20" ] || [ "${recovers}" = "p20,p17" ] ; then
  recovers=${recovers}",RecoCombined"
fi
recovers=${recovers//","/" "}

leptons=${2:-`cat do_these_channels.txt | grep -v "#" | grep "leptons" | awk '{print $3}'`}
if [ "${leptons}" = "CC,MU" ] || [ "${leptons}" = "MU,CC" ] ; then
   leptons=${leptons}",LeptonsCombined"
#elif [ "$leptons" = "CC,MU,EC" ] || [ "$leptons" = "MU,CC,EC" ] || \
#     [ "$leptons" = "MU,EC,CC" ] || [ "$leptons" = "EC,MU,CC" ] || \
#     [ "$leptons" = "CC,EC,MU" ] || [ "$leptons" = "EC,CC,MU" ] ; then
#    leptons=$leptons",LeptonsCombinedEC"
fi
leptons=${leptons//","/" "}

tagbins=${3:-`cat do_these_channels.txt | grep -v "#" | grep "tag_bins" | awk '{print $3}'`}
if [ "${tagbins}" = "EqOneTag,EqTwoTag" ] || [ "${tagbins}" = "EqTwoTag,EqOneTag" ] ; then
   tagbins=${tagbins}",TagsCombined"
fi
tagbins=${tagbins//","/" "}

jetbins=${4:-`cat do_these_channels.txt | grep -v "#" | grep "jet_bins" | awk '{print $3}'`}
if [ "${jetbins}" = "EqTwoJet,EqThreeJet" ] || [ "${jetbins}" = "EqThreeJet,EqTwoJet" ] ; then
   jetbins=${jetbins}",JetsCombined"
fi
jetbins=${jetbins//","/" "}

##############################################################################
# Parameters
##############

##############
# Put this two parameters to 'yes' or 'no' and they will take care about the
# jetbin, the input directory and the output directory.
# For wjets crosscheck sample it will only do EqTwoJet.
# For ttbar crosscheck sample it will only do EqThreeJet.
# You can set both to 'yes' if you want.
make_plots_for_wjets_crosscheck_sample=no
make_plots_for_ttbar_crosscheck_sample=no

##############
# Directory where the ROOT files with the discriminant histograms are located.
# It is assumed the directory structure inside "inputdir" is:
# reco/lepton/tagbin/jetbin/

inputdir="/work/budvar-clued0/francji/results/yield_sample"
# inputdir="/prj_root/7008/top_write/yuntse/SingleTopSummer2011/BNNComb/Disc.DataInt/TransBins_t_4ch/LooseTightCorr.v9/subsets/yield_sample"


wjets_crosscheck_samples_inputdir="${inputdir}/crosscheck_samples/wjets"
ttbar_crosscheck_samples_inputdir="${inputdir}/crosscheck_samples/ttbar"

##############
# Directory where to put the eps and png figures of the discriminant specified in "inputdir".
# A subdirectory structure will be created inside "outputdir":
# reco/lepton/tagbin/jetbin/

outputdir="/work/budvar-clued0/francji/results/plots"
# outputdir="/prj_root/7008/top_write/yuntse/SingleTopSummer2011/BNNComb/Disc.DataInt/TransBins_t_4ch/LooseTightCorr.v9/DataMC"

wjets_crosscheck_samples_outputdir="${outputdir}/crosscheck_samples/wjets"
ttbar_crosscheck_samples_outputdir="${outputdir}/crosscheck_samples/ttbar"

##############
# Name of the discriminant histogram in the input ROOT files
# This will be modified according to the signals
#hstr=GLM_tqb
#hstr=GLM_tbtqb
hstr=BDT_tb


outputdir=${outputdir}/${hstr}
wjets_crosscheck_samples_outputdir=${wjets_crosscheck_samples_outputdir}/${hstr}
ttbar_crosscheck_samples_outputdir=${ttbar_crosscheck_samples_outputdir}/${hstr}

#############
# List of all the sources present in the input directory.

types="DATA QCD tb tqb wlp wbb wcc ttbar-lepjets ttbar-dilepton zlp zbb zcc diboson"

##############
# What is the signal source?
 signals="tb"
# signals="tqb"
# signals="tbtqb"

##############
# Should the plot be done in log scale in the Y-axis?
logy=no #yes

##############
# Number of bins in the BNN-output axis (X-axis) in the plot
# This will be modified according to the signals as well
# Nbins=9
Nbins=25
# Nbins=50
# Nbins=8
xmin=0
xmax=1.0
# xmax=2.0

##############
# "medir" is where the ME packages are located
# and specially the subdirectory 'topd0root_me/macros' with all the scripts.
medir="/work/budvar-clued0/francji/scripts/yuntse"
##############
# By how much we have to scale the signal contribution (both, tb and tqb)
signal_scale_factor= 1  #1.3583815 #tbtqb 4.70/3.46
#signal_scale_factor=1.686359 #tqb
#signal_scale_factor=0.69192857 #tb
# signal_scale_factor=1.22031 # tb 9.7 ifb
# signal_scale_factor=0.93589

##############################################################################

error() {
    echo -e "\nError: $1\n"
    exit 1
}

# Print parameters
echo -e "\nME directory: ${medir}"
echo -e "Make plots for the wjets cross check samples? ${make_plots_for_wjets_crosscheck_sample}"
echo -e "Make plots for the ttbar cross check samples? ${make_plots_for_ttbar_crosscheck_sample}"
if   [ $make_plots_for_wjets_crosscheck_sample = no -a $make_plots_for_ttbar_crosscheck_sample = no ] ; then
    echo -e "Input directory: ${inputdir}"
    echo -e "Output directory: ${outputdir}"
elif [ $make_plots_for_wjets_crosscheck_sample = yes -a $make_plots_for_ttbar_crosscheck_sample = no  ] ; then
    echo -e "Input directory: ${wjets_crosscheck_samples_inputdir}"
    echo -e "Output directory: ${wjets_crosscheck_samples_outputdir}"
elif [ $make_plots_for_wjets_crosscheck_sample = no  -a $make_plots_for_ttbar_crosscheck_sample = yes ] ; then

    echo -e "Input directory: ${ttbar_crosscheck_samples_inputdir}"
    echo -e "Output directory: ${ttbar_crosscheck_samples_outputdir}"
elif [ $make_plots_for_wjets_crosscheck_sample = yes -a $make_plots_for_ttbar_crosscheck_sample = yes ] ; then
    echo -e "Input directories: ${wjets_crosscheck_samples_inputdir} and ${ttbar_crosscheck_samples_inputdir}"
    echo -e "Output directories: ${wjets_crosscheck_samples_outputdir} and ${ttbar_crosscheck_samples_outputdir}"
fi

echo -e "Input histogram name: ${hstr}"
echo -e "Sources: ${types}"
echo -e "Signals: ${signals}"
echo -e "Use logarithmic scale in the Y axis? ${logy}"
echo -e "Numbers of bins in the discriminant plot: ${Nbins}"
echo -en "\nAre the parameters above OK? [\"y\" to proceed] : "
#read answ
#[ "${answ}" != "y" ] && [ "${answ}" != "Y" ] && exit 0

[ ! -d $inputdir ] && error "directory ${inputdir} does not exist!"

logstr=""; yminstr="Histo.Ymin: .0"
[ $logy = yes ] && { logstr="_logy"; yminstr="#Histo.Ymin: 10"; }

# Loop over all channels
ch=0
for reco   in ${recovers} ; do
for lepton in ${leptons} ;  do
for tagbin in ${tagbins} ;  do
for jetbin in ${jetbins} ;  do

    [ $make_plots_for_wjets_crosscheck_sample = yes -a $make_plots_for_ttbar_crosscheck_sample = no  ] && [[ $jetbin != EqTwoJet  ]]  && continue
    [ $make_plots_for_ttbar_crosscheck_sample = yes -a $make_plots_for_wjets_crosscheck_sample = no  ] && [[ $jetbin != EqThreeJet ]] && continue
    [ $make_plots_for_ttbar_crosscheck_sample = yes -a $make_plots_for_wjets_crosscheck_sample = yes ] &&  [ $jetbin = EqThreeJet ]   && continue
    [ $make_plots_for_ttbar_crosscheck_sample = yes -o $make_plots_for_wjets_crosscheck_sample = yes ] &&  [ $jetbin = JetsCombined ] && continue

    if [ $make_plots_for_wjets_crosscheck_sample = yes ] && [ $jetbin = EqTwoJet  ] ; then
	inputdir=$wjets_crosscheck_samples_inputdir
	outputdir=$wjets_crosscheck_samples_outputdir
    fi
    if [ $make_plots_for_ttbar_crosscheck_sample = yes ] && [ $jetbin = EqThreeJet ] ; then
	inputdir=$ttbar_crosscheck_samples_inputdir
	outputdir=$ttbar_crosscheck_samples_outputdir
    fi

    ch=`expr $ch + 1`
    echo
    echo "$ch) Doing channel "$reco"_"$lepton"_"$tagbin"_"$jetbin" ..."

    sleep 1

    # Skip combined cases
    #if [ "$lepton" == "CC" ] || [ "$lepton" == "MU" ] || [ "$lepton" == "EC" ] ; then
	#continue
    #fi

    echo
    echo "   Using $Nbins bins"
    echo

    # [ "$reco" == "p17" ] && lum="1.0fb^{-1}"
    [ "$reco" == "p17" ] && lum="9.7fb^{-1}"
    [ "$reco" == "p20" ] && lum="8.7fb^{-1}"
    [ "$reco" == "RecoCombined" ] && lum="9.7fb^{-1}"

    # [ "$lepton" == "CC" ] && channel="e+jets"
    [ "$lepton" == "CC" ] && channel="e+#mu channel"
    [ "$lepton" == "EC" ] && channel="e(EC)+jets"
    [ "$lepton" == "MU" ] && channel="#mu+jets"
    [ "$lepton" == "LeptonsCombined" ]   && channel="e+#mu channel"
    [ "$lepton" == "LeptonsCombinedEC" ] && channel="e(CC)+#mu channel"
    [ "$lepton" == "LeptonsCombinedEC" ] && [ "$jetbin" == "EqTwoJet" ]     && channel="e(CC,EC)+#mu channel"
    [ "$lepton" == "LeptonsCombinedEC" ] && [ "$jetbin" == "JetsCombined" ] && channel="e(CC,EC)+#mu channel"

    [ "$tagbin" == "EqZeroTag"    ] && tagtxt="0 b-tags"
    [ "$tagbin" == "EqOneTag"     ] && tagtxt="1 b-tag "
    [ "$tagbin" == "EqTwoTag"     ] && tagtxt="2 b-tags"
    [ "$tagbin" == "TagsCombined" ] && tagtxt="1-2 b-tags"

    [ "$jetbin" == "EqOneJet"     ] && jettxt="1 jet "
    [ "$jetbin" == "EqTwoJet"     ] && jettxt="2 jets"
    [ "$jetbin" == "EqThreeJet"   ] && jettxt="3 jets"
    [ "$jetbin" == "EqFourJet"    ] && jettxt="4 jets"
    [ "$jetbin" == "JetsCombined" ] && jettxt="2-3 jets"

    # haddDir=${inputdir}/${reco}/${lepton}/${tagbin}/${jetbin}
    # mkdir -p ${haddDir}
    haddDir=${inputdir}

    for sig in $signals ; do

	# outdir=${outputdir}/${reco}/${lepton}/${tagbin}/${jetbin}
	outdir=${outputdir}
	mkdir -p ${outdir}
	list=${outdir}/plotlist_${sig}_vs_all_${reco}_${lepton}_${tagbin}_${jetbin}_${Nbins}bins${logstr}.list
	rm -f ${list}

	if [ ${jetbin} == "JetsCombined" ] ; then
	     for type in ${types} ; do 
		h=${haddDir}/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root 
		# h2j=${inputdir}/${reco}/${lepton}/${tagbin}/EqTwoJet/${reco}_${lepton}_${type}_${tagbin}_EqTwoJet_zero_yield_sample.root
		# h3j=${inputdir}/${reco}/${lepton}/${tagbin}/EqThreeJet/${reco}_${lepton}_${type}_${tagbin}_EqThreeJet_zero_yield_sample.root 
		h2j=${inputdir}/${reco}_${lepton}_${type}_${tagbin}_EqTwoJet_zero_yield_sample.root
		h3j=${inputdir}/${reco}_${lepton}_${type}_${tagbin}_EqThreeJet_zero_yield_sample.root 
		[ ! -e $h2j ] && error "$h2j does not exist!"
		[ ! -e $h3j ] && error "$h3j does not exist!"
		[ ! -e $h ] && hadd $h $h2j $h3j >> $haddDir/tmp.log
		[ -e $haddDir/tmp.log ] && rm $haddDir/tmp.log
	     done 
	fi
	if [ $tagbin == "TagsCombined" ] ; then
	   for type in $types ; do 
		h=$haddDir/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root 
	# 	h=$inputdir/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root 
		# h1t=$inputdir/$reco/$lepton/EqOneTag/$jetbin/${reco}_${lepton}_${type}_EqOneTag_${jetbin}_zero_yield_sample.root
		h1t=$inputdir/${reco}_${lepton}_${type}_EqOneTag_${jetbin}_zero_yield_sample.root
		# h2t=$inputdir/$reco/$lepton/EqTwoTag/$jetbin/${reco}_${lepton}_${type}_EqTwoTag_${jetbin}_zero.root
		h2t=$inputdir/${reco}_${lepton}_${type}_EqTwoTag_${jetbin}_zero_yield_sample.root
		[ ! -e $h1t ] && error "$h1t does not exist!"
		[ ! -e $h2t ] && error "$h2t does not exist!"
		[ ! -e $h ] && hadd $h $h1t $h2t >> $haddDir/tmp.log
		[ -e $haddDir/tmp.log ] && rm $haddDir/tmp.log
	    done 
	fi
	if [ $lepton == "LeptonsCombined" ] ; then
	    for type in $types ; do 
		h=$haddDir/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root 
		# hCC=$inputdir/$reco/CC/$tagbin/$jetbin/${reco}_CC_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		# hMU=$inputdir/$reco/MU/$tagbin/$jetbin/${reco}_MU_${type}_${tagbin}_${jetbin}_zero.root
		hCC=$inputdir/${reco}_CC_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		hMU=$inputdir/${reco}_MU_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		[ ! -e $hCC ] && error "$hCC does not exist!"
		[ ! -e $hMU ] && error "$hMU does not exist!"
		[ ! -e $h ] && hadd $h $hCC $hMU >> $haddDir/tmp.log
		[ -e $haddDir/tmp.log ] && rm $haddDir/tmp.log
	    done 
	fi
	if [ $lepton == "LeptonsCombinedEC" ] ; then
	    for type in $types ; do 
		h=$haddDir/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root 
		hCC=$inputdir/$reco/CC/$tagbin/$jetbin/${reco}_CC_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		hMU=$inputdir/$reco/MU/$tagbin/$jetbin/${reco}_MU_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		hEC=$inputdir/$reco/EC/$tagbin/$jetbin/${reco}_EC_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		[ ! -e $hCC ] && error "$hCC does not exist!"
		[ ! -e $hMU ] && error "$hMU does not exist!"
		[ ! -e $hEC ] && error "$hEC does not exist!"
		if [ $jetbin == "EqTwoJet" ] ; then
		    [ ! -e $h ] && hadd $h $hCC $hMU $hEC >> $haddDir/tmp.log
		else
		    [ ! -e $h ] && hadd $h $hCC $hMU >> $haddDir/tmp.log
		fi
		[ -e $haddDir/tmp.log ] && rm $haddDir/tmp.log
	    done 
	fi
	if [ $reco == "RecoCombined" ] ; then
	    for type in $types ; do
		h=$haddDir/${reco}_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		# h17=$inputdir/p17/$lepton/$tagbin/$jetbin/p17_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		# h20=$inputdir/p20/$lepton/$tagbin/$jetbin/p20_${lepton}_${type}_${tagbin}_${jetbin}_zero.root
		h17=$inputdir/p17_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		h20=$inputdir/p20_${lepton}_${type}_${tagbin}_${jetbin}_zero_yield_sample.root
		[ ! -e $h17 ] && error "$h17 does not exist!"
		[ ! -e $h20 ] && error "$h20 does not exist!"
		[ ! -e $h ] && hadd $h $h17 $h20 >> $haddDir/tmp.log
		[ -e $haddDir/tmp.log ] && rm $haddDir/tmp.log
	    done
	fi

        # Yun-Tse tentative change
	# indir=$haddDir
	indir=$inputdir

	eps=${sig}_vs_all_${reco}_${lepton}_${tagbin}_${jetbin}_${Nbins}bins${logstr}.eps
	cat >> $list <<EOF
Output.File.0: ${eps}

# allowed types: Add, Sum_Errors
Histo.Type.0: Add

Histo.Name: combined vs $sig
Histo.Xmin: ${xmin} 
Histo.Xmax: ${xmax}
${yminstr}
Histo.LogY: ${logy}
#Histo.Ymax: 20
Histo.Nbins: ${Nbins}
Histo.XNdivisions: 505
Histo.YNdivisions: 405
# Histo.XTitle: BNN $sig Discriminant
Histo.XTitle: ME $sig Discriminant
# Histo.YTitle: Event Yield
Histo.YTitle: Yield [Events/0.04]
Histo.DataCut: 1.1
Histo.ShowSignalSeparately: 0
#Histo.Signal.Title: ${sigtxt}
Histo.Signal.Title.0: Signal: $sig
#Histo.Signal.Title.1: Signal: Rv
Histo.Signal.ScaleFactor: ${signal_scale_factor}
Histo.ShowKS: no
# Histo.XtextKS: 0.94
# Histo.YtextKS: 0.73
# Histo.XLabel.Offset: 0.02
# Histo.YLabel.Offset: 0.05
# Histo.XTitle.Offset: 0.9
# Histo.YTitle.Offset: 1.2


###
# additional features/text/legend:
#
# Histo.Preliminary: papercolor
Histo.Preliminary:
Histo.Preliminary.Luminosity: ${lum}

# Histo.AddText.0.X0: 0.31
# Histo.AddText.0.Y0: 0.90
Histo.AddText.0: ${channel}
Histo.AddText.1: ${tagtxt}
Histo.AddText.2: ${jettxt}

# show the legend? yes/no/left/right/separate
Histo.ShowLegend: separate
#Histo.ShowLegend: yes


Sources.Number: 13

# DATA
Sources.0.Title: Data
Sources.0.Color: 1
Sources.0.Files.Number: 1
Sources.0.Files.0.Name: ${indir}/${reco}_${lepton}_DATA_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.0.Files.0.HistoName: ${hstr}

# QCD
Sources.1.Title: Multijets
Sources.1.Color: 28
Sources.1.Files.Number: 1
Sources.1.Files.0.Name: ${indir}/${reco}_${lepton}_QCD_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.1.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.1.Uncertainty: 0.02

# ttbar to lepton+jets
Sources.2.Title: t#bar{t}->l+jets
Sources.2.Color: 2
Sources.2.Files.Number: 1
Sources.2.Files.0.Name: ${indir}/${reco}_${lepton}_ttbar-lepjets_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.2.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.2.Uncertainty: 0.02

# ttbar to dilepton
Sources.3.Title: t#bar{t}->ll
Sources.3.Color: 6
Sources.3.Files.Number: 1
Sources.3.Files.0.Name: ${indir}/${reco}_${lepton}_ttbar-dilepton_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.3.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.3.Uncertainty: 0.02

# diboson
Sources.4.Title: WW+WZ+ZZ
Sources.4.Color: 94
Sources.4.Files.Number: 1
Sources.4.Files.0.Name: ${indir}/${reco}_${lepton}_diboson_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.4.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.4.Uncertainty: 0.02

# Z+lp
Sources.5.Title: Z+light jets
Sources.5.Color: 5
Sources.5.Files.Number: 1
Sources.5.Files.0.Name: ${indir}/${reco}_${lepton}_zlp_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.5.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.5.Uncertainty: 0.02

# Z+cc
Sources.6.Title: Zc#bar{c}+jets
Sources.6.Color: 41
Sources.6.Files.Number: 1
Sources.6.Files.0.Name: ${indir}/${reco}_${lepton}_zcc_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.6.Files.0.HistoName: ${hstr}

# Z+bb
Sources.7.Title: Zb#bar{b}+jets
#Sources.7.Color: 105
Sources.7.Color: 401
Sources.7.Files.Number: 1
Sources.7.Files.0.Name: ${indir}/${reco}_${lepton}_zbb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.7.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.5.Uncertainty: 0.02

# W+lp
Sources.8.Title: W+light jets
Sources.8.Color: 3
Sources.8.Files.Number: 1
Sources.8.Files.0.Name: ${indir}/${reco}_${lepton}_wlp_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.8.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.8.Uncertainty: 0.02

# W+c
#Sources.8.Title: W+c
#Sources.8.Color: 212
#Sources.8.Files.Number: 1
#Sources.8.Files.0.Name: ${indir}/${reco}_${lepton}_wc_${tagbin}_${jetbin}_zero_yield_sample.root
#Sources.8.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
#Sources.8.Uncertainty: 0.02

# W+j
#Sources.9.Title: W+j
#Sources.9.Color: 3
#Sources.9.Files.Number: 1
#Sources.9.Files.0.Name: ${indir}/${reco}_${lepton}_wj_${tagbin}_${jetbin}_zero_yield_sample.root
#Sources.9.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
#Sources.9.Uncertainty: 0.02

# W+cc
Sources.9.Title: Wc#bar{c}+jets
Sources.9.Color: 8
Sources.9.Files.Number: 1
Sources.9.Files.0.Name: ${indir}/${reco}_${lepton}_wcc_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.9.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.9.Uncertainty: 0.02

# W+bb
Sources.10.Title: Wb#bar{b}+jets
#Sources.10.Color: 103
Sources.10.Color: 419
Sources.10.Files.Number: 1
Sources.10.Files.0.Name: ${indir}/${reco}_${lepton}_wbb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.10.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.10.Uncertainty: 0.02

# tbtqb (for Couplings)
# Sources.11.Title: tbtqb
# Sources.11.Color: 7
# Sources.11.Files.Number: 1
# Sources.11.Files.0.Name: ${indir}/${reco}_${lepton}_tbtqb_${tagbin}_${jetbin}_zero_yield_sample.root
# Sources.11.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
# Sources.11.Uncertainty: 0.02
# Sources.11.Files.0.ScaleFactor: ${signal_scale_factor}


# signal 
# Sources.12.Title: Signal: Rv
# Sources.12.Color: 4
# Sources.12.Files.Number: 1
# Sources.12.Files.0.Name: ${indir}/${reco}_${lepton}_tbtqb_0100_${tagbin}_${jetbin}_zero_yield_sample.root
# Sources.12.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
# Sources.12.Uncertainty: 0.02
# Sources.12.Files.0.ScaleFactor: ${signal_scale_factor}
EOF

if [ "${sig}" == "tb" ]; then
cat >> $list <<EOF
#----------------------------------------------
# t-ch background
Sources.11.Title: Signal: tqb
Sources.11.Title: tqb
Sources.11.Color: 4
Sources.11.Files.Number: 1
Sources.11.Files.0.Name: ${indir}/${reco}_${lepton}_tqb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.11.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.11.Uncertainty: 0.02
Sources.11.Files.0.ScaleFactor: ${signal_scale_factor}
# Sources.11.Files.0.ScaleFactor: 0.95
#-------------------------------------------------------------------------

#-------------------------------------------------------------------------
# signal
Sources.12.Title: Signal: tb
Sources.12.Title: tb
Sources.12.Color: 7
Sources.12.Files.Number: 1
Sources.12.Files.0.Name: ${indir}/${reco}_${lepton}_tb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.12.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.12.Uncertainty: 0.02
Sources.12.Files.0.ScaleFactor: ${signal_scale_factor}
# Sources.12.Files.0.ScaleFactor: 1.09
#---------------------------------------------------------------------------


EOF


else 
cat >> $list <<EOF
#-------------------------------------------------------------------------
# s-ch background
Sources.11.Title: Signal: tb
Sources.11.Title: tb
Sources.11.Color: 7
Sources.11.Files.Number: 1
Sources.11.Files.0.Name: ${indir}/${reco}_${lepton}_tb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.11.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.11.Uncertainty: 0.02
# Sources.11.Files.0.ScaleFactor: ${signal_scale_factor}
#---------------------------------------------------------------------------

#----------------------------------------------
# signal
Sources.12.Title: Signal: tqb
Sources.12.Title: tqb
Sources.12.Color: 4
Sources.12.Files.Number: 1
Sources.12.Files.0.Name: ${indir}/${reco}_${lepton}_tqb_${tagbin}_${jetbin}_zero_yield_sample.root
Sources.12.Files.0.HistoName: ${hstr}
# relative uncertainty (same for all bins)
Sources.12.Uncertainty: 0.02
Sources.12.Files.0.ScaleFactor: ${signal_scale_factor}
EOF
fi

cat >> $list <<EOF
# When plotting tb+tqb togheter, we should use kAzure-3 color.
# See ROOT Color Wheel in
# http://root.cern.ch/root/html/MACRO_TAttFill_3_wheel.gif

EOF

	cd $outdir
	root -l -b -q $medir/topd0root_me/macros/combine_channel_plots.C\(\"${list}\",\"${medir}\"\)

        sed -i -e "s/0.95 0.95 0.95/10.00 10.00 10.00/" *.eps
	png=Add_${eps/eps/png}
	which pstoimg >& /dev/null
	if [ $? -eq 0 ]; then
	    #pstoimg --density 100 Add_${eps}
            convert Add_${eps} ${png}
	else
	    convert Add_${eps} ${png}
	fi
	    
	echo "Created $haddDir/$png"
	cd $wd
    done

done
done
done
done

echo
echo DONE
echo

exit 0
