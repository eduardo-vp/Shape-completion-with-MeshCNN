#!/usr/bin/env bash

## run the training
python train.py \
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
--slide_verts 0.2 \
--niter 20 \
--niter_decay 40 \
--gpu -1
#
# python train.py --dataroot datasets/coseg_vases --name coseg_vases --arch meshunet --dataset_mode
# segmentation --ncf 32 64 128 256 --ninput_edges 1500 --pool_res 1050 600 300 --resblocks 3 --lr 0.001 --batch_size 12 --num_aug 20
