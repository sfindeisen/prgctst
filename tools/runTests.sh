#!/bin/bash
#
# How to run - command line parameters:
#
# EXECUTABLE TESTDIR [MEMLIMIT]

MYDIR=`dirname $0`
timex=`which time`

if [ ! -x "$timex" ] ; then
    echo "time command not found!" ;
    exit 0 ;
fi ;

preview() {
    bigtext=$1
    header=`echo -e "$bigtext" | head -n 6` ;
    pview=${header:0:400} ;
    echo "$pview [...]" ;
}

runTest () {
    program=$1
    testin=$2
    # memlimit=$3
    testout=`echo $testin | sed 's/\.in$/\.out/'`

    if [ ! -f $testin ] ; then
        echo "Error: reference IN file ($testin) does not exist! Unable to run the test case."
        exit 0 ;
    fi ;
    if [ ! -f $testout ] ; then
        echo "Error: reference OUT file ($testout) does not exist! Unable to run the test case."
        exit 0 ;
    fi ;

    testresout=`mktemp`
    testreserr=`mktemp`
    echo "==== Test: IN: $testin OUT: $testresout ERR: $testreserr"

    export program testin testresout testreserr
    extime=`$timex -f 'User: %U Sys: %S Real: %E RSS max: %M kB K: %K kB D: %D kB' /bin/bash $MYDIR/runTestCase.sh 2>&1`

    if [ -f "$testresout" ] ; then
        dcmp=`cmp $testout $testresout`

        if [ -z "$dcmp" ] ; then
            echo "OK: $extime" ;
            if [ -s "$testreserr" ] ; then
                echo "Warning: STDERR present: $testreserr" ;
            fi ;
        else
            ddiff=`diff -NaurEb $testout $testresout`
            if [ -z "$ddiff" ] ; then
                echo "OK: $extime" ;
                echo "Warning: $dcmp" ;
            else
                echo "Error: bad result!" ;
                preview "$ddiff" ;
            fi ;
        fi ;
    else
        echo "Error: no output file!" ;
    fi ;
} ;

################################
# The program
################################

if [ 2 -gt $# ] ; then
  echo "Usage: $0 EXECUTABLE TESTDIR [MEMLIMIT]" ;
  exit 0 ;
fi ;

EXECUTABLE=$1
TESTDIR=$2
# MEMLIMIT=$3

if [ ! \( -f "$EXECUTABLE" \) ] ; then
    echo "Not found: $EXECUTABLE" ;
    exit 0 ;
fi ;

if [ ! \( -x "$EXECUTABLE" \) ] ; then
    echo "No execute permission: $EXECUTABLE" ;
    exit 0 ;
fi ;

EXECNEW=`mktemp`
cp -v $EXECUTABLE $EXECNEW
chmod 700 $EXECNEW
EXECUTABLE=$EXECNEW
echo "Executable copy: $EXECUTABLE"

if [ -d "$TESTDIR" ] ; then
    TESTDIRABS=`realpath $TESTDIR`
    nesting=`echo $TESTDIRABS | grep -o '/' | wc -l`
    nesting=$((2+$nesting))
    alltests=`ls -1 $TESTDIRABS/*.in | sort -n -t '/' -k $nesting.4`

    for testin in $alltests ; do
        runTest $EXECUTABLE $testin ;
    done ;
elif [ -f $TESTDIR ] ; then
    runTest $EXECUTABLE $TESTDIR ;
else
    echo "Cannot read: $TESTDIR" ;
fi
