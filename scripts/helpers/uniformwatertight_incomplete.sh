# Archivo que permitira transformar todos los archivos
# del dataset de 3D Pottery a watertight manifold

# Esta funcion auxiliar permitira cortar la ejecucion de manifold
# con una cierta pieza en caso haya demorado 1 minuto y no haya terminado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

for file in ../../Shape-completion-with-MeshCNN/datasets/3D_Pottery/Incomplete/**/*.obj; do
	infile=$file
	outfile=${file//Incomplete/Incomplete_watertight}
	echo "$infile"
	echo "$outfile"
	doalarm 15 ./manifold $infile $outfile 600
	#doalarm 15 ./simplify -i $infile -o $outfile -m -f 2048
done
