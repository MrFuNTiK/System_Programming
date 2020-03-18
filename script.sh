#!/bin/bash

echo ""
echo ""
echo "Developer: Kirill Voevodin, TUSUR, Gr. 736-1"
echo "Script runs in infinite cycle"
echo "This script asks name of package and searchs it in computer"
echo "If package is not installed script will search it in repos"
echo "If package was found in repos script asks user to install this package"
echo ""
echo "-----------------------------"
echo ""

PACK_NOT_FOUND=""
contin="y"

while  : #[ "$contin"="Y" ] || [ "$contin"="y" ]
do
        #echo "$contin"
        echo "Enter package name"
        read package
        echo ""
        if
                found_package=$(yum list installed | grep $package| tr : "\n" | head -1)#formatin list of found and get first pack in list
        then
                yum info $found_package
                echo ""
        else
                echo "Package is not installed." 1>&2
                echo "Find in repo..."
                echo ""
                result=$(yum search $package)
                #echo ""
                #echo "$result"
                if [ "$result" != "$PACK_NOT_FOUND" ]
                then
                        echo "Package found!"
                        echo "Do you want to install it? ([y]es \ [n]o)"
                        while :
                        do
                                read choice
                                case "$choice" in
                                        "Y" | "y" ) yum install -y $package; break;;
                                        "N" | "n" ) echo ""; echo "I will not install this package"; break;;
                                        * ) echo "Error input! Try again" 1>&2;;
                                esac
                        done
                fi
        fi
        while :
        do
                echo "Want to continue? ([y]es \ [n]o)"
                read contin
                case "$contin" in
                        "Y" | "y" ) echo ""; echo "Another loop"; echo""; break;;
                        "N" | "n" ) echo ""; echo "Good bye!"; exit 0;;
                        * ) echo "Error input! Try again" 1>&2;;
                esac
                #echo "$contin"
        done
done
