
for dir in `find articles/images_src/* -type d`; do
    dist=${dir//images_src\//images\//}
    rm -rf $dist
    mkdir $dist
    for file in `find ${dir} -type f`; do
        echo $file
        dist_file=${file//images_src\//images\//}

        convert $file -type grayscale $dist_file 
    done
done

