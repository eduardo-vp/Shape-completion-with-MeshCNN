# Archivo que permitira transformar todos los archivos
# del dataset de 3D Pottery a watertight manifold

# Esta funcion auxiliar permitira cortar la ejecucion de manifold
# con una cierta pieza en caso haya demorado 1 minuto y no haya terminado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

for file in ./datasets/3D_Pottery/Complete_normalized/**/*.obj; do
	infile=$file
	outfile=${file//Complete_normalized/Incomplete}
	echo "tanda"
	echo "$infile"
	for i in 1 2 3 4 5
	do
		outfile2=${outfile//.obj/_$i.obj}
		echo "$outfile2"
		doalarm 15 ./make_holes $infile $outfile2
	done
	#doalarm 15 ./make_holes $infile $outfile
done
