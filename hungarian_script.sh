# Esta funcion auxiliar permitira cortar la ejecucion de un comando pasado una cantidad de segundos fijo en caso no haya acabado
doalarm () { perl -e 'alarm shift; exec @ARGV' "$@"; }

shopt -s globstar

rm hungarian
g++ hungarian.cpp -std=c++14 -o hungarian

for file in ./datasets/3D_Pottery/Complete_normalized_watertight_2048/*/*.obj; do
	comp=$file
	incomp=${file//Complete_normalized_watertight_2048/Incomplete_watertight_1900}
	echo "tanda"
	echo "$comp"
	for i in {1..5}
	do
		incompp=${incomp//.obj/_$i.obj}
		for j in {0..5}
		do
			incomppp=${incompp//.obj/_$j.obj}
			incomppp_hung=${incomppp//Incomplete_watertight_1900//Incomplete_watertight_1900_hung}
			#doalarm 15 ./make_holes $infile $outfile2
			echo "$incompp"
			echo "$incomppp_hung"
			doalarm 10 ./hungarian $comp $incomppp $incomppp_hung
		done
	done
	#doalarm 15 ./make_holes $infile $outfile
done
