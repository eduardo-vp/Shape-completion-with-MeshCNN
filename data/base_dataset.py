import torch.utils.data as data
import numpy as np
import pickle
import os

class BaseDataset(data.Dataset):

    def __init__(self, opt):
        self.opt = opt
        self.mean = 0
        self.std = 1
        self.ninput_channels = None
        super(BaseDataset, self).__init__()

    def get_mean_std(self):
        """
        Realiza el computo de la media y la deviacion estandar de la data de entrenamiento
        Sea N = numero de input_channels
        Este metodo devuelve 
        mean : ndarray (N x numero de mallas)
        std: ndarray (N x numero de mallas)
        Aqui N = 5 (angulo diedro, los dos angulos internos de los triangulos y 
        las relaciones entre el largo de la arista y la altura de los triangulos)
        """
        # se crea el nombre del archivo que se creara o leera dependiendo del caso
        mean_std_cache = os.path.join(self.root, 'mean_std_cache.p')
        if not os.path.isfile(mean_std_cache):
            # si no es encontrado el archivo con las medias y desviaciones estandar, debe calcularse
            print('Calculating mean/std')
            # no hay que realizar el data_augmentation durante el calculo, por esto la volvemos temporalmente 1 y luego se regresa
            num_aug = self.opt.num_aug
            self.opt.num_aug = 1
            # se crea el ndarray de mean y std que iran acumulando los valores
            mean, std = np.array(0), np.array(0)
            for i, data in enumerate(self):
                if i % 500 == 0:
                    print('{} de {}'.format(i, self.size))
                # por cada malla se extrae todos los edge_features
                features = data['edge_features']
                # se acumula la media y desv. estandar de todas las aristas en cada uno de los 5 input_channels
                # tanto mean como std tienen shape (input_channels)
                mean = mean + features.mean(axis=1)
                std = std + features.std(axis=1)
            # se divide entre la cantidad de elementos
            mean = mean / (i + 1)
            std = std / (i + 1)
            # se guarda todo en un diccionario (media, desv. estandar e input_channels)
            transform_dict = {'mean': mean[:, np.newaxis], 'std': std[:, np.newaxis],
                              'ninput_channels': len(mean)}
            # se abre el archivo de mean_std_cache para guardar ahi toda esta informacion como pickle
            with open(mean_std_cache, 'wb') as f:
                pickle.dump(transform_dict, f)
            print('saved: ', mean_std_cache)
            # el parametro de num_aug retorna
            self.opt.num_aug = num_aug
        # abrir archivo de mean_std_cache para lectura
        with open(mean_std_cache, 'rb') as f:
            # leer el diccionario
            transform_dict = pickle.load(f)
            print('loading mean/std')
            # se guardan los valores del archivo
            self.mean = transform_dict['mean']
            self.std = transform_dict['std']
            self.ninput_channels = transform_dict['ninput_channels']

def collate_fn(batch):
    # Crea mini-batch de tensores
    # Es la que devuelve el batch al DataLoader cuando se itera por el
    meta = {}
    keys = batch[0].keys()
    for key in keys:
        meta.update({key: np.array([d[key] for d in batch])})
    return meta