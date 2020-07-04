#!/bin/sh


function def ()
{
    name=$1
    symbol=$2
    category=$3
    
    #printf "%-20s" "static auto ${symbol}"
    #printf "%s"    " = new UnitDefinition( "
    
    # printf "%s"       "static const UnitDefinition "
    # printf "%-10s"    "${symbol}"
    # printf "%-25s"    "( \"${name}\","
    # printf "%-10s"    "\"${symbol}\","
    # printf "%-20s"    "\"${category}\","
    # printf "%-30s"    "CLHEP::${name} );"
    # echo
    
    printf "%-20s" "sub.attr(\"${symbol}\")"
    printf "%s"    " = pybind11::cast( "
    printf "%-10s" "${symbol} );"
    echo
    #echo
}

function unit ()
{
    def "parsec" "pc" "Length" 
    def "kilometer" "km" "Length" 
    def "meter" "m" "Length"
    def "centimeter" "cm" "Length" 
    def "millimeter" "mm" "Length"
    def "micrometer" "um" "Length"
    def "nanometer" "nm" "Length" 
    def "angstrom" "Ang" "Length"     
    def "fermi" "fm" "Length"
    
    def "radian" "rad" "Angle"
    def "milliradian" "mrad" "Angle" 
    def "degree" "deg" "Angle"

    def "electronvolt" "eV" "Energy"
    def "kiloelectronvolt" "keV" "Energy"
    def "megaelectronvolt" "MeV" "Energy"
    def "gigaelectronvolt" "GeV" "Energy"
    def "teraelectronvolt" "TeV" "Energy"
    def "petaelectronvolt" "PeV" "Energy"
    def "joule" "J" "Energy"
    
    def "milligram" "mg" "Mass"
    def "gram" "g"  "Mass"
    def "kilogram" "kg" "Mass"
    
    def "pascal" "Pa" "Pressure"
    def "bar" "bar" "Pressure"
    def "atmosphere" "atm" "Pressure"
    
    def "becquerel" "Bq" "Activity"
    def "curie" "Ci" "Activity"
}

unit
