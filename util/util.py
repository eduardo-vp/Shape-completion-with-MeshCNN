from __future__ import print_function
import torch
import numpy as np
import os


def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)

MESH_EXTENSIONS = [
    '.obj',
]

def is_mesh_file(filename):
    return any(filename.endswith(extension) for extension in MESH_EXTENSIONS)

def pad(input_arr, target_length, val=0, dim=1):
    # pad llena en input_arr en la dimension 'dim', (target_lenght - len(input_arr[dim])) valores 'val'
    # 
    # Por ejemplo : 
    # 
    # input_arr = [1 2 3 4 5], target_length = 7, val = -1, dim = 0 
    # devuelve [1 2 3 4 5 -1 -1]
    # 
    # input_arr = [ [1,2,3] [4,5,6] [7,8,9] ], target_length = 5, val = -2, dim = 0 
    # devuelve [ [1,2,3] [4,5,6] [7,8,9] [-2 -2 -2] [-2 -2 -2] ]
    # 
    # input_arr = [ [1,2,3] [4,5,6] [7,8,9] ], target_lenght = 5, val = -3, dim = 1
    # devuelve [ [1,2,3,-3,-3] [4,5,6,-3,-3] [7,8,9,-3,-3] ]
    # 
    shp = input_arr.shape
    npad = [(0, 0) for _ in range(len(shp))]
    assert target_length >= shp[dim]
    npad[dim] = (0, target_length - shp[dim])
    assert len(shp) <= 2
    return np.pad(input_arr, pad_width=npad, mode='constant', constant_values=val)

def seg_accuracy(predicted, ssegs, meshes):
    correct = 0
    ssegs = ssegs.squeeze(-1)
    correct_mat = ssegs.gather(2, predicted.cpu().unsqueeze(dim=2))
    for mesh_id, mesh in enumerate(meshes):
        correct_vec = correct_mat[mesh_id, :mesh.edges_count, 0]
        edge_areas = torch.from_numpy(mesh.get_edge_areas())
        correct += (correct_vec.float() * edge_areas).sum()
    return correct

def print_network(net):
    # Imprimir el total de parametros utilizados en la red
    print('---------- Network initialized -------------')
    num_params = 0
    for param in net.parameters():
        num_params += param.numel()
    print('[Network] Total number of parameters : %.3f M' % (num_params / 1e6))
    print('-----------------------------------------------')

def get_heatmap_color(value, minimum=0, maximum=1):
    minimum, maximum = float(minimum), float(maximum)
    ratio = 2 * (value-minimum) / (maximum - minimum)
    b = int(max(0, 255*(1 - ratio)))
    r = int(max(0, 255*(ratio - 1)))
    g = 255 - b - r
    return r, g, b


def normalize_np_array(np_array):
    min_value = np.min(np_array)
    max_value = np.max(np_array)
    return (np_array - min_value) / (max_value - min_value)


def calculate_entropy(np_array):
    entropy = 0
    np_array /= np.sum(np_array)
    for a in np_array:
        if a != 0:
            entropy -= a * np.log(a)
    entropy /= np.log(np_array.shape[0])
    return entropy
