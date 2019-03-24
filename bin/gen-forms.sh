#!/bin/bash

filename="ron/forms.txt"

INDICES=""
LONGS=""
UUIDS=""
PAIRS=""
I=0
NL="\\n"
TAB="\\t"

while read -r LINE ; do

    read -ra FORM <<< "$LINE"

    NAME=${FORM[0]}
    TYPE=${FORM[1]}
    NAMEUC=${NAME^^}

    IDX_NAME="$NAMEUC""_""$TYPE""_FORM"
    UUID="$NAMEUC""_FORM_UUID"
    
    INDICES+="$NL"
    INDICES+="$TAB"
    INDICES+=$IDX_NAME
    INDICES+=" = "
    INDICES+=$I
    INDICES+=","

    U2I=`echo $NAME | bin/uuid2int`
    
    read -ra CONVERT <<< $U2I
    INT=${CONVERT[1]}
    LONGS+="$TAB"
    LONGS+=$INT
    LONGS+="UL, // $NAME"
    LONGS+="$NL"

    UUIDS+="const Uuid $UUID{FORMS[$IDX_NAME],0UL}; // NOLINT$NL"

    PAIRS+="$TAB""{ $UUID, $IDX_NAME },"
    PAIRS+="$NL"

    I=$((I+1))

done < $filename

echo "const uint64_t FORMS[] {"
echo -e $LONGS
echo "};"

echo "enum FORM : uint8_t {"
echo -e $INDICES
echo -e "\tRESERVED_ANY_FORM = 200,"
echo -e "\tERROR_NO_FORM = 255"
echo "};"
echo
echo "const std::unordered_map<Uuid,FORM> UUID2FORM = {"
echo -e $PAIRS
echo -e "{FATAL, ERROR_NO_FORM}"
echo "};"

echo -e $UUIDS
