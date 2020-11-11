import torch
import torch.nn as nn
import torch.nn.functional as F

class MeshConv(nn.Module):

    """
    Realiza convolucion entre una arista y sus 4 aristas incidentes (el anillo de aristas)
    en el forward toma:
    x: edge features (Batch x Features x Aristas)
    mesh: lista de las mallas
    y aplica convolucion
    """
    # ese k = 5 viene de los 5 datos que se usan para la convolucion
    def __init__(self, in_channels, out_channels, k=5, bias=True):
        super(MeshConv, self).__init__()
        self.conv = nn.Conv2d(in_channels=in_channels, out_channels=out_channels, kernel_size=(1, k), bias=bias)
        self.k = k

    def __call__(self, edge_f, mesh):
        return self.forward(edge_f, mesh)

    def forward(self, x, mesh):
        x = x.squeeze(-1)
        G = torch.cat([self.pad_gemm(i, x.shape[2], x.device) for i in mesh], 0)
        # construir el 'neighborhood image' y aplicar convolucion
        G = self.create_GeMM(x, G)
        x = self.conv(G)
        return x

    def flatten_gemm_inds(self, Gi):
        (b, ne, nn) = Gi.shape
        ne += 1
        batch_n = torch.floor(torch.arange(b * ne, device=Gi.device).float() / ne).view(b, ne)
        add_fac = batch_n * ne
        add_fac = add_fac.view(b, ne, 1)
        add_fac = add_fac.repeat(1, 1, nn)
        # flatten Gi
        Gi = Gi.float() + add_fac[:, 1:, :]
        return Gi

    def create_GeMM(self, x, Gi):
        """
        junta las edge features (x) con los indices del anillo de aristas (Gi)
        aplica funciones simetricas para manejar invariancia en el orden
        devuelve una 'imagen falsa' sobre la cual se puede aplicar convolucion 2d
        dimensiones del output: Batch x Canales x Aristas x 5
        """
        Gishape = Gi.shape
        # hacer pad de la primera fila de cada sample en el batch con ceros
        padding = torch.zeros((x.shape[0], x.shape[1], 1), requires_grad=True, device=x.device)
        x = torch.cat((padding, x), dim=2)
        Gi = Gi + 1 # shift

        # flatten indices
        Gi_flat = self.flatten_gemm_inds(Gi)
        Gi_flat = Gi_flat.view(-1).long()
        
        odim = x.shape
        x = x.permute(0, 2, 1).contiguous()
        x = x.view(odim[0] * odim[2], odim[1])

        f = torch.index_select(x, dim=0, index=Gi_flat)
        f = f.view(Gishape[0], Gishape[1], Gishape[2], -1)
        f = f.permute(0, 3, 1, 2)

        # aplicar funciones simetricas para una convolucion invariante al orden
        x_1 = f[:, :, :, 1] + f[:, :, :, 3] # suma entre 1 y 3
        x_2 = f[:, :, :, 2] + f[:, :, :, 4] # suma entre 2 y 4
        x_3 = torch.abs(f[:, :, :, 1] - f[:, :, :, 3]) # valor absoluto de la diferencia entre 1 y 3
        x_4 = torch.abs(f[:, :, :, 2] - f[:, :, :, 4]) # valor absoluto de la diferencia entre 2 y 4
        f = torch.stack([f[:, :, :, 0], x_1, x_2, x_3, x_4], dim=3)
        return f

    def pad_gemm(self, m, xsz, device):
        """
        extrae las 4 aristas adyacentes -> m.gemm_edges
        que tiene dimensiones ( numero de aristas, 4 )
        agregar el edge_id mismo para que sea ( numero de aritas, 5 )
        luego hacer pad para que quede del tamanho deseado ( xsz, 5 )
        """
        padded_gemm = torch.tensor(m.gemm_edges, device=device).float()
        padded_gemm = padded_gemm.requires_grad_()
        padded_gemm = torch.cat((torch.arange(m.edges_count, device=device).float().unsqueeze(1), padded_gemm), dim=1)
        # pad using F
        padded_gemm = F.pad(padded_gemm, (0, 0, 0, xsz - m.edges_count), "constant", 0)
        padded_gemm = padded_gemm.unsqueeze(0)
        return padded_gemm
