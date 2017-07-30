#!/bin/sh

# Setting up:
#   in QtCreator, clone a new 'Run' command and add the following into "Command line arguments" field:
#   "> output.log && (%{sourceDir}/UnitTests/scripts/runCoverage.sh ./)"
#
# For details, see:
#   https://asmaloney.com/2017/01/code/code-coverage-of-unit-tests-using-qt-5-on-macos/
# 
# ${1} is the directory containing the .gcno files (%{buildDir} in Qt Creator)
 
LCOV=lcov
GENHTML=genhtml
 
SRC_DIR="${1}"
HTML_RESULTS="${1}/html"
 
mkdir -p ${HTML_RESULTS}
 
# generate our initial info
"${LCOV}" -d "${SRC_DIR}" -c -o "${SRC_DIR}/coverage.info"
 
# remove some paths
"${LCOV}" -r "${SRC_DIR}/coverage.info" "*Qt*.framework*" "*Xcode.app*" "*.moc" "*moc_*.cpp" "/usr/include/*" "*/build-Mentol-*" "QtCore/*" "QtGui/*" "QtQml/*" "QtTest/*" "QtWidgets/*" "*/UnitTests/*" -o "${SRC_DIR}/coverage-filtered.info"
 
# generate our HTML
"${GENHTML}" -o "${HTML_RESULTS}" "${SRC_DIR}/coverage-filtered.info"
 
# reset our counts
"${LCOV}" -d "${SRC_DIR}" -z
 
# open in browser and bring to front
python -mwebbrowser "${HTML_RESULTS}/index.html"
