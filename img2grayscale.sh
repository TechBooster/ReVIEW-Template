
for dir in `find articles/images/src/* -type d`; do
    dist=${dir//src\//}
    rm -rf $dist
    mkdir $dist
    for file in `find ${dir} -type f`; do
        echo $file
        dist_file=${file//src\//}

        convert $file -type grayscale $dist_file 
    done
done

