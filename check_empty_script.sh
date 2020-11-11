# Esta funcion auxiliar permitira cortar la ejecucion de un comando pasado una cantidad de segundos fijo en caso no haya acabado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

for file in ./datasets/3D_Pottery/train_unnormalized/*.obj; do
	./check_empty $file
    if [ $? -eq 0 ]
    then
        echo "Ok"
    else
        echo "rm $file"
		rm $file
    fi
    #doalarm 15 ./make_holes $infile $outfile
done
