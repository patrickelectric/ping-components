#!/bin/bash

# Variables
bold=$(tput bold)
normal=$(tput sgr0)
scriptpath="$( cd "$(dirname "$0")" ; pwd -P )"
projectpath=${scriptpath}/..

# Functions
echob() {
    echo "${bold}${1}${normal}"
}

echob "Running style script:"
${scriptpath}/clang-format.sh
if ! git diff --quiet --ignore-submodules HEAD 2>/dev/null; then
    echob "Style error ! Run clang-format in tools. ($ ./tools/clang-format.sh)"
    git diff | cat
    exit 1
fi

echob "Running doxygen script:"
${scriptpath}/testdoxygen.sh || exit 1

echob "Compile code in test mode:"
build_test="$projectpath/build_test"
rm -rf $build_test
mkdir -p ${build_test}
qmake -o ${build_test} -r -Wall -Wlogic -Wparser CONFIG+=test ${projectpath}
make -C ${build_test}
xvfb-run --server-args="-screen 0 1024x768x24" $build_test/pingviewer || exit 1

echob "Do runtime test:"
${scriptpath}/compile.sh --autokill --no-deploy --debug || exit 1
export DISPLAY=:99.0
build_folder="$projectpath/build"
xvfb-run --server-args="-screen 0 1024x768x24" ${build_folder}/pingviewer || exit 1
echob "Checking for warning messages.."
last_log=$(ls -dt ~/Documents/PingViewer/Gui_Log/* | head -n1)
if grep "\[Warning\]" $last_log; then
    echob "Please fix the warning messages!"
    exit 1
fi

echob "Test CMake build:"
build_folder="$projectpath/build_cmake"
mkdir ${build_folder}
cd $build_folder
cmake ..
make
