






xxd_wrapper(){

    input_data=$(cat -)
    echo -e -n "const unsigned char  ${1} [] = {\n" 

    echo "$input_data"
    echo -e "};\nconst size_t ${1}d =(sizeof(${1})/sizeof(${1}[0])) ;\n"



}


