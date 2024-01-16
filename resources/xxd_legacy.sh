






xxd_wrapper(){

    input_data=$(cat -)
    echo -e -n "const unsigned char  ${1} [] = {\n" 

    echo "$input_data"
    echo -e "};\nconst unsigned int ${1}d =(sizeof($1)/sizeof(${1}[0])) ;\n"

    echo -n -e "const ImageDef ${1}Pack = {${1}, ${2}_WIDTH, ${2}_HEIGHT};\n" 


}


