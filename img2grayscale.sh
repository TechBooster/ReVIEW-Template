rm -rf articles/images/dist/*
for dir in `find articles/images/src/* -type d`; do
    for file in `find ${dir} -type f`; do
        dist=${dir//src/dist}
        mkdir $dist
        convert $file -type grayscale $dist/%d.png 
    done
done

