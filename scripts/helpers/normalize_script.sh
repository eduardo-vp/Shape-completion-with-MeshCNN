# Archivo que permitira transformar todos los archivos
# del dataset de 3D Pottery a watertight manifold

# Esta funcion auxiliar permitira cortar la ejecucion de manifold
# con una cierta pieza en caso haya demorado 1 minuto y no haya terminado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

for file in ./datasets/3D_Pottery/Complete/**/*.obj; do
	infile=$file
	outfile=${file//Complete/Complete_normalized}
	echo "$infile"
	echo "$outfile"
	doalarm 15 ./normalize $infile $outfile
done
