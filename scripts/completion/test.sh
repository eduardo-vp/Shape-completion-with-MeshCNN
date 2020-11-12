#!/usr/bin/env bash

## run the test and export collapses
python test.py \
--dataroot datasets/3D_Pottery \
--name 3D_Pottery \
--arch meshunet \
--dataset_mode completion \
--ncf 32 64 128 256 \
--ninput_edges 2900 \
--pool_res 2300 2000 1700 \
--resblocks 3 \
--lr 0.0001 \
--batch_size 4 \
--num_aug 1 \
--export_folder meshes \
--niter 20 \
--niter_decay 40 \
--gpu -1