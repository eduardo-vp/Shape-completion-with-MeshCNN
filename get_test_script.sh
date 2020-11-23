# Esta funcion auxiliar permitira cortar la ejecucion de un comando pasado una cantidad de segundos fijo en caso no haya acabado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

n=0

for file in ./datasets/3D_Pottery/train/*.obj; do
    x=$((RANDOM%18))
    if [ $x -eq 0 ]
    then
        out=${file//train/test}
        out=${out:0:27}
        out="${out}Piece$n.obj"
        echo $file
        echo $out
        cp $file $out
        n=$((n+1))
    fi
done

