
# Dado que existe la carpeta de archivos watertight de nombre 3DPotteryDatasetWatertight
# cuys archivos tienen la forma *watertight.obj
# se corre el script y crea una carpeta de nombre 3DPotteryFinal
# con todas las mallas con 2048 caras

# cuidadito
# este archivo sobreescribeeeeeeeee a diferencia de los otros

# Esta funcion auxiliar permitira cortar la ejecucion del comando
# con una cierta pieza en caso haya demorado 1 minuto y no haya terminado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar


for file in ./datasets/3D_Pottery/Complete_normalized_watertight_2048/**/*.obj; do
	infile=$file
	outfile=$file
	echo "Executing command"
	echo "$infile"
	echo "$outfile"
	doalarm 60 meshlabserver -i $infile -o $outfile -s filter_script_2048.mlx
done

for file in ./datasets/3D_Pottery/Incomplete_watertight_1900/**/*.obj; do
	infile=$file
	outfile=$file
	echo "Executing command"
	echo "$infile"
	echo "$outfile"
	doalarm 60 meshlabserver -i $infile -o $outfile -s filter_script_1900.mlx
done
