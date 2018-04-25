#!/bin/bash

# echo "==== runTestCase: $program ; IN: $testin OUT: $testresout ERR: $testreserr"
$program < $testin 1>$testresout 2>$testreserr
