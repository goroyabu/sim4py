#!/bin/sh

list="Int String Double DoubleUnit Vector3 Vector3Unit Bool"

for i in $list
do
    printf "%-25s" "ANLparams${i}*"
    printf "%-25s" "mANLparams${i};"
    echo
done

for i in $list
do
    printf "%s" "template<> "
    printf "%-25s" "ANLparams${i}*"
    printf "%s" "get_params_list() "
    printf "%-25s" "{ return mANLparams${i}; }"
    echo
done

for i in $list
do    
    printf "%-25s" "mANLparams${i}"
    printf "%s" " = new "
    printf "%-25s" "ANLparams${i};"
    echo
done

# for i in $list
# do
#     printf 
