#!/bin/bash

# Run Txt2Root for both leptons
# Txt2Root creates root files from txt files. But those txt files have to be already parsed.
# I've replaced NAN's by -99999 and -infs by 0, because I could.  
# restoreNans() Txt2Root.C replaces nans and infs by appropriate floats But only in certain variables !!! See Txt2Root.C

input_txt_files_path=/work/budvar-clued0/kuceraja/tt_leptonjets/samples_txt
output_root_files_path=/work/budvar-clued0/kuceraja/tt_leptonjets/samples_root

script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/Txt2Root

for leptons in ele muo; do # muo ele
	echo $leptons;
	mkdir -p ${output_root_files_path}/split_trees_3samples_${leptons}_1119
	for i in ${input_txt_files_path}/split_trees_3samples_${leptons}_1119_2QCD/*; do
		echo $i
		$script_path \
		$i \
		${output_root_files_path}/split_trees_3samples_${leptons}_1119/$(basename ${i/.txt/.root}})
	done
done

# pro generovani skriptu do batche.

#cat > bla <<EOF
#!/bin/sh
#source ascii_setup
#program $param
#EOF

#echo "program $param" >> bla
