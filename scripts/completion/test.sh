#!/usr/bin/env bash

## run the test and export collapses
python test.py \
--dataroot datasets/3D_Pottery \
--name 3D_Pottery \
--arch meshunet \
--dataset_mode completion \
--ncf 16 32 64 128 256 \
--ninput_edges 2900 \
--pool_res 2400 2200 2000 1800 \
--resblocks 3 \
--lr 0.001 \
--batch_size 4 \
--num_aug 1 \
--export_folder meshes \
--niter 50 \
--niter_decay 50 \
--gpu -1
