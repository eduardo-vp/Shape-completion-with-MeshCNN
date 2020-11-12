from options.test_options import TestOptions
from data import DataLoader
from models import create_model
from util.writer import Writer


def run_test(epoch=-1):
    print('Running Test')
    opt = TestOptions().parse()
    opt.serial_batches = True  # no shuffle
    dataset = DataLoader(opt)
    model = create_model(opt)
    writer = Writer(opt)
    # test
    writer.reset_counter()
    for i, data in enumerate(dataset):
        #print(type(data))
        model.set_input(data)
        loss = model.test()
        writer.update_counter(loss, data['mesh'].shape[0])
    writer.print_loss(epoch, writer.loss)
    return writer.loss


if __name__ == '__main__':
    run_test()
