#!/bin/bash

NAME=$1
NAMELO=`echo $NAME | tr '[:upper:]' '[:lower:]'`

ENUM="enum $NAME : uint8_t {"
CONST=""
ARRAY="static const Uuid ${NAME}_IDS [] = {";

E2U="Uuid ${NAMELO}2uuid ($NAME e) { return ${NAME}_IDS[e]; }\n"
U2E="$NAME uuid2${NAMELO} (const Uuid& u) {\n
\tswitch (uint8_t(u.value()._64>>52)) {\n"

COUNT="${NAME}_COUNT"

while read UUID; do
    
    UUIDHI=`echo $UUID | tr '[:lower:]' '[:upper:]'`
    INT=`./uuid2int $UUID | awk '{print $2}'`
    NM=${UUIDHI}"_"${NAME}
    ID=${NM}"_ID"
    IDNAME=${NM}"_NAME"
    CONST=$CONST"static const uint64_t $IDNAME{${INT}UL};\n"
    CONST=$CONST"static const Uuid $ID{${IDNAME},0};\n"
    ENUM=$ENUM"\n\t${NM},"
    ARRAY=${ARRAY}"\n\t$ID,"
    U2E=$U2E"\t\tcase uint8_t(${IDNAME}>>52): \treturn u==$ID ? "${UUIDHI}_${NAME}" : $COUNT;\n"

done

ENUM=$ENUM"\n\t$COUNT\n};\n"
U2E=$U2E"\t\tdefault: return $COUNT;\n\t}\n}"
ARRAY=${ARRAY}"\n\tUuid::FATAL\n};"

echo -e $ENUM

echo -e $CONST

echo -e $ARRAY
echo
echo -e $E2U
echo -e $U2E
