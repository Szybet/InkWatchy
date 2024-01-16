xxd_img_legacy() {

    echo -e -n "const unsigned char  ${2} [] = {\n" >>images.h

    convert $1 -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- |  xxd -i  >> images.h
    echo -e "};\nconst unsigned int ${2}d =(sizeof($2)/sizeof(${2}[0])) ;\n"   >> images.h

    echo -n -e "const ImageDef ${2}Pack = {${2}, ${3}_WIDTH, ${3}_HEIGHT};\n" >> images.h
    }


