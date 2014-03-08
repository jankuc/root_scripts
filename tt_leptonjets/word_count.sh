#!/bin/bash
#
sourceDirectory=$1
for filename in `find $sourceDirectory -name "*IINC*" -print`
do
#     without_backslash=${filename//\//-}
#
newFileName=${without_backslash//-home-www-nnsu-nnsu_user_data_area-SEP-fnalnnsu-single_top_02_13-no-gain-/}

     wc $filename
#     cp $filename ./all/$newFileName
done