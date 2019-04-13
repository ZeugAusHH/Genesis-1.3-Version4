#!/bin/sh
# C. Lechner (DESY), 2019-04-13
#
# Summarize current status of git repository and key information
# about build process and insert them into .cpp file to be included
# into the binary.

F=`mktemp`

echo -n "built by " >> $F
echo -n `whoami` >> $F
echo -n " " >> $F
echo -n `date "+%Y%m%dT%H%M"` >> $F
echo -n " from git commit ID " >> $F

# handle the unlikely case that not git is installed
if ! [ -x "$(command -v git)" ]; then
	echo -n "unknown (git not installed?)" >> $F
else
	# git is installed ...

	git log --format="%H" -n 1 | tr -d '\n' >> $F

	# cl 20190223: "git status -s -u no" does not show the modified files?
	# Lines with untracked files begin with '??', ignore those.
	if [[ ! -z $(git status -s | grep --invert "^??") ]]; then
		echo -n " (build includes uncommitted changes)" >> $F
	fi
fi
# no final newline


TXT=`cat $F`
rm -f $F

cat build_info.cpp.templ | sed "s/\$BI/$TXT/" > build_info.cpp

