# Esta funcion auxiliar permitira cortar la ejecucion de un comando pasado una cantidad de segundos fijo en caso no haya acabado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

rm get_mesh_from_incomplete
g++ get_mesh_from_incomplete.cpp -std=c++14 -o get_mesh_from_incomplete

for file in ./datasets/3D_Pottery/Incomplete_watertight_1900_hung/*/*.obj; do
	infile=$file
	outfile=${file//Incomplete_watertight_1900_hung/Mapped}
	#break
	#doalarm 15 ./make_holes $infile $outfile
	doalarm 15 ./get_mesh_from_incomplete $infile $outfile
done
