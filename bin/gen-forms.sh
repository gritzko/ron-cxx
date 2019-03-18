#!/bin/bash

filename="ron/forms.txt"

INDICES=""
LONGS=""
UUIDS=""
I=0
NL="\\n"
TAB="\\t"

while read -r LINE ; do

    read -ra FORM <<< "$LINE"

    NAME=${FORM[0]}
    TYPE=${FORM[1]}
    NAMEUC=${NAME^^}

    IDX_NAME="$NAMEUC""_""$TYPE""_FORM"
    
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

    UUIDS+="const Uuid $NAMEUC""_FORM_UUID{FORMS[$IDX_NAME],0UL}; // NOLINT$NL"

    I=$((I+1))

done < $filename

echo "enum form_idx_t : uint8_t {"
echo -e $INDICES
echo -e "\tRESERVED_ANY_FORM = 200\n"
echo "};"

echo "const uint64_t FORMS[] {"
echo -e $LONGS
echo "};"

echo -e $UUIDS
