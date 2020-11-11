from itertools import filterfalse
import os
import torch
from data.base_dataset import BaseDataset
from util.util import is_mesh_file, pad
import numpy as np
from models.layers.mesh import Mesh

class CompletionData(BaseDataset):

    def __init__(self, opt):
        BaseDataset.__init__(self, opt)
        # se guarda en opt las configuraciones de la red
        self.opt = opt
        # se guarda el device (gpu o cpu)
        self.device = torch.device('cuda:{}'.format(opt.gpu_ids[0])) if opt.gpu_ids else torch.device('cpu')
        # se guarda el root
        self.root = opt.dataroot
        # se guarda el dir 
        self.dir = os.path.join(opt.dataroot, opt.phase)
        # se guardan los paths a los archivos .obj
        self.paths = self.make_dataset(self.dir)
        # se guardan los paths a los archivos .eseg
        # esto no es necesario para hacer completion
        # self.seg_paths = self.get_seg_files(self.paths, os.path.join(self.root, 'seg'), seg_ext='.eseg')
        # se guardan los paths a los archivos .seseg
        # self.sseg_paths = self.get_seg_files(self.paths, os.path.join(self.root, 'sseg'), seg_ext='.seseg')
        # se guardan las clases como numeros eg [0,1,2,3] y el offset con respecto al original eg [1,2,3,4] 
        # self.classes, self.offset = self.get_n_segs(os.path.join(self.root, 'classes.txt'), self.seg_paths)
        # se guarda el numero de clases
        # self.nclasses = len(self.classes)
        # se guarda el tamanho (numero de mallas)
        self.size = len(self.paths)
        # se calcula la media y desv. estandar de cada uno de los input_channels
        self.get_mean_std()
        # se guardan estos parametros extras en el opt
        opt.nclasses = 3 # canales de salida (x,y,z)
        #print('input %d' % (self.ninput_channels))
        opt.input_nc = self.ninput_channels

    # metodo para iterar sobre SegmentationData en BaseDataset (get_mean_std)
    def __getitem__(self, index):
        #print('Se esta llamando a __getitem__')
        # se tiene el path al .obj
        path = self.paths[index]
        # se obtiene la malla en un objeto tipo Mesh
        mesh = Mesh(file=path, opt=self.opt, hold_history=True, export_folder=self.opt.export_folder)
        # toda la informacion se devolvera como un diccionario
        meta = {}
        # se guarda la malla
        meta['mesh'] = mesh
        # se guarda la lista de labels (cada lista se llena con -1 hasta alcanzar ninput_edges)
        # label.shape == (ninput_edges)
        # label = read_seg(self.seg_paths[index]) - self.offset
        label = get_labels(path, mesh)
        label = pad(label, self.opt.ninput_edges, val=-1, dim=0)
        meta['label'] = label
        # se guarda la lista de soft_labels (cada matriz se llena con [-1 -1 ... -1] hasta alcanzar ninput_edges)
        # soft_label.shape == (nclasses x ninput_edges)
        #soft_label = read_sseg(self.sseg_paths[index])
        #meta['soft_label'] = pad(soft_label, self.opt.ninput_edges, val=-1, dim=0)
        # se guardan las edge_features ( cada matriz se llena con [0 0 ... 0] hasta alcanzar ninput_edges )
        edge_features = mesh.extract_features()
        #print('num_edges = %d' % (len(edge_features)))
        edge_features = pad(edge_features, self.opt.ninput_edges)
        # se normalizan las edge_features
        meta['edge_features'] = (edge_features - self.mean) / self.std
        return meta

    def __len__(self):
        return self.size

    # permite obtener el archivos seg
    @staticmethod
    def get_seg_files(paths, seg_dir, seg_ext='.seg'):
        segs = []
        for path in paths:
            segfile = os.path.join(seg_dir, os.path.splitext(os.path.basename(path))[0] + seg_ext)
            assert(os.path.isfile(segfile))
            segs.append(segfile)
        return segs

    # se obtiene la lista de clases y se devuelve el offset
    @staticmethod
    def get_n_segs(classes_file, seg_files):
        if not os.path.isfile(classes_file):
            all_segs = np.array([], dtype='float64')
            for seg in seg_files:
                all_segs = np.concatenate((all_segs, read_seg(seg)))
            segnames = np.unique(all_segs)
            np.savetxt(classes_file, segnames, fmt='%d')
        classes = np.loadtxt(classes_file)
        offset = classes[0]
        classes = classes - offset
        return classes, offset

    # devolver los paths a los archivos .obj
    @staticmethod
    def make_dataset(path):
        meshes = []
        assert os.path.isdir(path), '%s is not a valid directory' % path

        for root, _, fnames in sorted(os.walk(path)):
            for fname in fnames:
                if is_mesh_file(fname):
                    path = os.path.join(root, fname)
                    meshes.append(path)

        return meshes


def read_seg(seg):
    # Lee un archivo de texto donde cada fila debe contener exactamente la misma cantidad de datos
    # y se almacena como un ndarray de numpy con tipo de dato 'float64' en seg_labels
    seg_labels = np.loadtxt(open(seg, 'r'), dtype='float64')
    return seg_labels


def read_sseg(sseg_file):
    # Lee el archivo seseg y guarda la informacion como un ndarray de tipo int32
    # Cada numero positivo del input se reemplaza por 1 y los demas se mantienen en 0
    sseg_labels = read_seg(sseg_file)
    sseg_labels = np.array(sseg_labels > 0, dtype=np.int32)
    return sseg_labels

def get_labels(path, mesh):
    real_vs = []
    with open(path) as file:
        for line in file:
            if(line[0] == '#'):
                linee = line[1:]
                position = [float(x) for x in linee.split()]
                real_vs.append(position)
    labels = []
    for edge in mesh.edges:
        u = edge[0]
        v = edge[1]
        u = np.array(real_vs[u])
        v = np.array(real_vs[v])
        real_pos = (u + v) / 2
        labels.append(real_pos)
    labels = np.array(labels)
    return labels