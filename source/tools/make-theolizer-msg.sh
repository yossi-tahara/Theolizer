#!/bin/sh
##############################################################################
#   © 2016 Theoride Technology (http://theolizer.com/) All Rights Reserved.
#   "Theolizer" is a registered trademark of Theoride Technology.
#
#   "Theolizer" License
#       In the case where you are in possession of a valid “Theolizer” License,
#       you may use this file in accordance with the terms and conditions of 
#       the use license determined by Theoride Technology.
#
#   General Public License Version 3 ("GPLv3")
#       You may use this file in accordance with the terms and conditions of 
#       GPLv3 published by Free Software Foundation.
#       Please confirm the contents of GPLv3 at https://www.gnu.org/licenses/gpl.txt .
#       A copy of GPLv3 is also saved in a LICENSE.TXT file.
#
#   商用ライセンス
#       あなたが有効なTheolizer商用ライセンスを保持している場合、
#       セオライド テクノロジーの定める使用許諾書の条件に従って、
#       このファイルを取り扱うことができます。
#
#   General Public License Version 3(以下GPLv3)
#       Free Software Foundationが公表するGPLv3の使用条件に従って、
#       あなたはこのファイルを取り扱うことができます。
#       GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
#       またGPLv3のコピーをLICENSE.TXTファイルにおいてます。
##############################################################################

# 旧ハッシュ記録を削除
sed -i -e '/------ auto generated message by Theolizer/,/------ end of message/d' $1

# ルートにbinary_dir.shがあれば、ハッシュ記録を生成する

cat ./binary_dir.sh

if [ -e ./binary_dir.sh ]; then
    echo "------ auto generated message by Theolizer" >> $1

	# PASS記録
	cmake -DBINARY_DIR=`./binary_dir.sh` -P source/tools/check_pass.cmake
	if [ "$?" -eq 0 ]; then
		echo "auto test(one-set) : PASS" >> $1
	fi

	# ハッシュ値生成
	cmake -DBINARY_DIR=`./binary_dir.sh` -P source/tools/make_version.cmake

	# ハッシュ値記録
    echo -n "TheolizerDriver  : " >> $1
    if [ -e source/driver/version.h ]; then
        grep -h "SourcesHash\[\]" source/driver/version.h                     | sed -e 's/^.*=\"//g' | sed -e 's/\";//g' >> $1
    else
        echo "no version.h" >> $1
    fi

    echo -n "TheolizerLibrary : " >> $1
    if [ -e source/library/theolizer/internal/version.h ]; then
        grep -h "SourcesHash\[\]" source/library/theolizer/internal/version.h | sed -e 's/^.*=\"//g' | sed -e 's/\";//g' >> $1
        echo -n "Library's Header : " >> $1
        grep -h "LibraryHash\[\]" source/library/theolizer/internal/version.h | sed -e 's/^.*=\"//g' | sed -e 's/\";//g' >> $1
    else
        echo "no version.h" >> $1
    fi
    echo "------ end of message" >> $1

fi
