#!/usr/bin/env bash
# Version 2021-08-11

#ensure there is an argument
if [[ $# -ne 1 ]]
then
  echo "Usage: do_tests.bash your_executable"
  exit 1
fi

#ensure the provided argument is an executable
if [[ ! -x $1 ]]
then
  echo "Usage: do_tests.bash your_executable"
  echo "  I can't find your_executable. Perhaps it is not where you think it is"
  exit 1
fi
EXECUTABLE_NAME="$1"

#ensure this script is being run from the correct location
if [[ $(basename "$PWD") != "run_script_from_in_here" ]]
then
  echo "Your current directory must be \"run_script_from_in_here\" when running this script. Try this:"
  printf "\n  cd run-script_from_in_here\n  ../do_tests.bash your_executable\n\n"
  exit 1
fi

if [ ! -d ../origfiles ] 
then
  echo "I cannot find origfiles. Have you been moving stuff around?"
  exit 1
fi

cat ../list_of_tests.dat | while read testname old new filename
do
  cp ../origfiles/$filename $testname.txt
  chmod +w $testname.txt
  echo "================"
  echo $testname
  echo "================"
  timeout 2s $EXECUTABLE_NAME $old $new $testname.txt 
  if [[ $? -eq 124 ]] # exit status returned by timeout when it kills a process
  then 
    echo -n "$testname RUNAWAY PROCESS. TERMINATED. "
  fi
  diff $testname.txt ../expected_results/$testname.txt >/dev/null
  if [ $? -eq 0 ]
  then
    echo "$testname success!" 
  else
    echo "$testname failure <########################################"
    #only show contents of short files
    echo "Search for: $old, replace with: $new"
    if [ `wc -c ../origfiles/$filename|awk '{print $1}'` -lt 50 ]
    then
      echo -n "Original file:--->"
      cat ../origfiles/$filename
      echo "<-"
      echo -n "Expected result:->"
      cat ../expected_results/$testname.txt
      echo "<-"
      echo -n "Test result:----->"
      cat $testname.txt
      echo "<-"
    else
      echo "Original file: ../origfiles/$filename"
      echo "Expected results: ../expected_results/$testname.txt"
      echo "Test results: $testname.txt"
    fi
  fi
  echo
  echo
done
