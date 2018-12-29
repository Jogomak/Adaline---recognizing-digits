#include "mainwindow.h"

MainWindow::MainWindow(ErrorPlot *plot, QWidget *parent)
    : QMainWindow(parent)
{
    this->plot = plot;

    learning_sample_size = 100;
    image_original_size = 5;
    image_pixels_original_count = image_original_size * image_original_size;

    this->setMinimumSize(400, 460);
    image_width_height = 400;
    image_in = new QImage(image_original_size, image_original_size, QImage::Format_ARGB32);
    image_in_bits = reinterpret_cast<unsigned int *>(image_in->bits());
    input_data = new double[image_pixels_original_count];
    input_data_dft_amplitude = new double[image_pixels_original_count];
    white = 0xFFFFFFFF;
    black = 0xFF000000;

    button_clear_image = new QPushButton("Clear", this);
    button_clear_image->setGeometry(155, 425, 90, 23);
    connect(button_clear_image, SIGNAL(pressed()), this, SLOT(clear_image()));

    label_result = new QLabel("?", this);
    label_result->setAlignment(Qt::AlignHCenter);
    label_result->setGeometry(10, 405, 380, 23);

    std::srand(static_cast<unsigned int>(time(nullptr)));
    int test = static_cast<int>(time(nullptr));
    image_in->fill(white);
    initialize_adaline_and_prepare_data();
    learn_adaline();
    std::cout << "czas: " << time(nullptr) - test << '\n';
}

MainWindow::~MainWindow()
{
    delete image_in;
    delete button_clear_image;

    for(int i = 0; i < learning_sample_size; i++){
        delete []learning_samples_dft_amplitude[i];
    }
    delete []learning_samples_dft_amplitude;

    for(int i = 0; i < 10; i++){
        delete adalines[i];
    }

    delete []weights_dft;
    delete []input_data;
    delete []input_data_dft_amplitude;
}

void MainWindow::set_plot_data(int sheet_number, QVector<double> y){
    plot->set_plot_data(sheet_number, y);
}

void MainWindow::initialize_adaline_and_prepare_data(){
    learning_sample_tmp = new double[image_pixels_original_count];
    learning_samples_dft_amplitude = new double*[learning_sample_size];
    std::complex<double> x;
    char image_tmp_path[26] = {':', '/', 'l', 'e', 'a', 'r', 'n', 'i', 'n', 'g', '_', 's', 'a', 'm', 'p', 'l', 'e', 's', '/', '0', '0', '.', 'p', 'n', 'g', '\0'};
    QImage image_tmp;
    unsigned int *image_tmp_bits;

    weights_dft = new std::complex<double>[static_cast<int>(pow(image_pixels_original_count - 1, 2) + 1)];
    double temp = 2. / image_pixels_original_count * acos(-1);

    for(int i = 0; i < pow(image_pixels_original_count - 1, 2) + 1; i++){
        weights_dft[i].real(cos(-temp * i));
        weights_dft[i].imag(sin(-temp * i));
    }

    for(int i = 0; i < learning_sample_size; i++){
        image_tmp_path[19] = char(48 + i / 10);
        image_tmp_path[20] = char(48 + i % 10);
        image_tmp = QImage(image_tmp_path, "PNG");

        image_tmp_bits = reinterpret_cast<unsigned int *>(image_tmp.bits());

        for(int j = 0; j < image_pixels_original_count; j++){
            if(image_tmp_bits[j] == black){
                learning_sample_tmp[j] = 1;
            }else{
                learning_sample_tmp[j] = -1;
            }
        }

        learning_samples_dft_amplitude[i] = new double[image_pixels_original_count];
        for(int j = 0; j < image_pixels_original_count; j++){
            x = 0;
            for(int k = 0; k < image_pixels_original_count; k++){
                x.imag(x.imag() + learning_sample_tmp[k] * weights_dft[k * j].imag());
                x.real(x.real() + learning_sample_tmp[k] * weights_dft[k * j].real());
            }

            learning_samples_dft_amplitude[i][j] = sqrt(std::norm(x));
        }
    }

    for(int i = 0; i < 10; i++){
        adalines[i] = new AdaLine(image_pixels_original_count, learning_samples_dft_amplitude, learning_sample_size, i);
    }

    for(int i = 0; i < image_pixels_original_count; i++){
        input_data[i] = -1;
    }

    delete []learning_sample_tmp;
}

void MainWindow::learn_adaline(){
    plot->set_plot_xrange(2000);
    //set_plot_data(0, adalines[0]->learn(2000, 1, 1));
    for(int i = 0; i < 10; i++){
        set_plot_data(i, adalines[i]->learn(2000, 1, 10));
    }

}

void MainWindow::check(){
    int answer = 0;
    double max = -999;

    std::complex<double> x;
    for(int j = 0; j < image_pixels_original_count; j++){
        x = 0;
        for(int k = 0; k < image_pixels_original_count; k++){
            x.imag(x.imag() + input_data[k] * weights_dft[k * j].imag());
            x.real(x.real() + input_data[k] * weights_dft[k * j].real());

        }
        input_data_dft_amplitude[j] = sqrt(std::norm(x));
    }

    for(int i = 0; i < 10; i++){
        std::cout << i << ' ' << adalines[i]->get_answer(input_data_dft_amplitude) << '\n';
        if(max < adalines[i]->get_answer(input_data_dft_amplitude)){
            max = adalines[i]->get_answer(input_data_dft_amplitude);
            answer = i;
        }
    }

    label_result->setText(QString(char(48 + answer)));
}

void MainWindow::paintEvent(QPaintEvent *){
    static QSize image_size = QSize(image_width_height, image_width_height);
    QPainter painter(this);

    painter.drawImage(0, 0, (*image_in).scaled(image_size, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}

void MainWindow::mousePressEvent(QMouseEvent *mouse_event){
    x = mouse_event -> x() / 80;        //division by 80, to get x, y from original image
    y = mouse_event -> y() / 80;
    mouse_button_pressed = mouse_event->button();

    if(mouse_button_pressed == 0x00000001){
        painting_color = black;
    }else{
        painting_color = white;
    }

    put_pixel();
}

void MainWindow::mouseMoveEvent(QMouseEvent *mouse_event){
    x = mouse_event -> x() / 80;
    y = mouse_event -> y() / 80;

    put_pixel();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *){
    update();
}

void MainWindow::put_pixel(){
    if(x >= 0 && x < image_original_size && y >= 0 && y <= image_pixels_original_count - image_original_size){
        image_in_bits[x + y * image_original_size] = painting_color;

        if(painting_color == black){
            input_data[x + y * image_original_size] = 1;
        } else {
            input_data[x + y * image_original_size] = -1;
        }
    }

    check();
    update();
}

void MainWindow::clear_image(){
    image_in->fill(white);
    update();
}
