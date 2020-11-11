# Archivo que permite mover todos los archivos watertight que se encuentran
# en 3DPotteryDataset a una nueva carpeta 3DPotteryDatasetWatertight

shopt -s globstar

for file in ./**/*watertight.obj; do
	infile=$file
	dest=${file//3DPotteryDataset/3DPotteryDatasetWatertight}
	dest=${dest%/*}
	echo $infile
	echo $dest
	mkdir --parents $dest
	mv $infile $dest
done

