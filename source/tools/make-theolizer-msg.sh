#!/bin/sh

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
