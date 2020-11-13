#!/usr/bin/env bash

## run the training
python train.py \
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
--slide_verts 0.2 \
--niter 50 \
--niter_decay 50 \
--gpu 0
#
# python train.py --dataroot datasets/coseg_vases --name coseg_vases --arch meshunet --dataset_mode
# segmentation --ncf 32 64 128 256 --ninput_edges 1500 --pool_res 1050 600 300 --resblocks 3 --lr 0.001 --batch_size 12 --num_aug 20
