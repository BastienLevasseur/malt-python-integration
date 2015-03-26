#!/bin/bash
######################################################
#            PROJECT  : MATT                         #
#            VERSION  : 0.1.0                        #
#            DATE     : 03/2015                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
COLOR_RED="$(printf "\033[31m")"
COLOR_RESET="$(printf "\033[0m")"

######################################################
function print_error()
{
	echo "${COLOR_RED}$@${COLOR_RESET}" 1>&2
}

######################################################
#check args
if [ -z "$1" ]; then
	print_error "Missing version !"
	echo "Usage : $0 {VERSION}" 1>&2
	exit 1
fi

######################################################
#check call from root directory of project
if [ "$0" != "./dev/update-version.sh" ]; then
	print_error "Caution, you must call this script from the project root directory !"
fi

######################################################
version="$1"
newdate="DATE     : $(date +%m/%Y)"
V="VERSION"
newversion="$V  : $(printf "%-16s" "$version")"

######################################################
find ./ | while read file
do
	#exclude node
	if [ ! -z "$(echo $file | grep node_modules)" ]; then continue; fi
	#exclude bower
	if [ ! -z "$(echo $file | grep bower_components)" ]; then continue; fi
	#exclude git
	if [ ! -z "$(echo $file | grep .git)" ]; then continue; fi
	#exclude git
	if [ ! -z "$(echo $file | grep extern-deps)" ]; then continue; fi
	sed -i -r -e "s#  DATE     : [0-9]{2}/[0-9]{4}#  ${newdate}#g" \
	          -e "s#  $V  : .{16}#  ${newversion}#" \
	          -e "s#  $V  : [0-9a-zA-Z.-]+\$#  $V  : ${version}#" \
			  "${file}"
done

######################################################
#update bower and package
sed -i -r -e "s/\"version\": \"[0-9A-Za-z.-]+\"/\"version\": \"${version}\"/g" src/webview/bower.json
sed -i -r -e "s/\"version\": \"[0-9A-Za-z.-]+\"/\"version\": \"${version}\"/g" src/webview/package.json
sed -i -r -e "s/^version=[0-9A-Za-z.-]+$/version=${version}/g" dev/gen-archive.sh